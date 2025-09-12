#ifndef     __TcpConnection_H__
#define     __TcpConnection_H__

#include "func.h"
#include "InetAddress.hh"
#include "Socket.hh"
#include "SocketIO.hh"


#include <memory>
#include <functional>
#include <string>
#include <iostream>

using std::string;
using std::function;
using std::shared_ptr;
using std::cout;
using std::endl;

namespace Titan{

    class EventLoop;
    class TcpConnection;
    using TcpConnectionPtr=shared_ptr<TcpConnection>;
    using TcpConnectionCallback=function<void(TcpConnectionPtr)>;


    class TcpConnection :public std::enable_shared_from_this<TcpConnection>
    {
        public:
            TcpConnection(int fd,EventLoop *loop);
            ~TcpConnection();

            void setAllCallbacks(const TcpConnectionCallback & cb1,
                                 const TcpConnectionCallback & cb2,
                                 const TcpConnectionCallback & cb3)
            {
                _onConnection=cb1;
                _onMessage=cb2;
                _onClose=cb3;
            }

            bool isClosed() ;

            string receiveData() ;
            int readPacket(Packet &packet);
            void send(const string &msg);
            void sendInLoop( const TLV &tlv);
            void sendInLoop(const string &msg);

            void    handleNewConnectionCallback();
            void    handleMessageCallback();
            void    handleCloseCallback();

            void    printfConnectionInfo();
            void    printfDisconnectionInfo();
        private:
            InetAddress     getLocalAddr();
            InetAddress     getPeerAddr();
        private:
            Socket          _sock;
            SocketIO        _sockIO;
            InetAddress     _localAddr;
            InetAddress     _peerAddr;

            TcpConnectionCallback   _onConnection;
            TcpConnectionCallback   _onMessage;
            TcpConnectionCallback   _onClose;
            EventLoop *             _loop;
    };

}

#endif
