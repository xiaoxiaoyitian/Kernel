#include "TLV.h"
#include <iostream>
using namespace std;
namespace Titan{

TLV::TLV() 
:_type(0), _length(0) {
    memset(_data, 0, DATA_BUFFER_SIZE);
}

TLV::TLV(int type, const char* data, int length)
:_type(type)
,_length(0)
{
    memset(&_data,0,DATA_BUFFER_SIZE);
    setData(data,length);//设置TLV数据
}

TLV::TLV(const TLV &rhs){
    _type=rhs._type;
    _length=rhs._length;
    memcpy(_data,rhs._data,DATA_BUFFER_SIZE);
}

TLV& TLV::operator=(const TLV &rhs){
    if(this!=&rhs){
        _type=rhs._type;
        _length=rhs._length;
        memcpy(_data,rhs._data,DATA_BUFFER_SIZE);
    }
    return *this;
} 

// 设置TLV数据
void TLV::setData(const char* data, int length){
    //判断长度是否合法
    if(length<=0||length>DATA_BUFFER_SIZE){
        cerr<<"setData length error!"<<endl;
        return;
    }
    //将data的数据拷入_data
    memcpy(_data,data,length);
    _length=length;
}
// 编码TLV为网络传输格式
string TLV::encode() const{
    string encoded;

    //编码type(网络字节序)
    uint32_t type_net=htonl(static_cast<uint32_t>(_type));
    encoded.append(reinterpret_cast<const char *>(&type_net),sizeof(type_net));

    //编码length(网络字节序)
    uint32_t length_net=htonl(static_cast<uint32_t>(_length));
    encoded.append(reinterpret_cast<const char *>(&length_net),sizeof(length_net));

    //编码data
    encoded.append(_data,_length);

    return encoded;
}

// 从网络数据解码TLV
bool TLV::decode(const char* data, int length){
    //TLV 单元所需的最小数据长度type+length,如果小于，则后续无法进行
    if(length<static_cast<int>(sizeof(_type)+sizeof(length))){
        cerr<<"decode error:length less than mindatalength"<<endl;
        return false;
    }

    int offset=0;//初始偏移量

   // 解码type
    uint32_t type_net=0;
    cout<<sizeof(type_net)<<endl;

    memcpy(&type_net, data + offset, sizeof(type_net));


    _type = static_cast<int>(ntohl(type_net));

    cout<<"_type:"<<_type<<endl;
    offset += sizeof(type_net);
    
    // 解码length
    uint32_t length_net=0;
    memcpy(&length_net, data + offset, sizeof(length_net));
    _length = static_cast<int>(ntohl(length_net));
    offset += sizeof(length_net);
    
    // 检查数据长度是否匹配
    if (_length < 0 || _length > DATA_BUFFER_SIZE || offset + _length > length) {
        cerr<<"decode error:length nomatch!"<<endl;
        return false;
    }
    
    // 解码data
    memset(_data, 0, DATA_BUFFER_SIZE);
    if (_length > 0) {
        memcpy(_data, data + offset, _length);
    }

    return true;
}

// 发送TLV数据
bool TLV::send(int sockfd) const{
    if (sockfd < 0) {
        cerr<<"send error: bad sockfd!"<<endl;
        return false;
    }
    //编码
    std::string encoded = encode();
    const char* data = encoded.c_str();
    int total_length = encoded.length();
    int send_length = 0;
    
    // 确保所有数据都被发送
    while (send_length < total_length) {
        int ret = ::send(sockfd, data + send_length, total_length - send_length, 0);
        if (ret <= 0) {
           cerr<<"send error!"<<endl;
           return false;
        }
        send_length += ret;
    }
    printf("send %d bytes.\n",send_length);
    return true;
}

// 接收TLV数据
bool TLV::receive(int sockfd){
    if (sockfd < 0) {
        cerr<<"TLV receive error: bad sockfd!"<<endl;
        return false;
    }
    
    // 先接收type和length（各4字节，共8字节）
    char header[sizeof(_type) + sizeof(_length)];
    int received = 0;
    
    while (received < static_cast<int>(sizeof(header))) {
        int ret = ::recv(sockfd, header + received, sizeof(header) - received, 0);
        if (ret <= 0) {
            cerr<<"TLV recv error!"<<endl;
            return false;
        }
        received += ret;
    }
    
    // 解析length以确定需要接收的数据长度
    uint32_t length_net;
    memcpy(&length_net, header + sizeof(_type), sizeof(length_net));
    int data_length = static_cast<int>(ntohl(length_net));
    
    // 检查数据长度是否有效
    if (data_length < 0 || data_length > DATA_BUFFER_SIZE) {
        cerr<<"TLV receive error:length nomatch"<<endl;
        return false;
    }
    
    // 接收数据部分
    char* buffer = new char[sizeof(header) + data_length];
    //将前面获得的头部数据放入buffer中
    memcpy(buffer, header, sizeof(header));
    received = 0;
    
    while (received < data_length) {
        int ret = ::recv(sockfd, buffer + sizeof(header) + received, 
                        data_length - received, 0);
        if (ret <= 0) {
            perror("receive");
            delete[] buffer;
            return false;
        }
        received += ret;
    }
    
    // 解码完整的TLV数据
    bool result=decode(buffer, sizeof(header) + data_length);
    printf("recv %ld bytes.\n",strlen(buffer));
    delete [] buffer;
    return result;
}

};
