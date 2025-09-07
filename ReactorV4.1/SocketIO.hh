#ifndef  __SocketIO_H__
#define  __SocketIO_H__

#include <func.h>


namespace ReactorV41{

class SocketIO{
public:
    SocketIO(int fd)
    :_fd(fd){}
    
    int recvPeek(char *buf ,int len) const;
    int recvn(char *buf,int len);
    int readLine(char *buf,int len);
    int sendn(const char *buf,int len);
private:
    int _fd;

};

}

#endif
