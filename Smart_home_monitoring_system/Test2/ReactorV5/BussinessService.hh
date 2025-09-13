#ifndef __BussinessService_H__
#define __BussinessService_H__
#include "TcpConnection.hh"
#include "Message.hh"
#include "TLV.h"
#include "MySQLClient.hh"

#include <iostream>
#include <shadow.h>

using std::cout;
using std::endl;

class MyTask
{
public:
    MyTask(const Titan::TLV &tlv, Titan::TcpConnectionPtr conn)
        : _tlv(tlv), _con(conn)
    {
    }
    void Login()
    {
        cout<<"Login()"<<endl;
        if(_tlv.getType()==Titan::LOGIN_PART1){
            string usrname=_tlv.getData();
            cout<<"usrname:"<<usrname<<endl;
            if (usrname.empty())
            {
                //假设读取到的用户名为空
                cout<<"empty usrname"<<endl;
                Titan::TLV tlv;
                tlv.setType(Titan::LOGIN_PART1_FAIL);
                const char msg[10]={0};
                tlv.setData(msg,0);
                _con->sendInLoop(tlv);
                return;
            }
            //假设用户名合法
            Titan::MySQLClient mysql;
            bool ret=mysql.connect("127.0.0.1",3306,"root","123456","Titan");
            if(!ret){
                cout<<"mysql connect error!"<<endl;
                return;
            }
            string sql="select setting from usr where name='"+usrname+"';";
            vector<vector<string>> res=mysql.readOperationQuery(sql);
            if(res.size()<=1){
                //查询数据库，用户名不存在
                Titan::TLV tlv;
                tlv.setType(Titan::LOGIN_PART1_FAIL);
                const char msg[10]={0};
                tlv.setData(msg,0);
                _con->sendInLoop(tlv);
                return;
            }
            //查询数据库，假设用户名存在
            string setting=res[1][0];
            cout<<"setting:"<<setting<<endl;
            //发送盐值
            Titan::TLV tlv;
            tlv.setType(Titan::LOGIN_PART1_SUCCESS);
            tlv.setData(setting.c_str(),setting.size());
            _con->sendInLoop(tlv);
        }

        if(_tlv.getType()==Titan::LOGIN_PART2){
            string encrypt=_tlv.getData();
            cout<<"encrypt:"<<encrypt<<endl;
            if(encrypt.empty()){
                //假设读取到的密码为空
                cout<<"empty encrypt"<<endl;
                Titan::TLV tlv;
                tlv.setType(Titan::LOGIN_PART2_FAIL);
                const char msg[10]={0};
                tlv.setData(msg,0);
                _con->sendInLoop(tlv);
                return;
            }
            //验证密码
            Titan::MySQLClient mysql;
            bool ret=mysql.connect("127.0.0.1",3306,"root","123456","Titan");
            if(!ret){
                cout<<"mysql connect error!"<<endl;
                return;
            }
            cout<<"encrypt= "<<encrypt<<endl;
            string sql="select encrypt from usr where setting='"+string(encrypt,0,8)+"';";
            vector<vector<string>> res=mysql.readOperationQuery(sql);
            if(res[1][0]!=encrypt){
                //假设密码不匹配
                Titan::TLV tlv;
                tlv.setType(Titan::LOGIN_PART2_FAIL);
                const char msg[10]={0};
                tlv.setData(msg,0);
                _con->sendInLoop(tlv);
                return;
            }
            if(res[1][0]==encrypt){
            //假设密码匹配成功  
            Titan::TLV tlv;
            tlv.setType(Titan::LOGIN_PART2_SUCCESS);
            cout<<"login success!"<<endl;
            const char msg[10]={0};
            tlv.setData(msg,0);
            _con->sendInLoop(tlv);
            }
        }
    }

