#include "EchoServer.hh"
#include "BussinessService.hh"
#include "TcpConnection.hh"
#include "TLV.hh"
#include <iostream>
using std::cout;
using std::endl;

namespace Titan{

EchoServer::EchoServer(const string &ip,unsigned short port,int threadNum,int sz)
:_threadpool(threadNum,sz)
,_tcpServer(ip,port)
{
    using namespace std::placeholders;
    _tcpServer.setAllCallbacks(
        std::bind(&EchoServer::onConnection,this,_1),
        std::bind(&EchoServer::onMessage,this,_1),
        std::bind(&EchoServer::onClose,this,_1));
}


void EchoServer::start(){
    _threadpool.start();
    _tcpServer.start();
}


void EchoServer::stop(){
    _tcpServer.stop();
    _threadpool.stop();
}

void EchoServer::onConnection(TcpConnectionPtr con){
    if(con){
        con->printfConnectionInfo();
    }
}

void EchoServer::onMessage(TcpConnectionPtr con){
    TLV _tlv;
    _tlv.receive(_tcpServer.fd());
    MyTask task(_tlv.getData(),con);
    switch (_tlv.getType())
    {
    case TASK_REGISTER:
      _threadpool.addTask(std::bind(&MyTask::Register,task));
        break;
    case TASK_LOGIN:
      _threadpool.addTask(std::bind(&MyTask::Login,task));
        break;
    case TASK_CAMERA:
      _threadpool.addTask(std::bind(&MyTask::Camera,task));
    default:
        break;
    }
   
}

void EchoServer::onClose(TcpConnectionPtr con){
    if(con){
        con->printfDisconnectionInfo();
    }
}

}