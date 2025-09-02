#include "TcpConnection.hh"
#include "InetAddress.hh"
#include <func.h>
#include <iostream>
#include <netinet/in.h>
#include <sstream>
#include <unistd.h>
using std::cout;
using std::endl;

namespace ReactorV1{

TcpConnection::TcpConnection(int fd)
:_sockio(fd)
,_sock()
,_localAddr(getLocalAddr())
,_peerAddr(getPeerAddr())
{
}

TcpConnection::~TcpConnection(){

}

void TcpConnection::send(const string &msg){
    int ret=_sockio.writen(msg.data(),msg.size());
    cout<<"write "<<ret<<" bytes."<<endl;
}

string TcpConnection::receive(){
    char buf[65535]={0};
    int ret=_sockio.readLine(buf,sizeof(buf));
    return string(buf,ret);
}

bool TcpConnection::isClosed() const{
    char buf[20]={0};
    int ret=_sockio.readPeek(buf,sizeof(buf));
    return ret==0;
}

string TcpConnection::toString() const{
    std::ostringstream oss;
    oss<<"tcp "<<_localAddr.ip()<<":"<<_localAddr.port()
        <<"->"<<_peerAddr.ip()<<":"<<_peerAddr.port()<<endl;
    return oss.str();
}



InetAddress TcpConnection::getPeerAddr(){
    struct sockaddr_in addr;
    socklen_t len =sizeof(addr);
    memset(&addr,0,sizeof(addr));
    int ret=getpeername(_sock.getFd(),(struct sockaddr *)&addr,&len);
    if(ret<0){
        perror("getpeername");
    }
    return InetAddress(addr);
}

InetAddress TcpConnection::getLocalAddr(){
    struct sockaddr_in addr;
    socklen_t len=sizeof(addr);
    memset(&addr,0,sizeof(addr));
    int ret=getsockname(_sock.getFd(),(struct sockaddr *)&addr,&len);
    if(ret<0){
        perror("getsockname");
    }
    return InetAddress(addr);
}
























}


