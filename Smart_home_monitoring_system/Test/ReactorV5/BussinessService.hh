#ifndef __BussinessService_H__
#define __BussinessService_H__
#include "TcpConnection.hh"
#include <iostream>
#include "Message.hh"
using std::cout;
using std::endl;


class MyTask{
public:
    MyTask(const Titan::Packet &packet,Titan::TcpConnectionPtr con)
    :_packet(packet)
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
    Titan::Packet               _packet;
    Titan::TcpConnectionPtr     _con;
};

#endif
