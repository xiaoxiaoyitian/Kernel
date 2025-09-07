#ifndef  __TcpConnection_H__
#define  __TcpConnection_H__
#include "InetAddress.hh"
#include "Socket.hh"
#include "SocketIO.hh"

#include <string>
#include <memory>
#include <functional>

using std::string;
using std::shared_ptr;
using std::function;


namespace ReactorV40{

class TcpConnection;
using TcpConnectionPtr=shared_ptr<TcpConnection>;
using TcpConnectionCallback=function<void(TcpConnectionPtr)>;

class TcpConnection:public std::enable_shared_from_this<TcpConnection>
{
public:
    explicit
    TcpConnection(int fd);
    ~TcpConnection();
    
    void setAllCallbacks(const TcpConnectionCallback & cb1,
                         const TcpConnectionCallback & cb2,
                         const TcpConnectionCallback & cb3)
    {
        _onConnection=cb1;
        _onMessage=cb2;
        _onClose=cb3;
    }

    bool isClosed() const;
    string receive();
    void send(const string & msg);
    string toString() const;

    void handleNewConnectionCallback();
    void handleMessageCallback();
    void handleCloseCallback();

private:
    InetAddress getLocalAddr();
    InetAddress getPeerAddr();
private:
    Socket          _sock;
    SocketIO        _sockIO;
    InetAddress     _localAddr;
    InetAddress     _peerAddr;

    TcpConnectionCallback   _onConnection;
    TcpConnectionCallback   _onMessage;
    TcpConnectionCallback   _onClose;
};

}

#endif
