#include "Acceptor.hh"
#include <asm-generic/socket.h>
#include <iostream>
#include <sys/socket.h>
using std::cout;
using std::endl;


namespace ReactorV2{

Acceptor::Acceptor(const string &ip,unsigned short port)
:_sock()
,_addr(ip,port)
{

}
Acceptor::~Acceptor(){

}

void Acceptor::ready(){
    setReuseAddr(true);
    setReusePort(true);
    bind();
    listen();
}

int Acceptor::accept(){
    int netfd=::accept(_sock.getFd(),nullptr,nullptr);
    if(netfd<0){
        perror("accept");

    }
    return netfd;
}

void Acceptor::bind(){
    int ret=::bind(_sock.getFd(),(const struct sockaddr *)_addr.getInetAddress(),sizeof(_addr));
    if(ret<0){
        perror("bind");
        return;
    }
}

void Acceptor::listen(){
    int ret=::listen(_sock.getFd(),20000);
    if(ret<0){
        perror("listen");
    }
}


void Acceptor::setReuseAddr(bool on){
    int one=on;
    int ret=setsockopt(_sock.getFd(),SOL_SOCKET,SO_REUSEADDR,&one,sizeof(one));
    if(ret<0){
        perror("setsockopt");
    }
}


void Acceptor::setReusePort(bool on){
    int one=on;
    int ret=setsockopt(_sock.getFd(),SOL_SOCKET,SO_REUSEPORT,&one,sizeof(one));
    if(ret<0){
        perror("setsockopt");
    }
}


}

