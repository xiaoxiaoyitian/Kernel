#include "EventLoop.hh"
#include "Acceptor.hh"
#include "TcpConnection.hh"
#include <cerrno>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/epoll.h>
using std::cout;
using std::endl;

namespace ReactorV2{

EventLoop::EventLoop(Acceptor &ac)
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

    ev.data.fd=fd;
    int ret=epoll_ctl(_epfd,EPOLL_CTL_DEL,fd,&ev);
    if(ret<0){
        perror("epoll_ctl");
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
        cout<<"epoll timeout."<<endl;
        return;
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

void EventLoop::handleNewConnection(){

    int netfd=_acceptor.accept();
    if(netfd<0){
        return;
    }
    addEpollReadEvent(netfd);

    TcpConnectionPtr cons(new TcpConnection(netfd));

    cons->setAllCallbacks(_onConnection,
                          _onMessage,
                          _onClose);

    cons->handleNewConnectionCallback();
    _cons[netfd]=cons;
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


}


