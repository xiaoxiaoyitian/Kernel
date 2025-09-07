#ifndef __Acceptor_H__
#define __Acceptor_H__

#include "InetAddress.hh"
#include "Socket.hh"
#include <sys/stat.h>


namespace ReactorV41{

class Acceptor{
public:
    Acceptor(const string &ip,unsigned short port);
    void ready();
    int accept();

    int fd()const{
        return _listensock.fd();
    }

private:
    void setReuseAddr(bool on);
    void setReusePort(bool on);
    void bind();
    void listen();
private:
    InetAddress     _addr;
    Socket          _listensock;
};
}
#endif
