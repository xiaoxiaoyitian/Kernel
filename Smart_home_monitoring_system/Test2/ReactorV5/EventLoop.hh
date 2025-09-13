#ifndef  __EventLoop_H__
#define  __EventLoop_H__

#include "TcpConnection.hh"
#include "MutexLock.hh"
#include <sys/eventfd.h>
#include <sys/epoll.h>

#include "func.h"
#include <functional>
#include <vector>
#include <map>


using std::map;
using std::vector;
using std::function;

namespace Titan{
    using Functor=function<void ()>;
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
        void runInLoop(Functor &&f);
    private:
        int createEpollfd();
        void addEpollReadEvent(int);
        void delEpollReadEvent(int);

        void waitEpollFd();
        void handleNewConnection();
        void handleMessage(int);

        int createEventFd();
        void handleReadEvent();
        
        void wakeup();
        void doPendingFunctors();

    private:
        int                         _epfd;
        int                         _eventfd;
        Acceptor &                  _acceptor;
        bool                        _isLooping;
        vector<struct epoll_event>  _evtArr;

        map<int,TcpConnectionPtr>   _cons;

        TcpConnectionCallback       _onConnection;
        TcpConnectionCallback       _onMessage;
        TcpConnectionCallback       _onClose;

        vector<Functor>             _pendingFunctors;
        MutexLock                   _mutex;
    };
}

#endif
