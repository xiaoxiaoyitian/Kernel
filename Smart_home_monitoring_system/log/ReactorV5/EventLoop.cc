#include "EventLoop.hh"

namespace Titan{

    EventLoop::EventLoop(Acceptor &ac)
    :_epfd(createEpollfd())
    ,_eventfd(createEventFd())
    ,_acceptor(ac)
    ,_isLooping(false)
    ,_evtArr(1024)
    {
        addEpollReadEvent(_acceptor.fd());
        addEpollReadEvent(_eventfd);
    }

    EventLoop::~EventLoop(){
        if(_epfd>0){
            close(_epfd);
        }
    }
    
    EventLoop::loop(){
        _isLooping=true;
        while(_isLooping){
            waitEpollFd();
        }
    }


    void EventLoop::runInLoop(Functor && f){
        {
            MutexLockGuard  autolock(_mutex);
            _pendingFunctors.push_back(std::move(f));
        }
        wakeup();
    }

    void EventLoop::waitEpollFd(){

        int nready=epoll_wait(_epfd,_evtArr.data(),_evtArr.size(),5000);
        if(nready==-1&&errno==EINTR){
            return;
        }else if(nready==-1){
            perror("epoll_wait");
            return;
        }else if(nready==0){
            cout<<"epoll timeout"<<endl;
            return;
        }else{
            for(int i=0;i<nready;++i){
                int fd=_evtArr[i].data.fd;
                if(fd==_acceptor.fd()){
                    handleNewConnection();
                }else if(fd==_eventfd){
                    handleReadEvent();
                    doPendingFunctors();
                }else{
                    handleMessage(fd);
                }
            }
        }
    }

    int EventLoop::createEpollfd(){
        int fd=epoll_create1(0);
        if(fd<0){
            perror("epoll_create1");
        }
        return fd;
    }

    void EventLoop::addEpollReadEvent(int fd){
        struct epoll_event ev;
        memset(&ev,0,sizeof(ev));
        ev.events=EPOLLIN;
        ev.data.fd=fd;

        int ret=epoll_ctl(_epfd,EPOLL_CTL_ADD,fd,&ev);
        if(ret<0){
            perror("epoll_ctl");
        }
    }

    void EventLoop::delEpollReadEvent(int fd){
        struct epoll_event ev;
        memset(&ev,0,sizeof(ev));

        ev.data.fd=fd;
        int ret=epoll_ctl(_epfd,EPOLL_CTL_DEL,fd,&ev);
        if(ret<0){
            perror("epoll_ctl");
        }
    }


    void EventLoop::handleNewConnection(){
        int netfd=_acceptor.accept();
        if(netfd<0){
            return;
        }

        addEpollReadEvent(netfd);

        TcpConnectionPtr con(new TcpConnection(netfd,this));

        con->setAllCallbacks(_onConnection,
                            _onMessage,
                            _onClose);

        con->handleNewConnectionCallback();
        _cons[netfd]=con;

    }

    void EventLoop::handleMessage(int fd){
        auto iter=_cons.find(fd);
        if(iter!=_cons.end()){
            bool isClosed=iter->second->isClosed();
            if(isClosed){
                iter->second->handleCloseCallback();
                delEpollReadEvent(fd);
                _cons.erase(fd);
            }else{
                iter->second->handleMessageCallback();
            }
        }
    }

    int EventLoop::createEventFd()
    {
        int fd=::eventfd(0,0);
        if(fd<0){
            perror("eventfd");
        }
        return fd;
    }

    void EventLoop::handleReadEvent(){
        uint64_t howmany=0;
        int ret=read(_eventfd,&howmany,sizeof(howmany));
        if(ret!=sizeof(howmany)){
            perror("read");
        }
    }

    void EventLoop::wakeup(){
        uint64_t one=1;
        int ret=write(_eventfd,&one,sizeof(one));
        if(ret!=sizeof(one)){
            perror("write");
        }
    }

    void EventLoop::doPendingFunctors(){

        vector<Functor> temp;
        {
            MutexLockGuard autolock(_mutex);
            temp.swap(_pendingFunctors);
        }

        for(auto &f:temp){
            f();
        }
    }


}


