#include "Acceptor.hh"
#include "TcpConnection.hh"
#include <iostream>
using std::cout;
using std::endl;

void test(){
    ReactorV1::Acceptor acceptor("0.0.0.0",8000);
    acceptor.ready();

    cout<<">> server  start listening."<<endl;

    int netfd=acceptor.accept();
    ReactorV1::TcpConnection con(netfd);
    cout<<con.toString()<<"has connected successfully."<<endl;

    con.send("welcome to server.");
    cout<<"receive msg: "<<con.receive()<<endl;

}

int main()
{
    test();
    return 0;
}

