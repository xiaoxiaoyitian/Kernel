#include <arpa/inet.h>
#include <asm-generic/socket.h>
#include <cerrno>
#include <func.h>
#include <iostream>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#define EVENT_ARR_SIZE 100
using std::cout;
using std::endl;

void test(){
    //1.创建服务器的监听套接字
    int  listenfd=socket(AF_INET,SOCK_STREAM,0);
    if(listenfd<0){
        perror("socket");
        return ;
    }

    cout<<"listenfd is "<<listenfd<<endl;

    //1.1设置套接字的属性，网络地址可以重用
    int on=1;
    int ret=setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
    if(ret<0){
        perror("setsockpot");
        close(listenfd);
        return ;
    }

    struct sockaddr_in serveraddr;
    memset(&serveraddr,0,sizeof(serveraddr));
    serveraddr.sin_family=AF_INET;
    serveraddr.sin_port=htons(8000);
    serveraddr.sin_addr.s_addr=inet_addr("0.0.0.0");

    //2.绑定网络
    ret=bind(listenfd,(const struct sockaddr *)&serveraddr,sizeof(serveraddr));
    if(ret<0){
        perror("bind");
        close(listenfd);
        return;
    }

    //3.服务器进行监听
    ret=listen(listenfd,20000);
    cout<<" server is listening."<<endl;

    //4.创建epoll的实例

    int epfd=epoll_create1(0);
    if(epfd<0){
        perror("epoll_create1");
        return ;
    }
    cout<<"epfd is "<<epfd<<endl;

    //5.监听listenfd上的读事件
    struct epoll_event ev;
    memset(&ev,0,sizeof(ev));
    ev.data.fd=listenfd;
    ev.events=EPOLLIN;
    ret=epoll_ctl(epfd,EPOLL_CTL_ADD,listenfd,&ev);
    if(ret<0){
        perror("epoll_ctl");
        return;
    }

    //6.准备用户态中存储已经就绪的文件描述符及其事件
    struct epoll_event evtArr[EVENT_ARR_SIZE]={0};

    while(1){
        int nready=epoll_wait(epfd,evtArr,EVENT_ARR_SIZE,5000);
        if(nready==-1&&errno==EINTR){
            continue;
        }else if(nready==0){
            cout<<"epoll timeout."<<endl;
        }else if(nready==-1){
            perror("epoll_wait");
            return;
        }else{
            //7.对nready个已经就绪的文件描述符进行相应的处理
            for(int i=0;i<nready;++i){
                int fd=evtArr[i].data.fd;
                if(fd==listenfd){
                    //8.对新链接进行处理
                    struct sockaddr_in clientaddr;
                    memset(&clientaddr,0,sizeof(clientaddr));
                    socklen_t len=sizeof(clientaddr);
                    cout<<"before accept."<<endl;

                    //8.0 获取与对端进行交互的netfd
                    int netfd=accept(listenfd,(struct sockaddr *)&clientaddr,&len);
                    if(netfd<0){
                        perror("accept");
                        continue;
                    }
                    //8.1打印客户端的网络地址
                    cout<<"client["<<netfd<<"/"<<inet_ntoa(clientaddr.sin_addr)<<":"<<ntohs(clientaddr.sin_port)<<"] has connected."<<endl;

                    ev.data.fd=netfd;
                    ev.events=EPOLLIN;
                    epoll_ctl(epfd,EPOLL_CTL_ADD,netfd,&ev);
                }else{
                    //9.已经建立好的连接的处理
                    char buf[100]={0};
                    //9.1从内核接收缓冲区中拷贝数据
                    ret=recv(fd,buf,sizeof(buf),0);

                    //当连接断开时，一定会触发读事件
                    if(ret==0){
                        cout<<fd<<"fd has closed."<<endl;
                        
                        //先从epoll的监听红黑树上删除fd
                        ev.data.fd=fd;
                        epoll_ctl(epfd,EPOLL_CTL_DEL,fd,&ev);
                        close(fd);
                        continue;
                    }

                    //9.2打印接收的数据到终端上
                    cout<<"recv "<<ret<<"bytes, msg:"<<buf<<endl;
                    //9.3进行业务逻辑的处理
                    


                    ret=send(fd,buf,strlen(buf),0);
                    cout<<">>send "<<ret<<"bytes."<<endl;
                }
            }
        }
    }
}

int main()
{
    test();
    return 0;
}

