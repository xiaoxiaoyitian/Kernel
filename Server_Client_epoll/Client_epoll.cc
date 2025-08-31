#include <arpa/inet.h>
#include <cstdio>
#include <cstring>
#include <func.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
using std::cout;
using std::endl;

void test(){
    
    //1.创建客户端套接字
    int clientfd =socket(AF_INET,SOCK_STREAM,0);
    if(clientfd<0){
        perror("socket");
        return ;
    }
    printf("clientfd :%d\n",clientfd);


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
        return ;
    }

    //3.连接成功
    cout<<"connect success."<<endl;

    //4.使用select进行监听：标准输入，clientfd

    fd_set readfds;


    char buf[100]={0};

    while(1){
        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO,&readfds);
        FD_SET(clientfd,&readfds);

        int ret=select(clientfd+1,&readfds,NULL,NULL,NULL);
        printf(">>%d fd ready.\n",ret);

        //判断标准输入和clientfd上是否有读事件发生
        
        if(FD_ISSET(STDIN_FILENO,&readfds)){
            memset(buf,0,sizeof(buf));

            ret=read(STDIN_FILENO,buf,sizeof(buf));

            cout<<"read "<<ret<<"bytes from stdin."<<endl;

            //发送给服务器
            ret=send(clientfd,buf,strlen(buf),0);
            cout<<"send "<<ret<<"bytes."<<endl;
        }

        if(FD_ISSET(clientfd,&readfds)){
            memset(buf,0,sizeof(buf));

            //服务器发送数据过来
            ret=recv(clientfd,buf,sizeof(buf),0);

            cout<<"recv "<<ret<<"bytes,msg:"<<buf<<endl;
        }
    }
    close(clientfd);


}

int main()
{
    test();
    return 0;
}

