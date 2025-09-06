#include <func.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <iostream>
#include <unistd.h>
using std::cout;
using std::endl;

void test(){

    int clientfd=socket(AF_INET,SOCK_STREAM,0);
    if(clientfd<0){
        perror("socket");
        return;
    }

    struct sockaddr_in sockAddr;
    sockAddr.sin_family=AF_INET;
    sockAddr.sin_port=htons(8000);
    sockAddr.sin_addr.s_addr=inet_addr("0.0.0.0");

    int ret=connect(clientfd,(const struct sockaddr *)&sockAddr,sizeof(sockAddr));
    if(ret<0){
        perror("connect");
        close(clientfd);
    }


    fd_set readfds;
    char buf[100]={0};

    while(1){
        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO,&readfds);
        FD_SET(clientfd,&readfds);

        ret=select(clientfd+1,&readfds,NULL,NULL,NULL);
        
        if(FD_ISSET(STDIN_FILENO,&readfds)){
            memset(buf,0,sizeof(buf));
            ret=read(STDIN_FILENO,buf,sizeof(buf));
            cout<<"接收到了 "<<ret<<"个字节的数据！"<<endl;

            ret=send(clientfd,buf,strlen(buf),0);
            cout<<"发送了 "<<ret<<"个字节的数据！"<<endl;

        }
        if(FD_ISSET(clientfd,&readfds)){
            memset(buf,0,sizeof(buf));
            ret=recv(clientfd,buf,sizeof(buf),0);
            if(ret==0){
                cout<<"服务器已经断开连接！！！"<<endl;
                close(clientfd);
                break;
            }
            cout<<"接收到了 "<<ret<<"个字节的数据为："<<buf<<endl;
        }
    }
    close(clientfd);
}

int main()
{
    test();
    return 0;
}

