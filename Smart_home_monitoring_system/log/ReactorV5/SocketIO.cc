#include "SocketIO.hh"

namespace Titan{

    int SocketIO::readTLV(TLV &tlv){

        if (_fd < 0) {
            return -1;
        }
    
        // 先接收type和length（各4字节，共8字节）
        char header[sizeof(tlv.getType()) + sizeof(tlv.encode())];
        int received = 0;
    
        while (received < static_cast<int>(sizeof(header))) {
            int ret = ::recv(_fd, header + received, sizeof(header) - received, 0);
            if (ret <= 0) {
                return -1;
            }
            received += ret;
        }
    
        // 解析length以确定需要接收的数据长度
        uint32_t length_net;
        memcpy(&length_net, header + sizeof(tlv.getType()), sizeof(length_net));
        int data_length = static_cast<int>(ntohl(length_net));
    
        // 检查数据长度是否有效
        if (data_length < 0 || data_length > DATA_BUFFER_SIZE) {
            return -1;
        }
    
        // 接收数据部分
        char* buffer = new char[sizeof(header) + data_length];
        //将前面获得的头部数据放入buffer中
        memcpy(buffer, header, sizeof(header));
        received = 0;
    
        while (received < data_length) {
            int ret = ::recv(_fd, buffer + sizeof(header) + received, 
                        data_length - received, 0);
            if (ret <= 0) {
                delete[] buffer;
                return -1;
            }
            received += ret;
        }
    
        // 解码完整的TLV数据
        //bool result=tlv.decode(buffer, sizeof(header) + data_length);
        //printf("recv %d bytes.\n",strlen(buffer));
        
        //delete[] buffer;

        return tlv.getLength();
        }

    int SocketIO::sendTLV(TLV &tlv){
        if (_fd < 0) {
            return -1;
        }
        //编码
        std::string encoded = tlv.encode();
        const char* data = encoded.c_str();
        int total_length = encoded.length();
        int send_length = 0;
    
        // 确保所有数据都被发送
        while (send_length < total_length) {
            int ret = ::send(_fd, data + send_length, total_length - send_length, 0);
            if (ret <= 0) {
                return -1;
            }
            send_length += ret;
        }
    
        //printf("send %d bytes.\n",send_length);
        return tlv.getLength();

    }
}