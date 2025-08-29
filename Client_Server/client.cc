#include <cstdio>
#include <func.h>
#include <sys/socket.h>
#include <iostream>
using std::cout;
using std::endl;

void test(){
    //1.创建客户端的套接字
    int clientfd=socket(AF_INET,SOCK_STREAM,0);
    if(clientfd<0){
        perror("socket");
        return;
    }

    struct sockaddr_in serveraddr;
    memset(&serveraddr,0,sizeof(serveraddr));
    serveraddr.sin_family=AF_INET;
    serveraddr.sin_port=htons(8000);

    serveraddr.sin_addr.s_addr=inet_addr("0.0.0.0");

    //2.连接服务器
    int ret=connect(clientfd,(const struct sockaddr *)&serveraddr,sizeof(serveraddr));
    if(ret<0){
        perror("connect");
        close(clientfd);
        return;
    }

    //3.连接成功
    printf("connect success.\n");

    
    //4.发送数据
    const char *hello="hello,server";
    ret=send(clientfd,hello,strlen(hello),0);
    cout<<"send "<<ret<<"bytes."<<endl;
    //5.接收数据
    char buf[1000]={0};
    ret=recv(clientfd,buf,sizeof(buf),0);
    cout<<"recv "<<ret<<"bytes,content:"<<buf<<endl;


    while(1){}
    close(clientfd);

}

int main()
{
    test();
    return 0;
}

