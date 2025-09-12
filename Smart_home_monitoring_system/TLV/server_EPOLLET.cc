#include "TLV.h"
#include <header.h>
#include <errno.h>
#include <iostream>
using namespace std;
using namespace Titan;

#define EVENT_ARR_SIZE 100


int main()
{

    //创建socket对象
    int listenfd=socket(AF_INET,SOCK_STREAM,0);
    if(listenfd<0){
        perror("socket");
        return EXIT_FAILURE;
    }

    //设置端口复用
    int on=1;
    int ret=setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
    if(ret<0){
        perror("setsockopt");
        close(listenfd);
        return EXIT_FAILURE;
    }

    struct sockaddr_in sockaddr;
    sockaddr.sin_family=AF_INET;
    sockaddr.sin_addr.s_addr=inet_addr("0.0.0.0");
    sockaddr.sin_port=htons(8080);

    //绑定端口和IP
    ret=bind(listenfd,(const struct sockaddr*)&sockaddr,sizeof(sockaddr));
    if(ret<0){
        perror("bind");
        close(listenfd);
        return EXIT_FAILURE;
    }

    //监听端口
    ret=listen(listenfd,20000);
    if(ret<0){
        perror("listen");
        close(listenfd);
        return EXIT_FAILURE;
    }
    printf("listening.......\n");

    //创建epoll实例
    int epfd=epoll_create1(0);
    if(epfd<0){
        perror("epoll_create1");
        close(listenfd);
        return EXIT_FAILURE;
    }

    printf("epfd is %d\n",epfd);

    //给epoll实例添加监听事件
    struct epoll_event ev;
    memset(&ev,0,sizeof(ev));
    ev.data.fd=listenfd;
    ev.events=EPOLLIN|EPOLLET;

    ret=epoll_ctl(epfd,EPOLL_CTL_ADD,listenfd,&ev);
    if(ret<0){
        perror("epoll_ctl");
        return EXIT_FAILURE;
    }

    //存储已经就绪的文件描述符及其事件
    struct epoll_event evArr[EVENT_ARR_SIZE]={0};
    while(1){
        int readyfd=epoll_wait(epfd,evArr,EVENT_ARR_SIZE,5000);
        if(readyfd==-1&&errno==EINTR){
            continue;
        }
        else if(readyfd==0){
            printf("timeout\n");
        }
        else if(readyfd==-1){
            perror("epoll_wait");
            return EXIT_FAILURE;
        }
        else{
            for(int i=0;i<readyfd;++i){
                int fd=evArr[i].data.fd;
                if(fd==listenfd){
                    //新连接
                    struct sockaddr_in clientaddr;
                    memset(&clientaddr,0,sizeof(clientaddr));
                    printf("\n before accept.\n");
                    socklen_t len=sizeof(clientaddr);
                    int netfd=accept(listenfd,(struct sockaddr*)&clientaddr,&len);
                    if(netfd<0){
                        perror("accept");
                        continue;
                    }

                    //打印连接的客户端消息
                    printf("client[%d/%s:%d] has connected\n",netfd,inet_ntoa(clientaddr.sin_addr),ntohs(clientaddr.sin_port));

                    //监听新连接的读取事件
                    ev.data.fd=netfd;
                    ev.events=EPOLLIN;
                    ret=epoll_ctl(epfd,EPOLL_CTL_ADD,netfd,&ev);
                }else{
                    //已经就绪的连接
                    //构造TLV接收数据
                    Titan::TLV recv_tlv;
                    if(recv_tlv.receive(fd)){
                        cout<<"---------------------------------"<<endl;
                        cout<<"Received TLV:"<<endl;
                        cout<<"Type:"<<recv_tlv.getType()<<endl;
                        cout<<"Length:"<<recv_tlv.getLength()<<endl;
                        cout<<"Data:"<<recv_tlv.getData()<<endl;
                    }

                    if(recv_tlv.getLength()==0){
                        printf("client fd is %d has closed.\n",fd);
                        ev.data.fd=fd;
                        ret=epoll_ctl(epfd,EPOLL_CTL_DEL,fd,&ev);
                        close(fd);
                        continue;
                    }
                    //构造响应TLV
                    string response=string(recv_tlv.getData());
                    Titan::TLV send_tlv(TASK_LOGIN,response.c_str(),response.size()-1);
                    send_tlv.send(fd);
                }
            }
        }
    }
    return 0;
}

