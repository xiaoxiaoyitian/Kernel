#ifndef  __InetAddress_H__
#define  __InetAddress_H__
#include <netinet/in.h>
#include <string>
#include <func.h>
using std::string;
namespace ReactorV1{

class InetAddress{
public:
    InetAddress(const string &ip,unsigned short port);
    InetAddress(const struct sockaddr_in &addr);
    ~InetAddress();
    string ip();
    unsigned short port();
    
    struct sockaddr_in *getInetAddress(){
        return &_addr;
    }

private:
    struct sockaddr_in _addr;
};

}

#endif
