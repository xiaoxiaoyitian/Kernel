#include "TcpServer.hh"
#include <iostream>
using std::cout;
using std::endl;

void onConnection(ReactorV3::TcpConnectionPtr con){
    cout<<"ReactorV3:"<<con->toString()<<"已经成功建立连接！"<<endl;
}

void onMessage(ReactorV3::TcpConnectionPtr con){
    string msg=con->receive();
    cout<<"接收到的数据为："<<msg<<endl;
    sleep(1);
    //回显消息
    con->send(msg);
}

void onClose(ReactorV3::TcpConnectionPtr con){
    cout<<"ReactorV3:"<<con->toString()<<"连接已经断开！"<<endl;
}

void test(){
    ReactorV3::TcpServer server("0.0.0.0",8000);
    cout<<"服务器正在监听端口...."<<endl;

    server.setAllCallbacks(onConnection,onMessage,onClose);

    server.start();
}

int main()
{
    test();
    return 0;
}

