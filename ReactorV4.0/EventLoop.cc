#include "EventLoop.hh"
#include "Acceptor.hh"
#include "TcpConnection.hh"
#include <cerrno>
#include <iostream>
#include <sys/epoll.h>
using std::cout;
using std::endl;

namespace ReactorV40{

EventLoop::EventLoop(Acceptor & ac)
:_epfd(createEpollFd())
,_acceptor(ac)
,_isLooping(false)
,_evtArr(1024)
{
    addEpollReadEvent(_acceptor.fd());
}

EventLoop::~EventLoop(){
    if(_epfd>0){
        close(_epfd);
    }
}

void EventLoop::loop(){
    _isLooping=true;
    while(_isLooping){
        waitEpollFd();
    }
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
    }else{
        for(int i=0;i<nready;++i){
            int fd=_evtArr[i].data.fd;
            if(fd==_acceptor.fd()){
                handleNewConnection();
            }else{
                handleMessage(fd);
            }
        }
    }
}


int EventLoop::createEpollFd(){
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
    ev.events=EPOLLIN;
    ev.data.fd=fd;
    int ret=epoll_ctl(_epfd,EPOLL_CTL_DEL,fd,&ev);
    if(ret<0){
        perror("epoll_ctl");
    }
}


void EventLoop::handleNewConnection(){
    //1.获取与对端进行交互的netfd
    int netfd=_acceptor.accept();
    if(netfd<0){
        return;
    }
    //添加到epoll的监听红黑树上
    addEpollReadEvent(netfd);
    //2.1 创建TcpConnection
    TcpConnectionPtr con(new TcpConnection(netfd));
   //2.2 注册函数对象时，不能使用移动语义
    con->setAllCallbacks(_onConnection,
                         _onMessage,
                         _onClose);

    //3. 调用新连接建立时的函数对象
    con->handleNewConnectionCallback();
    //4.管理con对象，放入map中存储起来
    _cons[netfd]=con;
}

void EventLoop::handleMessage(int fd){
    //1.通过fd查找到TcpConnection对象
    auto iter=_cons.find(fd);
    if(iter!=_cons.end()){

        //2.查找成功后，先判断连接是否断开
        bool isClosed=iter->second->isClosed();
        if(isClosed){
            
            //2.1 连接断开的情况
            iter->second->handleCloseCallback();
            //从epoll监听的红黑树上删除
            delEpollReadEvent(fd);
            //从map中删除该连接对象
            _cons.erase(fd);
        }else{
            //2.2 收到消息时的情况
            iter->second->handleMessageCallback();
        }
    }
}


}


