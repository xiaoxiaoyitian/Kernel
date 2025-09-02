#ifndef  __InetAddress_H__
#define  __InetAddress_H__
#include <func.h>
#include <netinet/in.h>
#include <string>

using std::string;

namespace ReactorV1{

class InetAddress{
public:
    InetAddress(unsigned short port,const string &ip="127.0.0.1");
    explicit InetAddress(struct sockaddr_in addr);

    string ip() const;
    unsigned short port() const;
    struct sockaddr_in *getInetAddressPtr(){
        return &_addr;
    }

private:
    struct sockaddr_in _addr;
};
}


#endif
