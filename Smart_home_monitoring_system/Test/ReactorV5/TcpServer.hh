#ifndef __TcpServer_H__
#define __TcpServer_H__

#include "EventLoop.hh"
#include "TcpConnection.hh"
#include "Acceptor.hh"

namespace Titan{

class TcpServer{
public:
    TcpServer(const string &ip,unsigned short port)
    :_acceptor(ip,port)
    ,_loop(_acceptor)
    {
        _acceptor.ready();
    }

    void setAllCallbacks(TcpConnectionCallback && cb1,
                         TcpConnectionCallback && cb2,
                         TcpConnectionCallback && cb3)
    {
        _loop.setAllCallbacks(std::move(cb1),
                              std::move(cb2),
                              std::move(cb3));
    }

    void start(){
        _loop.loop();
    }


    void stop(){
        _loop.unloop();
    }

    int fd() const{
        return _acceptor.fd();
    }

private:
    Acceptor    _acceptor;
    EventLoop   _loop;
};

}

#endif
