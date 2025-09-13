#include <iostream>
#include "EchoServer.hh"
using namespace std;
void test(){
    Titan::EchoServer server("127.0.0.1",8000,4,10);
    cout<<"server start listening"<<endl;
    server.start();
}
int main()
{
 test();
 return 0;
}

