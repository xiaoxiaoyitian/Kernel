#include "TLV.h"
#include <header.h>
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
    sockaddr.sin_addr.s_addr=inet_addr("0.0.0.0");
    sockaddr.sin_port=htons(8080);
    socklen_t len=sizeof(sockaddr);
    //连接服务器
    int ret=connect(clientfd,(const struct sockaddr*)&sockaddr,len);

    if(ret<0){
        perror("connect error");
        close(clientfd);
        return EXIT_FAILURE;
    }

    printf("connect success\n");
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
            send_tlv.setType(TASK_LOGIN);
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
            if(recv_tlv.getLength()==0){
                printf("服务器断开，或被踢出群聊\n");
                break;
            }
        }
    }
    close(clientfd);
    return 0;
}

