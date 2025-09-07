#ifndef __BussinessService_H__
#define __BussinessService_H__

#include "TcpConnection.hh"
#include  <iostream>
#include <pthread.h>

using std::cout;
using std::endl;

class MyTask{
public:
    MyTask(const string &msg,ReactorV5::TcpConnectionPtr con)
    :_msg(msg)
    ,_con(con)
    {}

    void process(){
        cout<<"compute thread "<<pthread_self()
            <<",MyTask::process 正在运行"<<endl;

        string  response=_msg;

        _con->sendInLoop(response);
    }


private:
    string _msg;
    ReactorV5::TcpConnectionPtr _con;
};
#endif
