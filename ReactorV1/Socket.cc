#include "Socket.hh"
#include <func.h>
#include <iostream>
#include <sys/socket.h>
using std::cout;
using std::endl;


namespace ReactorV1{
Socket::Socket()
:_fd(-1)
{
    _fd=socket(AF_INET,SOCK_STREAM,0);
    if(_fd<0){
        perror("socket");
    }
}

Socket::Socket(int fd)
:_fd(fd)
{

}

Socket::~Socket(){
    if(_fd>0){
        close(_fd);
    }
}

int Socket::getFd() const{
    return _fd;
}

void Socket::shutdownWrite(){
    //只关闭写端，在主动断开连接时使用
    shutdown(_fd,SHUT_WR);
}

}


