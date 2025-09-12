#include "TcpConnection.hh"
#include "EventLoop.hh"
#include "TLV.hh"
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
        TLV tlv;
        char buf[20]={0}; 
        int ret=_sockIO.readTLV(tlv);
        return ret==0;
    }

    string TcpConnection::receiveData(){
        TLV tlv;
        char buf[65535]={0};
        int ret=_sockIO.readTLV(tlv);
        return string(tlv.getData());
    }

    //接收到的数据TLV
    TLV TcpConnection::receive(){
        TLV tlv;
        char buf[65535]={0};
        int ret=_sockIO.readTLV(tlv);
        return tlv;
    }

    void TcpConnection::send(TLV &tlv){
        int ret=_sockIO.sendTLV(tlv);
    }

    void TcpConnection::sendInLoop(TLV &tlv){
        if(_loop){
            _loop->runInLoop(std::bind(&TcpConnection::send,this,tlv));
        }
    }
   
    // void TcpConnection::sendInLoop(const string & msg){
   //     if(_loop){
   //         _loop->runInLoop();
   //     }
    //}


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
