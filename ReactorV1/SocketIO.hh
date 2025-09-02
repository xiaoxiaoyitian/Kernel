#ifndef  __SocketIO_H__
#define __SocketIO_H__

namespace ReactorV1{

class SocketIO{
public:
    SocketIO(int fd);
    ~SocketIO();
    int readPeek(char *buf,int len) const;
    int readn(char *buf,int len);
    int writen(const char *buf,int len);
    int readLine(char *buf,int len);
private:
    int _fd;
};

}
#endif
