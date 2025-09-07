#ifndef  __TcpServer_H__
#define  __TcpServer_H__

#include "Acceptor.hh"
#include "TcpConnection.hh"
#include "EventLoop.hh"

namespace ReactorV5{


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

private:
    Acceptor    _acceptor;
    EventLoop   _loop;
};

}

#endif
