#include <arpa/inet.h>
#include <asm-generic/socket.h>
#include <cstdio>
#include <netinet/in.h>
#include <sys/socket.h>
#include <func.h>
#include <iostream>
using std::cout;
using std::endl;

void test(){

    //1.创建服务器的监听套接字
    int listenfd=socket(AF_INET,SOCK_STREAM,0);
    if(listenfd<0){
        perror("socket");
        return;
    }

    //设置套接字的属性，网络地址可以重用
    int on=1;
    int ret=setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
    if(ret<0){
        perror("setsockopt");
        close(listenfd);
        return;
    }
    struct sockaddr_in serveraddr;
    memset(&serveraddr,0,sizeof(serveraddr));
    serveraddr.sin_family=AF_INET;
    serveraddr.sin_port=htons(8000);
    serveraddr.sin_addr.s_addr=inet_addr("0.0.0.0");

    //2.绑定网络地址
    ret=bind(listenfd,(const struct sockaddr *)&serveraddr,sizeof(serveraddr));
    if(ret<0){
        perror("bind");
        close(listenfd);
        return;
    }

    //3.服务器进行监听
    ret =listen(listenfd,3);

    while(1){
        struct sockaddr_in clientaddr;
        memset(&clientaddr,0,sizeof(clientaddr));
        socklen_t len=sizeof(clientaddr);
        cout<<endl<<"before accept."<<endl;

        //4.从内核全连接队列上去下一个已经建好的连接
        int netfd=accept(listenfd,(struct sockaddr *)&clientaddr,&len);


        if(netfd<0){
            perror("accept");
            continue;
        }

        //5.打印客户端的网络地址
        cout<<"client["<<inet_ntoa(clientaddr.sin_addr)<<":"
            <<ntohs(clientaddr.sin_port)<<"has connected."<<endl;

        //6.进行数据的接收和发送
        char buf[1000]={0};
        ret=recv(netfd,buf,sizeof(buf),0);
        cout<<"recv "<<ret<<"bytes,content:"<<buf<<endl;

        const char *hello="hello,client";
        ret=send(netfd,hello,strlen(hello),0);
        close(netfd);
    }
}

int main()
{
    test();
    return 0;
}

