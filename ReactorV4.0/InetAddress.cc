#include "InetAddress.hh"
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
using std::cout;
using std::endl;


namespace ReactorV40{

InetAddress::InetAddress(const string &ip,unsigned short port){
    memset(&_addr,0,sizeof(_addr));
    _addr.sin_addr.s_addr=inet_addr(ip.c_str());
    _addr.sin_family=AF_INET;
    _addr.sin_port=htons(8000);
}

InetAddress::InetAddress(struct sockaddr_in addr)
:_addr(addr)
{}

string InetAddress::ip() const{
    return string(inet_ntoa(_addr.sin_addr));
}

unsigned short InetAddress::port() const{
    return ntohs(_addr.sin_port);
}




}


