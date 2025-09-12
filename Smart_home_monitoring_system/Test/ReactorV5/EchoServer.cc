#include "EchoServer.hh"
#include "BussinessService.hh"
#include "TcpConnection.hh"
#include <iostream>
#include "Message.hh"
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
    cout << "onMessage..." << endl;
    Titan::Packet packet;//解析TLV格式之后的消息放在packet中
    int ret = con->readPacket(packet);
    cout << "read:" << ret << "bytes.\n";
    cout << "packet.type: " << packet.type << endl
         << "packet.length:" << packet.length << endl
         << "pakcet.msg:" << packet.msg << endl;

    //handleTask((wd::TaskType)packet.type);
    switch(packet.type)
    {
    case Titan::TASK_TYPE_LOGIN_SECTION1:
        {
            // UserLoginSection1 userLogin1(con, packet);
            MyTask mytask(packet,con);
            _threadpool.addTask(std::bind(&MyTask::Login,mytask));
        }
        break;
    case Titan::TASK_TYPE_LOGIN_SECTION2:
        {}
        break;
    }
}

void EchoServer::onClose(TcpConnectionPtr con){
    if(con){
        con->printfDisconnectionInfo();
    }
}

}