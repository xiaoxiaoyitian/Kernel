#include <header.h>
#include <iostream>
#include "Message.hh"
using namespace std;
int main()
{
    //创建socket对象
    int clientfd=socket(AF_INET,SOCK_STREAM,0);
    if(clientfd<0){
        perror("clientfd error");
        return EXIT_FAILURE;
    }

    struct sockaddr_in sockaddr;
    sockaddr.sin_family=AF_INET;
    sockaddr.sin_addr.s_addr=inet_addr("0.0.0.0");
    sockaddr.sin_port=htons(8080);
    socklen_t len=sizeof(sockaddr);
    //连接服务器
    int ret=connect(clientfd,(const struct sockaddr*)&sockaddr,len);

    if(ret<0){
        perror("connect error");
        close(clientfd);
        return EXIT_FAILURE;
    }

    printf("connect success\n");
    while(1)
    {
        cout << ">> input:";
        string line;
        cin >> line;
        int id = 1;
        int len = line.length();
        //测试: 发用户名
        send(clientfd, &id, 4, 0);
        send(clientfd, &len, 4, 0);
        send(clientfd, line.c_str(), line.size(), 0);
        cout << "send " << 8 + len << " bytes." << endl;

        Titan::TLV tlv = {0};
        recv(clientfd, &tlv.type, 4, 0);
        recv(clientfd, &tlv.length, 4, 0);
        recv(clientfd, tlv.data, tlv.length, 0);
        cout << ">> recv from server:\n" << tlv.data << ", len:" << tlv.length <<endl;
    }
    close(clientfd);
    return 0;
}

