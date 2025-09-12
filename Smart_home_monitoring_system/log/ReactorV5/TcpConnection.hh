#ifndef     __TcpConnection_H__
#define     __TcpConnection_H__

#include "func.h"
#include "InetAddress.hh"
#include "Socket.hh"
#include "SocketIO.hh"


#include <memory>
#include <functional>
#include <string>

using std::string;
using std::function;
using std::shared_ptr;


namespace Titan{

    class EventLoop;
    class TcpConnection;
    using TcpConnectionPtr=shared_ptr<TcpConnection>;
    using TcpConnectionCallback=function<void(TcpConnectionPtr)>;


    class TcpConnection :public std::enable_shared_from_this<TcpConnection>
    {
        public:
            TcpConnection(int fd,TLV &tlv,EventLoop *loop);
            ~TcpConnection();

            void    setAllCallbacks(const TcpConnectionCallback & cb1,
                                    const TcpConnectionCallback & cb2,
                                    const TcpConnectionCallback & cb3)
            {
                _onConnection=cb1;
                _onMessage=cb2;
                _onClose=cb3;
            }

            bool isClosed() ;
            string receive() ;
            
            void send(TLV &tlv);
            void sendInLoop( TLV &tlv);


            //void sendInLoop(const  string &msg);
            string toString() const;

            void    handleNewConnectionCallback();
            void    handleMessageCallback();
            void    handleCloseCallback();

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