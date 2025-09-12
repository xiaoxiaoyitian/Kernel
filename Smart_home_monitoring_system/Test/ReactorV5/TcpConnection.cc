#include "TcpConnection.hh"
#include "EventLoop.hh"
#include "Message.hh"
namespace Titan{

    TcpConnection::TcpConnection(int fd ,EventLoop *loop)
    :_sock(fd)
    ,_sockIO(fd)
    ,_localAddr(getLocalAddr())
    ,_peerAddr(getPeerAddr())
    ,_loop(loop)
    {

    }

    TcpConnection::~TcpConnection(){
        cout<<"~TcpConnection()"<<endl;
    }

    bool TcpConnection::isClosed() {
        char buff[20]={0}; 
        return _sockIO.recvPeek(buff,sizeof(buff))==0;
    }

    string TcpConnection::receiveData(){
        char buf[65535]={0};
        _sockIO.readline(buf,sizeof(buf));
        return string(buf);
    }

    //接收到的数据TLV
    int TcpConnection::readPacket(Packet &packet){
       return _sockIO.readPacket(packet);
    }

    void TcpConnection::send(const string &msg){
        _sockIO.sendn(msg.data(),msg.size());
    }
    
    void TcpConnection::sendInLoop(const TLV &tlv){
        int tlvlen=8+tlv.length;
        string msg;
        msg.assign((const char *)&tlv,tlvlen);
        sendInLoop(msg);
    }
   
    void TcpConnection::sendInLoop(const string & msg){
       if(_loop){
           _loop->runInLoop(std::bind(&TcpConnection::send,this,msg));
       }
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
        struct  sockaddr_in addr;
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

    void TcpConnection::printfConnectionInfo(){
        printf("[%s/%d]-------------->[%s/%d]  has connected successfully!\n",_localAddr.ip().data(),_localAddr.port(),_peerAddr.ip().data(),_peerAddr.port());
    }

    void TcpConnection::printfDisconnectionInfo(){
        printf("[%s/%d]-------------->[%s/%d]  has Closed !\n",_localAddr.ip().data(),_localAddr.port(),_peerAddr.ip().data(),_peerAddr.port());
    }
}
