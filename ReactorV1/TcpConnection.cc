#include "TcpConnection.hh"
#include "InetAddress.hh"
#include <cstring>
#include <iostream>
#include <sstream>
#include <sys/socket.h>
#include <unistd.h>
using std::cout;
using std::endl;

namespace ReactorV1{

TcpConnection::TcpConnection(int fd)
:_sock(fd)
,_sockio(fd)
,_localAddr(getLocalAddr())
,_peerAddr(getPeerAddr())
{

}

TcpConnection::~TcpConnection(){

}

bool TcpConnection::isClosed(){
    char buf[20]={0};
    int ret=_sockio.readPeek(buf,sizeof(buf));
    return ret==0;
}


string TcpConnection::toString(){
    std::ostringstream oss;
    oss<<"tcp连接从ip："<<_localAddr.ip()<<"，端口号："<<_localAddr.port()
        <<"-> ip："<<_peerAddr.ip()<<"，端口号："<<_peerAddr.port()<<endl;

    return oss.str();
}

void TcpConnection::send(const string &msg){
    int ret=_sockio.writen(msg.data(),msg.size());
    cout<<"发送了 "<<ret<<"个字节的数据！"<<endl;
}

string TcpConnection::receive(){
    char buf[65535]={0};
    int ret=_sockio.readLine(buf,sizeof(buf));
    return string(buf,ret);
}

InetAddress TcpConnection::getLocalAddr(){
    struct sockaddr_in  addr;
    socklen_t len=sizeof(addr);
    memset(&addr,0,len);
    int ret=getsockname(_sock.getFd(),(struct sockaddr*)&addr,&len);
    if(ret<0){
        perror("getsockname");
    }
    return InetAddress(addr);
}

InetAddress TcpConnection::getPeerAddr(){
    struct sockaddr_in addr;
    socklen_t len=sizeof(addr);
    memset(&addr,0,len);
    int ret=getpeername(_sock.getFd(),(struct sockaddr*)&addr,&len);
    if(ret<0){
        perror("getpeername");
    }

    return InetAddress(addr);
}


}


