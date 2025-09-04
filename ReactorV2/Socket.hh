#ifndef  __Socket_H__
#define  __Socket_H__

#include <func.h>


namespace ReactorV2{

class Socket{
public:
    Socket();
    explicit Socket(int fd);
    ~Socket();
    int getFd(){
        return _fd;
    }

    void shutdownWrite();
private:
    int _fd;
};

}

#endif
