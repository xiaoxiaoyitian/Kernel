#ifndef  __TcpConnection_H__
#define  __TcpConnection_H__

#include "Socket.hh"
#include "SocketIO.hh"
#include "InetAddress.hh"
#include <func.h>
#include <functional>
#include <memory>

using std::shared_ptr;
using std::function;
namespace ReactorV2{

class TcpConnection;
using TcpConnectionPtr=shared_ptr<TcpConnection>;
using TcpConnectionCallback=function<void(TcpConnectionPtr)>;

class TcpConnection :public std::enable_shared_from_this<TcpConnection>{
public:
    TcpConnection(int fd);
    ~TcpConnection();
    bool isClosed();
    
    void setAllCallbacks(const TcpConnectionCallback &cb1,
                         const TcpConnectionCallback &cb2,
                         const TcpConnectionCallback &cb3)
    {
        _onConnection=cb1;
        _onMessage=cb2;
        _onClose=cb3;
    } 

    string toString();
    void send(const string &msg);
    string receive();

    void handleNewConnectionCallback();
    void handleMessageCallback();
    void handleCloseCallback();

private:
    InetAddress getLocalAddr();
    InetAddress getPeerAddr();
private:
    Socket _sock;
    SocketIO _sockio;
    InetAddress _localAddr;
    InetAddress _peerAddr;

    TcpConnectionCallback _onConnection;
    TcpConnectionCallback _onMessage;
    TcpConnectionCallback _onClose;
};

}
#endif
