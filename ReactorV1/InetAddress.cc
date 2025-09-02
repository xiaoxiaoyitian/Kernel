#include "InetAddress.hh"
#include <arpa/inet.h>
#include <iostream>
#include <netinet/in.h>
using std::cout;
using std::endl;


namespace  ReactorV1{

InetAddress::InetAddress(unsigned short port,const string &ip){
    memset(&_addr,0,sizeof(_addr));
    _addr.sin_family=AF_INET;
    _addr.sin_port=htons(port);
    _addr.sin_addr.s_addr=inet_addr(ip.c_str());
}

InetAddress::InetAddress(struct sockaddr_in addr)
:_addr(addr)
{
}

string InetAddress::ip() const{
    return string(inet_ntoa(_addr.sin_addr));
}

unsigned short InetAddress::port() const{
    return ntohs(_addr.sin_port);
}



}
