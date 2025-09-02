#ifndef  __Socket_H__
#define  __Socket_H__

namespace ReactorV1{

class Socket{
public:
    Socket();
    explicit Socket(int fd);
    ~Socket();
    void shutdownWrite();
    int getFd() const;
private:
    int _fd;
};
}
#endif

