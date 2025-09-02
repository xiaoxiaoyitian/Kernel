#ifndef  __TcpConnection_H__
#define  __TcpConnection_H__

#include "SocketIO.hh"
#include "Socket.hh"
#include "InetAddress.hh"

namespace ReactorV1{

class TcpConnection{
public:
    
    explicit TcpConnection(int fd);
    ~TcpConnection();
    void send(const string &msg);
    string receive();
    bool isClosed() const;
    string toString() const;

private:
    InetAddress getLocalAddr();
    InetAddress getPeerAddr();
private:
    SocketIO    _sockio;
    Socket      _sock;
    InetAddress _localAddr;
    InetAddress _peerAddr;

};

}
#endif
