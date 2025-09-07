#ifndef  __EventLoop_H__
#define  __EventLoop_H__

#include "TcpConnection.hh"
#include <algorithm>
#include <sys/epoll.h>
#include <vector>
#include <map>

using std::vector;
using std::map;

namespace ReactorV40{


class Acceptor;
class EventLoop{
public:
    EventLoop(Acceptor &);
    ~EventLoop();

    void setAllCallbacks(TcpConnectionCallback && cb1,
                         TcpConnectionCallback && cb2,
                         TcpConnectionCallback && cb3)
    {
        _onConnection=std::move(cb1);
        _onMessage=std::move(cb2);
        _onClose=std::move(cb3);

    }

    void loop();
    void unloop(){
        _isLooping=false;
    }

private:
    int createEpollFd();
    void addEpollReadEvent(int);
    void delEpollReadEvent(int);

    void waitEpollFd();
    void handleNewConnection();
    void handleMessage(int);

private:
    int                         _epfd;
    Acceptor &                  _acceptor;
    bool                        _isLooping;
    vector<struct epoll_event>  _evtArr;
    map<int,TcpConnectionPtr>   _cons;

    TcpConnectionCallback       _onConnection;
    TcpConnectionCallback       _onMessage;
    TcpConnectionCallback       _onClose;
};

}
#endif
