#include "Acceptor.hh"
#include <asm-generic/socket.h>
#include <iostream>
#include <sys/socket.h>
#include <sys/stat.h>
using std::cout;
using std::endl;


namespace ReactorV3{

Acceptor::Acceptor(const string &ip,unsigned short port)
:_addr(ip,port)
,_listensock()
{}

void Acceptor::ready(){
    setReuseAddr(true);
    setReusePort(true);
    bind();
    listen();
}
int Acceptor::accept(){
    int netfd=::accept(fd(),nullptr,nullptr);
    if(netfd<0){
        perror("accept");
    }
    return netfd;
}

void Acceptor::bind(){
    int ret=::bind(fd(),(struct sockaddr *)_addr.getInetAddressPtr(),sizeof(_addr));
    if(ret<0){
        perror("bind");
    }
}

void Acceptor::listen()
{
    int ret=::listen(fd(),20000);
    if(ret<0){
        perror("listen");
    }
}

void Acceptor::setReuseAddr(bool on){
    int one=on;
    int ret=setsockopt(fd(),SOL_SOCKET,SO_REUSEADDR,&one,sizeof(one));
    if(ret<0){
        perror("setsockopt");
    }
}

void Acceptor::setReusePort(bool on){
    int one=on;
    int ret=::setsockopt(fd(),SOL_SOCKET,SO_REUSEPORT,&one,sizeof(one));
    if(ret<0){
        perror("setsockopt");
    }
}

}


