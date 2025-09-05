#include "Socket.hh"
#include <iostream>
#include <sys/socket.h>
using std::cout;
using std::endl;


namespace ReactorV2{

Socket::Socket()
:_fd(-1)
{
    _fd=socket(AF_INET,SOCK_STREAM,0);
    if(_fd<0){
        perror("socket");
    }
}

Socket::~Socket(){
    if(_fd>0){
        close(_fd);
    }
}

void Socket::shutdownWrite(){
    shutdown(_fd,SHUT_WR);
}

}


