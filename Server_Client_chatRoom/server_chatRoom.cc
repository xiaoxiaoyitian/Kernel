#include <arpa/inet.h>
#include <asm-generic/socket.h>
#include <cerrno>
#include <func.h>
#include <map>
#include <netinet/in.h>
#include <set>
#include <iostream>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>
using std::cout;
using std::endl;
using std::map;
using std::set;


#define MAX_ADDR_SIZE 100


void test(){
    int listenfd=socket(AF_INET,SOCK_STREAM,0);
    if(listenfd<0){
        perror("socket");
        return;
    }

    cout<<">>listenfd is "<<listenfd<<endl;
    int on=1;
    int ret=setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
    if(ret<0){
        perror("setsockopt");
        close(listenfd);
        return;
    }

    struct sockaddr_in serveraddr;
    serveraddr.sin_family=AF_INET;
    serveraddr.sin_port=htons(8000);
    serveraddr.sin_addr.s_addr=inet_addr("0.0.0.0");
    
    ret=bind(listenfd,(const struct sockaddr*)&serveraddr,sizeof(serveraddr));
    if(ret<0){
        perror("bind");
        close(listenfd);
        return;
    }


    ret=listen(listenfd,20000);
    cout<<"server is listening."<<endl;

    int epfd=epoll_create1(0);
    if(epfd<0){
        perror("epoll_create1");
        return;
    }
    cout<<"epfd is "<<epfd<<endl;


    struct epoll_event ev;
    ev.data.fd=listenfd;
    ev.events=EPOLLIN;

    ret=epoll_ctl(epfd,EPOLL_CTL_ADD,listenfd,&ev);
    if(ret<0){
        perror("epoll_ctl");
        return;
    }

    struct epoll_event evtArr[MAX_ADDR_SIZE]={0};
    map<int,int> conns;

    while(1){
        cout<<"epoll do wait operation."<<endl;
        int nready=epoll_wait(epfd,evtArr,MAX_ADDR_SIZE,1000);

        cout<<"nready is "<<nready<<endl;

        if(nready==-1&&errno == EINTR){
            continue;

        }else if(nready==0){
            cout<<"epoll timeout."<<endl;

        }else if(nready==-1){
            perror("epoll_wait");
            return;
        }else{

            for(int i=0;i<nready;++i){
                int fd=evtArr[i].data.fd;

                if(fd==listenfd){
                    struct sockaddr_in clientaddr;
                    socklen_t len=sizeof(clientaddr);
                    memset(&clientaddr,0,sizeof(clientaddr));
                    cout<<"before accept"<<endl;

                    int netfd=accept(fd,(struct sockaddr*)&clientaddr,&len);
                    if(netfd<0){
                        perror("accept");
                        continue;
                    }

                    cout<<"client ["<<netfd<<"/"<<inet_ntoa(clientaddr.sin_addr)<<":"<<ntohs(clientaddr.sin_port)<<"] has connected."<<endl;

                    ev.data.fd=netfd;
                    ev.events=EPOLLIN;
                    epoll_ctl(epfd,EPOLL_CTL_ADD,netfd,evtArr);

                    conns[netfd]=time(nullptr);
                }else{
                    char buf[100]={0};

                    cout<<">> fd read event trigger"<<endl;
                    ret=recv(fd,buf,sizeof(buf),0);
                    if(ret==0){
                        cout<<fd<<" fd has closed."<<endl;
                        ev.data.fd=fd;
                        epoll_ctl(epfd,EPOLL_CTL_DEL,fd,&ev);
                        conns.erase(fd);
                        close(fd);
                        continue;
                    }

                    conns[fd]=time(nullptr);

                    cout<<"recv "<<ret <<"bytes,msg:"<<buf<<endl;

                    for(auto &elem:conns){
                        if(elem.first!=fd){
                            ret=send(elem.first,buf,sizeof(buf),0);
                        }
                    }
                    cout<<"send "<<ret<<"bytes."<<endl;

                }
            }
        }
        cout<<"start kill value"<<endl;
        set<int> deleteFds;
        int curTime=time(nullptr);
        for(auto & elem:conns){
            if(curTime-elem.second>=30){
                close(elem.first);
                cout<<"conn"<<elem.first<<"has timeout,closed."<<endl;
                deleteFds.insert(elem.first);
            }
        }
        for(auto &fd:deleteFds){
            conns.erase(fd);
        }
    }
}

int main()
{
    test();
    return 0;
}

