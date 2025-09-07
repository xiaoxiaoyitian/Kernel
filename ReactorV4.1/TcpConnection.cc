#include "TcpConnection.hh"
#include "EventLoop.hh"
#include "InetAddress.hh"
#include <iostream>
#include <netinet/in.h>
#include <sstream>
#include <sys/socket.h>
#include <unistd.h>
using std::cout;
using std::endl;


namespace ReactorV41{

TcpConnection::TcpConnection(int fd,EventLoop * loop)
:_sock(fd)
,_sockIO(fd)
,_localAddr(getLocalAddr())
,_peerAddr(getPeerAddr())
,_loop(loop)
{}

TcpConnection::~TcpConnection(){
    cout<<"~TcpConnection()"<<endl;
}

bool TcpConnection::isClosed() const{
    char buf[20]={0};
    int ret=_sockIO.recvPeek(buf,sizeof(buf));
    return ret==0;
}

string TcpConnection::receive(){

    char buf[65535]={0};
    int ret=_sockIO.readLine(buf,sizeof(buf));
    return string(buf,ret);
}

void TcpConnection::send(const string & msg){
    int ret=_sockIO.sendn(msg.data(),msg.size());
    cout<<"发送了 "<<ret<<"个字节的数据！"<<endl;
}

void TcpConnection::sendInLoop(const string &msg){
    if(_loop){
        _loop->runInLoop(std::bind(&TcpConnection::send,this,msg));
    }
}

string TcpConnection::toString() const{
    std::ostringstream oss;
    oss<<"TCP IP为 "<<_localAddr.ip()<<"，端口号为 "<<_localAddr.port()
        <<"---> IP为 "<<_peerAddr.ip()<<",端口号为 "<<_peerAddr.port()<<endl;

    return oss.str();
}

void TcpConnection::handleNewConnectionCallback(){
    if(_onConnection){
        _onConnection(shared_from_this());
    }
}

void TcpConnection::handleMessageCallback(){
    if(_onMessage){
        _onMessage(shared_from_this());
    }
}

void TcpConnection::handleCloseCallback(){
    if(_onClose){
        _onClose(shared_from_this());
    }
}

InetAddress TcpConnection::getLocalAddr(){
    struct sockaddr_in addr;
    socklen_t len=sizeof(addr);
    memset(&addr,0,sizeof(addr));
    int ret=getsockname(_sock.fd(),(struct sockaddr *)&addr,&len);
    if(ret<0){
        perror("getsockname");
    }
    return InetAddress(addr);
}

InetAddress TcpConnection::getPeerAddr(){
    struct sockaddr_in addr;
    socklen_t len=sizeof(addr);
    memset(&addr,0,sizeof(addr));
    int ret=getpeername(_sock.fd(),(struct sockaddr *)&addr,&len);
    if(ret<0){
        perror("getpeername");
    }
    return InetAddress(addr);
}

}


