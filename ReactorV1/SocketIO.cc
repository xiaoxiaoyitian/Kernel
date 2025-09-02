#include "SocketIO.hh"
#include <cerrno>
#include <func.h>
#include <iostream>
#include <sys/socket.h>
using std::cout;
using std::endl;


namespace ReactorV1{


SocketIO::SocketIO(int fd)
:_fd(fd)
{

}

SocketIO::~SocketIO(){

}

//查看内核接收缓冲区是否有数据
int SocketIO::readPeek(char *buf,int len) const{
    int ret=-1;
    do{
        ret=recv(_fd,buf,len,MSG_PEEK);
    }while(ret==-1&&errno==EINTR);
    return ret;
}
//确定接收len字节的数据
int SocketIO::readn(char *buf,int len){
    int left=len;
    char *pbuf=buf;
    while(left>0){
        int ret=recv(_fd,pbuf,left,0);
        if(ret==-1&&errno==EINTR){
            continue;
        }else if(ret==-1){
            perror("recv");
            return len-left;
        }else if(ret==0){
            return len-left;
        }else {
            left-=ret;
            pbuf+=ret;
        }
    }
    return len-left;
}

int SocketIO::writen(const char *buf,int len){
    int left=len;
    const char *pbuf=buf;
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
//读一行的数据
int SocketIO::readLine(char *buf,int len){
    char *pbuf=buf;
    //预留缓冲区最后一个位置的值，设为‘\0’
    //防止出现数组访问越界的情况
    int left=len-1;
    int total=0;//用来记录一共读取字节数
    while(left>0){

        //查看内核接收缓冲区的数据，但不移走
        int ret=readPeek(pbuf,left);

        //在ret个字节的数据中查找是否有'\n'
        for(int i=0;i<ret;++i){
            if(pbuf[i]=='\n'){
                //找到了'\n'
                int sz=i+1;
                ret=readn(pbuf,sz);
                total+=ret;
                pbuf[i]='\0';
                return total;
            }
        }
        ret=readn(pbuf,ret);
        left-=ret;
        pbuf+=ret;
        total+=ret;
    }
    buf[len-1]='\0';
    return len-1;
}



}



