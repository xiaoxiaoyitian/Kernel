#ifndef  _EchoServer_H__
#define  _EchoServer_H__

#include "TcpConnection.hh"
#include "ThreadPool.hh"
#include "TcpServer.hh"

namespace Titan{

class EchoServer{
public:
    EchoServer(const string &ip, unsigned short port ,int ,int);

    void start();
    void stop();

    void onConnection(TcpConnectionPtr con);
    void onMessage(TcpConnectionPtr con);
    void onClose(TcpConnectionPtr con);

private:
    ThreadPool      _threadpool;
    TcpServer       _tcpServer;
};

}
#endif
