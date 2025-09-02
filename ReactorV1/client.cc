#include <func.h>
#include <iostream>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>
using std::cout;
using std::endl;

void test(){
    int clientfd=socket(AF_INET,SOCK_STREAM,0);
    if(clientfd<0){
        perror("socket");
        return;
    }

    struct sockaddr_in serveraddr;
    memset(&serveraddr,0,sizeof(serveraddr));
    serveraddr.sin_addr.s_addr=inet_addr("0.0.0.0");
    serveraddr.sin_family=AF_INET;
    serveraddr.sin_port=htons(8000);
    

    int ret=connect(clientfd,(const struct sockaddr*)&serveraddr,sizeof(sockaddr));
    if(ret<0){
        perror("connect");
        close(clientfd);
        return;
    }

    fd_set readfds;

    char buf[100]={0};

    //事件循环
    while(1){
        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO,&readfds);
        FD_SET(clientfd,&readfds);
        
        ret=select(clientfd+1,&readfds,NULL,NULL,NULL);
        if(ret<0){
            perror("select");
            return;
        }

        if(FD_ISSET(STDIN_FILENO,&readfds)){
            memset(buf,0,sizeof(buf));
            ret=read(STDIN_FILENO,buf,sizeof(buf));
            cout<<"read "<<ret<<" bytes from stdin."<<endl;
            ret=send(clientfd,buf,sizeof(buf),0);
            cout<<"send "<<ret<<" bytes."<<endl;
        }

        if(FD_ISSET(clientfd,&readfds)){
            memset(buf,0,sizeof(buf));
            
            ret=recv(clientfd,buf,sizeof(buf),0);
            if(ret==0){
                cout<<"server closed."<<endl;
                close(clientfd);
                break;
            }
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

