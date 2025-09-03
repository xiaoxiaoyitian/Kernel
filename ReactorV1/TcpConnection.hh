#ifndef  __TcpConnection_H__
#define  __TcpConnection_H__

#include "Socket.hh"
#include "SocketIO.hh"
#include "InetAddress.hh"
#include <func.h>

namespace ReactorV1{

class TcpConnection{
public:
    TcpConnection(int fd);
    ~TcpConnection();
    bool isClosed();
    string toString();
    void send(const string &msg);
    string receive();

private:
    InetAddress getLocalAddr();
    InetAddress getPeerAddr();
private:
    Socket _sock;
    SocketIO _sockio;
    InetAddress _localAddr;
    InetAddress _peerAddr;
};

}
#endif
