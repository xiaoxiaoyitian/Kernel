#include "InetAddress.hh"
#include <arpa/inet.h>
#include <iostream>
#include <netinet/in.h>
using std::cout;
using std::endl;

namespace ReactorV2{

InetAddress::InetAddress(const string &ip,unsigned short port)
{
    memset(&_addr,0,sizeof(_addr));
    _addr.sin_addr.s_addr=inet_addr(ip.c_str());
    _addr.sin_port=htons(port);
    _addr.sin_family=AF_INET;
}

InetAddress::InetAddress(const struct sockaddr_in &addr)
:_addr(addr)
{

}
InetAddress::~InetAddress(){

}

string InetAddress::ip(){
    return string(inet_ntoa(_addr.sin_addr));
}

unsigned short InetAddress::port(){
    return ntohs(_addr.sin_port);
}



}
