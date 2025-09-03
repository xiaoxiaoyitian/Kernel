#include "Acceptor.hh"
#include "TcpConnection.hh"

#include <iostream>
#include <sys/socket.h>
using std::cout;
using std::endl;

void test(){
    ReactorV1::Acceptor acceptor("0.0.0.0",8000);
    acceptor.ready();

    cout<<"服务器开始监听端口8000..."<<endl;

    int netfd=acceptor.accept();
    ReactorV1::TcpConnection tcp(netfd);

    cout<<tcp.toString()<<"已经成功连接！"<<endl;

    tcp.send("小比崽子！");
    cout<<"接收到的数据为："<<tcp.receive()<<endl;
}

int main()
{
    test();
    return 0;
}

