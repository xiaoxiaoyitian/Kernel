#include "SocketIO.hh"
#include <cerrno>
#include <iostream>
#include <sys/socket.h>
using std::cout;
using std::endl;


namespace ReactorV2{

SocketIO::SocketIO(int fd)
:_fd(fd)
{

}

SocketIO::~SocketIO(){

}

int SocketIO::readPeek(char *buf,int len){
    int ret=-1;
    do{
        ret=recv(_fd,buf,len,MSG_PEEK);
    }while(ret==-1&&errno==EINTR);

    return ret;
}

int SocketIO::readn(char *buf,int len){
    char *pbuf=buf;
    int left=len;
    while(left>0){
        int ret=recv(_fd,pbuf,left,0);
        if(ret==-1&&errno==EINTR){
            continue;
        }else if(ret==-1){
            perror("recv");
            return len-left;
        }else if(ret==0){
            cout<<"connection closed."<<endl;
            return len-left;
        }else{
            left-=ret;
            pbuf+=ret;
        }
    }
    return len-left;
}


int SocketIO::writen(const char *buf,int len){
    const char *pbuf=buf;
    int left=len;

    while(left>0){
        int ret=send(_fd,pbuf,left,0);
        if(ret==-1&&errno==EINTR){
            continue;
        }else if(ret==-1){
            perror("send");
            return len-left;
        }else{
            left-=ret;
            pbuf+=ret;
        }
    }
    return len-left;
}


int SocketIO::readLine(char *buf,int len){
    char *pbuf=buf;
    int left=len-1;
    int total=0;
    while(left>0){
        int ret=readPeek(pbuf,left);
        
        for(int i=0;i<ret;++i){
            if(pbuf[i]=='\n'){
                int sz=i+1;
                ret=readn(pbuf,sz);
                total+=ret;
                pbuf[i]='\0';
                return total;
            }
        }
        ret=readn(pbuf,left);
        left-=ret;
        pbuf+=ret;
        total+=ret;
    }

    buf[len-1]='\0';
    return len-1;
}
}