    void Register()
    {
        if(_tlv.getType()==Titan::SIGN_PART1){
            string usrname=_tlv.getData();
            cout<<"usrname:"<<usrname<<endl;
            if(usrname.empty()){
                //假设读取到的用户名为空
                cout<<"empty usrname"<<endl;
                Titan::TLV tlv;
                tlv.setType(Titan::SIGN_PART1_FAIL);
                const char msg[10]={0};
                tlv.setData(msg,0);
                _con->sendInLoop(tlv);
                return;
            }
            //假设用户名合法
            Titan::MySQLClient mysql;
            bool ret=mysql.connect("127.0.0.1",3306,"root","123456","Titan");
            if(!ret){
                cout<<"mysql connect error!"<<endl;
                return;
            }


            string sql="select setting from usr where name='"+usrname+"'";

            vector<vector<string>> res=mysql.readOperationQuery(sql);
            if(res.size()>1){
                //查询数据库，用户名和盐值已存在
                 //需要判断是否只存了用户名盐值，没有存密码
                //若只存了用户名盐值，没有存密码，则将其删除
                //反之，存在密码，则证明用户名已经存在，无法注册
                string sql="select encrypt from usr where name='"+usrname+"'";
                vector<vector<string>> res=mysql.readOperationQuery(sql);
                if(res.size()>1){
                    Titan::TLV tlv;
                    tlv.setType(Titan::SIGN_PART1_FAIL);
                    const char msg[10]={0};
                    tlv.setData(msg,0);
                    _con->sendInLoop(tlv);
                    return;
                }else{
                    string sql1="delete from usr where name='"+usrname+"'";
                    mysql.writeOperationQuery(sql1);
                }
            }
            //查询数据库，假设用户名不存在
            Titan::TLV tlv;
            tlv.setType(Titan::SIGN_PART1_SUCCESS);
            //生成盐值并发送给客户端
            char *salt=GenSalt(8);
            cout<<"salt:"<<salt<<endl;
            tlv.setData(salt,8);
            _con->sendInLoop(tlv);

            string sql1="insert into usr(name,setting) values('"+usrname+"','"+string(salt)+"')";
            mysql.writeOperationQuery(sql1);
        }

        if(_tlv.getType()==Titan::SIGN_PART2){
            string encrypt=_tlv.getData();
            cout<<"encrypt:"<<encrypt<<endl;
            if(encrypt.empty()){
                //假设读取到的密码为空
                cout<<"empty passwd"<<endl;
                Titan::TLV tlv;
                tlv.setType(Titan::SIGN_PART2_FAIL);
                const char msg[10]={0};
                tlv.setData(msg,0);
                _con->sendInLoop(tlv);
                return;
            }
            //假设密码合法，写入数据库
            Titan::MySQLClient mysql;
            bool ret=mysql.connect("127.0.0.1",3306,"root","123456","Titan");
            if(!ret){
                cout<<"mysql connect error!"<<endl;
                return;
            }
            //让客户端在密文之前加上salt作为前8位来判断数据库中插入的位置
            string sql="update usr set encrypt='"+encrypt+"' where setting='"+string(encrypt,0,8)+"'";
            mysql.writeOperationQuery(sql);
            //假设写入成功
            Titan::TLV tlv;
            tlv.setType(Titan::SIGN_PART2_SUCCESS);
            const char msg[10]={0};
            tlv.setData(msg,0);
            _con->sendInLoop(tlv);
        }
    }

