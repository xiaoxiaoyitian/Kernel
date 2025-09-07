#include "SocketIO.hh"
#include <cerrno>
#include <iostream>
#include <sys/socket.h>
using std::cout;
using std::endl;


namespace ReactorV40{

int SocketIO::recvPeek(char *buf,int len)const{
    int ret=-1;
    do{
        ret=recv(_fd,buf,len,MSG_PEEK);
    }while(ret==-1&&errno==EINTR);

    return ret;
}


int SocketIO::recvn(char *buf, int len){
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
        int ret=recvPeek(pbuf,left);
        for(int i=0;i<ret;++i){
            if(pbuf[i]=='\n'){
                int sz=i+1;
                ret=recvn(pbuf,sz);
                total+=ret;
                pbuf[i]='\0';
                return total;
            }
        }
        ret=recvn(pbuf,ret);
        left-=ret;
        pbuf+=ret;
        total+=ret;
    }
    buf[len-1]='\0';
    return len-1;
}




int SocketIO::sendn(const char *buf,int len){
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


}


