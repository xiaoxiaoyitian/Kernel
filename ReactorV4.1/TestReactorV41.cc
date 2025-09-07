#include "TcpServer.hh"
#include "TcpConnection.hh"
#include "ThreadPool.hh"
#include <iostream>
#include <pthread.h>
using std::cout;
using std::endl;

//声明全局变量的线程池
ReactorV41::ThreadPool threadpool(4,10);

class MyTask{
public:
    MyTask(const string & msg,ReactorV41::TcpConnectionPtr con)
        :_msg(msg)
         ,_con(con){}

    void process(){
        cout<<"compute thread "<<pthread_self()
            <<",MyTask::process is running"<<endl;

        string response=_msg;
        _con->sendInLoop(response);
    }
private:
    string  _msg;
    ReactorV41::TcpConnectionPtr _con;
};

void onConnection(ReactorV41::TcpConnectionPtr con){
    cout<<"ReactorV4.1:"<<con->toString()<<" 已经成功建立连接。"<<endl;
}

void onMessage(ReactorV41::TcpConnectionPtr con){
    string msg=con->receive();

    cout<<"io thread "<<pthread_self()
        <<",recv msg:"<<msg<<endl;

    MyTask mytask(msg,con);
    threadpool.addTask(std::bind(&MyTask::process,mytask));
}


void onClose(ReactorV41::TcpConnectionPtr con){
    cout<<"ReactorV4.1:"<<con->toString()<<"已经关闭了。"<<endl;
}

void test(){
    threadpool.start();
    ReactorV41::TcpServer server("0.0.0.0",8000);
    cout<<"服务器开始监听端口....."<<endl;

    server.setAllCallbacks(onConnection,onMessage,onClose);
    server.start();
}


int main(){
    test();
    return 0;
}

