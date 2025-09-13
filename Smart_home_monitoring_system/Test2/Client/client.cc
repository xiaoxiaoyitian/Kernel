#include "Message.hh"
#include "TLV.h"
#include <iostream>
using namespace std;
using namespace Titan;

int main()
{

    //创建socket对象
    int clientfd=socket(AF_INET,SOCK_STREAM,0);
    if(clientfd<0){
        perror("clientfd error");
        return EXIT_FAILURE;
    }

    struct sockaddr_in sockaddr;
    sockaddr.sin_family=AF_INET;
    sockaddr.sin_addr.s_addr=inet_addr("127.0.0.1");
    sockaddr.sin_port=htons(8000);
    socklen_t len=sizeof(sockaddr);

    //连接服务器
    int ret=connect(clientfd,(const struct sockaddr*)&sockaddr,len);

    if(ret<0){
        perror("connect error");
        close(clientfd);
        return EXIT_FAILURE;
    }

    printf("connect success\n");
    
    printf("输入l登录,r注册,d删除\n");
    char tip;
    cin>>tip;
    int flag=0;
    if(tip=='l'){
        cout<<"请输入用户名:"<<endl;
        flag=1;
    }
    if(tip=='r'){
        cout<<"请输入你要注册的用户名:"<<endl;
        flag=2;
    }
    if(tip=='d'){
        cout<<"请输入你要删除的用户名:"<<endl;
        flag=3;
    }
    fd_set set;
    while(1){
        FD_ZERO(&set);
        FD_SET(clientfd,&set);
        FD_SET(STDIN_FILENO,&set);

        int ret=select(clientfd+1,&set,NULL,NULL,NULL);
        //判断标准输入流是否就绪
        if(FD_ISSET(STDIN_FILENO,&set)){
            char buff[1024]={0};
            int ret=read(STDIN_FILENO,buff,sizeof(buff)-1);
            if(ret<0){
                perror("read");
            }
            //构建发送的TLV
            TLV send_tlv;
            if(flag==1){
                //登录
                send_tlv.setType(Titan::LOGIN_PART1);
            }
            if(flag==2){
                //注册
                send_tlv.setType(Titan::SIGN_PART1);
            }
            if(flag==3){
                //删除
                send_tlv.setType(Titan::DELETE_PART1);
            }
            send_tlv.setData(buff,strlen(buff)-1);
            if(send_tlv.send(clientfd)){
                cout<<endl;
                cout<<"Type:"<<send_tlv.getType()<<endl<<"send:"<<buff<<endl<<"length"<<strlen(buff)<<endl;
                cout<<endl;
            }
        }

        //判断服务器是否有数据到达
        if(FD_ISSET(clientfd,&set)){

            TLV recv_tlv;
            
            if(recv_tlv.receive(clientfd)){
                cout<<"Receive from Server:"<<endl;
                cout<<"Type:"<<recv_tlv.getType()<<endl;
                cout<<"Length:"<<recv_tlv.getLength()<<endl;
                cout<<"Data:"<<recv_tlv.getData()<<endl;
            }
            //登录阶段1成功
            if(recv_tlv.getType()==Titan::LOGIN_PART1_SUCCESS){
                cout<<"请输入密码:"<<endl;
                char buff[1024]={0};
                int ret=read(STDIN_FILENO,buff,sizeof(buff)-1);
                if(ret<0){
                    perror("read");
                }
                cout<<"passwd: "<<buff<<endl;
                //获取服务器发送的盐值
                string data=recv_tlv.getData();
                string setting="$y$j9T$";
                setting+=data;
                cout<<"加上前缀后的盐值:"<<setting<<endl;
                char * cryptpassword=crypt(buff,setting.data());

                //将盐值和密文拼接发送给服务器
                string msg=data+string(cryptpassword);
                //构建发送的TLV
                TLV send_tlv;
                send_tlv.setType(Titan::LOGIN_PART2);
                send_tlv.setData(msg.data(),msg.size());
                if(send_tlv.send(clientfd)){
                    cout<<"Type:"<<send_tlv.getType()<<endl<<"Data:"<<msg<<endl<<"length"<<msg.size()<<endl;
                }
            }
            //登录阶段1失败
            if(recv_tlv.getType()==Titan::LOGIN_PART1_FAIL){
                cout<<"用户名不存在"<<endl;
                cout<<"请重新输入用户名:"<<endl;

                char buff[1024]={0};
                int ret=read(STDIN_FILENO,buff,sizeof(buff)-1);
                if(ret<0){
                    perror("read");
                }
                //构建发送的TLV
                TLV send_tlv;
                send_tlv.setType(Titan::LOGIN_PART1);
                send_tlv.setData(buff,strlen(buff));
                if(send_tlv.send(clientfd)){
                    cout<<"Type:"<<send_tlv.getType()<<endl<<"Data:"<<buff<<endl<<"length"<<strlen(buff)<<endl;
                }
            }
            //登录阶段2成功
            if(recv_tlv.getType()==Titan::LOGIN_PART2_SUCCESS){
                cout<<"登录成功，欢迎来到系统"<<endl;
            }
            //登录阶段2失败
            if(recv_tlv.getType()==Titan::LOGIN_PART2_FAIL){
                cout<<"密码错误,请重新输入"<<endl;
                char buff[1024]={0};
                int ret=read(STDIN_FILENO,buff,sizeof(buff)-1);
                if(ret<0){
                    perror("read");
                }
                //构建发送的TLV
                TLV send_tlv;
                send_tlv.setType(Titan::LOGIN_PART2);
                send_tlv.setData(buff,strlen(buff));
                if(send_tlv.send(clientfd)){
                    cout<<"Type:"<<send_tlv.getType()<<endl<<"Data:"<<buff<<endl<<"length"<<strlen(buff)<<endl;
                }
            }

            //注册阶段1成功
            if(recv_tlv.getType()==Titan::SIGN_PART1_SUCCESS){
                cout<<"请输入密码:"<<endl;
                char password[100]={0};
                int ret=read(STDIN_FILENO,password,sizeof(password)-1);
                //获取服务器发送的盐值
                string data=recv_tlv.getData();
                string setting="$y$j9T$";
                setting+=data;
                cout<<"加上前缀后的盐值:"<<setting<<endl;
                char * cryptpassword=crypt(password,setting.data());

                //将盐值和密文拼接发送给服务器
                string msg=data+string(cryptpassword);
                //构建发送的TLV
                TLV send_tlv;
                send_tlv.setType(Titan::SIGN_PART2);
                send_tlv.setData(msg.data(),msg.size());
                if(send_tlv.send(clientfd)){
                    cout<<"Type:"<<send_tlv.getType()<<endl<<"Data:"<<msg<<endl<<"length"<<msg.size()<<endl;
                }
            }
            //注册阶段1失败
            if(recv_tlv.getType()==Titan::SIGN_PART1_FAIL){
                cout<<"用户名已存在,请重新输入"<<endl;
                char buff[1024]={0};
                int ret=read(STDIN_FILENO,buff,sizeof(buff)-1);
                if(ret<0){
                    perror("read");
                }
                //构建发送的TLV
                TLV send_tlv;
                send_tlv.setType(Titan::SIGN_PART1);
                send_tlv.setData(buff,strlen(buff));
                if(send_tlv.send(clientfd)){
                    cout<<"Type:"<<send_tlv.getType()<<endl<<"send:"<<buff<<endl<<"length"<<strlen(buff)<<endl;
                }
            }

            //注册阶段2成功
            if(recv_tlv.getType()==Titan::SIGN_PART2_SUCCESS){
                cout<<"注册成功"<<endl;
            }

            //删除阶段1成功,接收到发送过来的用户名
            if(recv_tlv.getType()==Titan::DELETE_PART1_SUCCESS){
                cout<<"请输入密码:"<<endl;
                char buff[1024]={0};
                int ret=read(STDIN_FILENO,buff,sizeof(buff)-1);
                if(ret<0){
                    perror("read");
                }
                cout<<"passwd: "<<buff<<endl;
                //获取服务器发送的盐值
                string data=recv_tlv.getData();
                string setting="$y$j9T$";
                setting+=data;
                cout<<"加上前缀后的盐值:"<<setting<<endl;
                char * cryptpassword=crypt(buff,setting.data());

                //将盐值和密文拼接发送给服务器
                string msg=data+string(cryptpassword);
                //构建发送的TLV
                TLV send_tlv;
                send_tlv.setType(Titan::DELETE_PART2);
                send_tlv.setData(msg.data(),msg.size());
                if(send_tlv.send(clientfd)){
                    cout<<"Type:"<<send_tlv.getType()<<endl<<"Data:"<<msg<<endl<<"length"<<msg.size()<<endl;
                }
            }

            //删除阶段1失败，收到的用户名出现了问题
            if(recv_tlv.getType()==Titan::DELETE_PART1_FAIL){
                cout<<"用户名不存在"<<endl;
                cout<<"请重新输入用户名:"<<endl;

                //用户不存在，跳到注册执行
                char buff[1024]={0};
                int ret=read(STDIN_FILENO,buff,sizeof(buff)-1);
                if(ret<0){
                    perror("read");
                }
                //构建发送的TLV
                TLV send_tlv;
                send_tlv.setType(Titan::LOGIN_PART1);
                send_tlv.setData(buff,strlen(buff));
                if(send_tlv.send(clientfd)){
                    cout<<"Type:"<<send_tlv.getType()<<endl<<"Data:"<<buff<<endl<<"length"<<strlen(buff)<<endl;
                }
            }
            //删除阶段2成功，成功删除用户
            if(recv_tlv.getType()==Titan::DELETE_PART2_SUCCESS){
                cout<<"成功删除该用户！"<<endl;
            }

            //删除阶段2失败，接收到的密码出现了问题
            if(recv_tlv.getType()==Titan::DELETE_PART2_FAIL){
                cout<<"密码错误,请重新输入"<<endl;
                char buff[1024]={0};
                int ret=read(STDIN_FILENO,buff,sizeof(buff)-1);
                if(ret<0){
                    perror("read");
                }
                //构建发送的TLV
                TLV send_tlv;
                send_tlv.setType(Titan::DELETE_PART2);
                send_tlv.setData(buff,strlen(buff));
                if(send_tlv.send(clientfd)){
                    cout<<"Type:"<<send_tlv.getType()<<endl<<"Data:"<<buff<<endl<<"length"<<strlen(buff)<<endl;
                }
            }


        }
    }
    close(clientfd);
    return 0;  
}
