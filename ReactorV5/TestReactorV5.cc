#include "EchoServer.hh"
#include <iostream>

using std::cout;
using std::endl;

int main(){
    EchoServer server("0.0.0.0",8000,4,10);
    cout<<"服务器正在监听端口..."<<endl;

    server.start();
    return 0;
}
