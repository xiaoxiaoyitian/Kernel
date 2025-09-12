#ifndef __BussinessService_H__
#define __BussinessService_H__
#include "TcpConnection.hh"
#include <iostream>

using std::cout;
using std::endl;


class MyTask{
public:
    MyTask(const string &msg,Titan::TcpConnectionPtr con)
    :_msg(msg)
    ,_con(con)
    {}
    void Login(){
        printf("登录逻辑\n");
    }

    void Register(){
        printf("注册逻辑\n");
    }

    void Camera(){
        printf("摄像头逻辑\n");
    }
private:
    string                      _msg;
    Titan::TcpConnectionPtr     _con;
};

#endif
