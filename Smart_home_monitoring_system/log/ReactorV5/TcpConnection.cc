#include "TcpConnection.hh"
#include "TLV.hh"
namespace Titan{

    TcpConnection::TcpConnection(int fd ,TLV &tlv,EventLoop *loop)
    :_sock(fd)
    ,_sockIO(fd,tlv)
    ,_localAddr(getLocalAddr())
    ,_peerAddr(getPeerAddr())
    ,_loop(loop)
    {

    }

    TcpConnection::~TcpConnection(){
        cout<<"~TcpConnection()"<<endl;
    }

    bool TcpConnection::isClosed() {
        char buf[20]={0};
        TLV & tlv=_sockIO.getInstanceTLV();
        
        int ret=_sockIO.readTLV(tlv);
        return ret==0;
    }

    //接收到的数据
    string TcpConnection::receive(){
        char buf[65535]={0};
        TLV & tlv=_sockIO.getInstanceTLV();
        int ret=_sockIO.readTLV(tlv);

        //！！！！！！！！！！！需要修改！！！！！！！！！！
        return string(tlv.getData());
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





}