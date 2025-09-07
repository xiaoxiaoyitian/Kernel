#include "EchoServer.hh"
#include "BussinessService.hh"
#include "TcpConnection.hh"
#include <iostream>
#include <pthread.h>
using std::cout;
using std::endl;

EchoServer::EchoServer(const string &ip,unsigned short port ,int threadNum,int sz)
:_threadpool(threadNum,sz)
,_tcpServer(ip,port)
{
    
    using namespace std::placeholders;
    _tcpServer.setAllCallbacks(std::bind(&EchoServer::onConnection,this,_1),
                               std::bind(&EchoServer::onMessage,this,_1),
                               std::bind(&EchoServer::onClose,this,_1));
}

void    EchoServer::start(){
    _threadpool.start();
    _tcpServer.start();
}

void EchoServer::stop(){
    _tcpServer.stop();
    _threadpool.stop();
}

void EchoServer::onConnection(TcpConnectionPtr con){
    cout<<"io thread "<<pthread_self()
        <<":ReactorV5:"<<con->toString()<<"成功建立连接！"<<endl;
}

void EchoServer::onMessage(TcpConnectionPtr con){
    string msg =con->receive();
    cout<<"io thread "<<pthread_self()
        <<":ReactorV5 收到的数据为 "<<msg<<endl;
    MyTask mytask(msg,con);
    _threadpool.addTask(std::bind(&MyTask::process,mytask));
}

void EchoServer::onClose(TcpConnectionPtr con){
    cout<<"io thread "<<pthread_self()
        <<":ReactorV5:"<<con->toString()<<"已经断开连接！"<<endl;
}