    void Delete()
    {
        if(_tlv.getType()==Titan::DELETE_PART1){
            string usrname=_tlv.getData();
            cout<<"usrname:"<<usrname<<endl;
            if(usrname.empty()){
                //假设读取到的用户名为空
                cout<<"empty usrname"<<endl;
                Titan::TLV tlv;
                tlv.setType(Titan::DELETE_PART1_FAIL);
                const char msg[10]={0};
                tlv.setData(msg,0);
                _con->sendInLoop(tlv);
                return;
            }
            //假设用户名合法
            Titan::MySQLClient mysql;
            bool ret=mysql.connect("127.0.0.1",3306,"root","123456","Titan");
                
            if(!ret){
                cout<<"mysql connect error!"<<endl;
                return;
            }
            cout<<"1111111"<<endl;

            string sql="select setting from usr where name='"+usrname+"'";
            cout<<"name sql"<<usrname<<endl;
            vector<vector<string>> res=mysql.readOperationQuery(sql);
            if(res.size()<=1){
                //查询数据库，假设用户名不存在
                cout<<"usrname not exist"<<endl;
                Titan::TLV tlv;
                tlv.setType(Titan::DELETE_PART1_FAIL);
                const char msg[10]={0};
                tlv.setData(msg,0);
                _con->sendInLoop(tlv);
                return;
            }
            if(res.size()>1){
                //查询数据库，用户名和盐值已存在
                Titan::TLV tlv;
                tlv.setType(Titan::DELETE_PART1_SUCCESS);
                //发送盐值
                string setting=res[1][0];
                cout<<"setting:"<<setting<<endl;
                tlv.setData(setting.c_str(),setting.size());
                _con->sendInLoop(tlv);
                return;
            }
        }

        if(_tlv.getType()==Titan::DELETE_PART2){
            string encrypt=_tlv.getData();
            cout<<"encrypt:"<<encrypt<<endl;
            if(encrypt.empty()){
                //假设读取到的密码为空
                cout<<"empty encrypt"<<endl;
                Titan::TLV tlv;
                tlv.setType(Titan::DELETE_PART2_FAIL);
                const char msg[10]={0};
                tlv.setData(msg,0);
                _con->sendInLoop(tlv);
                return;
            }
            //验证密码
            Titan::MySQLClient mysql;
            bool ret=mysql.connect("127.0.0.1",3306,"root","123456","Titan");
            if(!ret){
                cout<<"mysql connect error!"<<endl;
                return;
            }
            //接收到的密码
            cout<<"encrypt= "<<encrypt<<endl;
            string sql="select encrypt from usr where setting='"+string(encrypt,0,8)+"'";  
            vector<vector<string>> res=mysql.readOperationQuery(sql);
            if(res[1][0]!=encrypt){
                //假设密码不匹配
                Titan::TLV tlv;
                tlv.setType(Titan::DELETE_PART2_FAIL);
                const char msg[10]={0};
                tlv.setData(msg,0);
                _con->sendInLoop(tlv);
                return;
            }
            if(res[1][0]==encrypt){
            //假设密码匹配成功  
            Titan::TLV tlv;
            string sql1="delete from usr where setting='"+string(encrypt,0,8)+"'";
            mysql.writeOperationQuery(sql1);

            tlv.setType(Titan::DELETE_PART2_SUCCESS);
            cout<<"login success!"<<endl;
            const char msg[10]={0};
            tlv.setData(msg,0);
            _con->sendInLoop(tlv);
            }

        }
}



    void Camera()
    {
        printf("摄像头逻辑\n");
    }

    //生成随机盐值
    char *GenSalt(int length){
        int flag,i;
        char *str;
        srand((unsigned)time(NULL));
        if((str=(char *)malloc(length))==NULL){
            return NULL;
        }
        for(int i=0;i<length+1;++i){
            flag=rand()%3;
            switch(flag){
                case 0:
                    str[i]=rand()%26+'A';
                    break;
                case 1:
                    str[i]=rand()%26+'a';
                    break;
                case 2:
                    str[i]=rand()%10+'0';
                    break;
                default:
                    str[i]='x';
                    break;
            }
        }
        str[length]='\0';
        return str;
    }

    //获取盐值setting
    void getSetting(string &str,const char *passwd){
        int i,j;
        for(i=0,j=0;passwd[i]&&j!=4;++i){
            if(passwd[i]=='$'){
                ++j;
            }
        }
        char buf[100]={0};
        strncpy(buf,passwd,i);
        str=buf;
    }

private:
    Titan::TLV _tlv;
    Titan::TcpConnectionPtr _con;
};

#endif
