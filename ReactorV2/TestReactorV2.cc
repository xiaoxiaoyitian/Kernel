#include "Acceptor.hh"
#include "EventLoop.hh"
#include "TcpConnection.hh"

#include <iostream>
using std::cout;
using std::endl;

void onConnection(ReactorV2::TcpConnectionPtr con){
    cout<<con->toString()<<"已经成功建立连接！"<<endl;
}

void onMessage(ReactorV2::TcpConnectionPtr con){
    string msg=con->receive();
    cout<<"接收到的数据为："<<msg<<endl;
    //回显消息
    con->send(msg);
}

void onClose(ReactorV2::TcpConnectionPtr con){
    cout<<con->toString()<<"连接已经断开！"<<endl;
}

void test(){
    ReactorV2::Acceptor acceptor("0.0.0.0",8000);
    acceptor.ready();
    cout<<"服务器正在监听端口...."<<endl;

    ReactorV2::EventLoop loop(acceptor);
    loop.setAllCallbacks(onConnection,onMessage,onClose);

    loop.loop();
}

int main()
{
    test();
    return 0;
}

