#ifndef  __Acceptor_H__
#define  __Acceptor_H__
#include "Socket.hh"
#include "InetAddress.hh"


namespace ReactorV1{
class Acceptor{
public:
    Acceptor(const string &ip,unsigned short port);
    ~Acceptor(){}
    void ready();
    int accept();
    int fd() const {
        return _sock.getFd();
    }
private:
    void setReuseAddr(bool on);
    void setReusePort(bool on);
    void bind();
    void listen();
private:
    Socket _sock;
    InetAddress _addr;
};

}
#endif
