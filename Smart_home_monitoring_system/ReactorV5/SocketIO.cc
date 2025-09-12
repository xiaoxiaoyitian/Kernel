#include "SocketIO.hh"

namespace Titan{

    int SocketIO::readTLV(TLV &tlv){
        // 先接收type和length（各4字节，共8字节）
        int headerlen=sizeof(tlv.getType()) + sizeof(tlv.getLength());
        char header[headerlen];
        int received = 0;
    
        int ret=readn(header,headerlen);
        
        // 解析length以确定需要接收的数据长度
        uint32_t length_net;
        memcpy(&length_net, header + sizeof(tlv.getType()), sizeof(length_net));
        int data_length = static_cast<int>(ntohl(length_net));
    
        // 检查数据长度是否有效
        if (data_length < 0 || data_length > DATA_BUFFER_SIZE) {
           //数据长度小于0和超过tlv接收数据的数组的长度
            return 8;
        }
    
        // 接收数据部分
        char* buffer = new char[sizeof(header) + data_length];
        //将前面获得的头部数据放入buffer中
        memcpy(buffer, header, sizeof(header));
        ret=readn(buffer+headerlen,data_length);
    
        //解码完整的TLV数据
        tlv.decode(buffer, sizeof(header) + data_length);
        //释放申请的堆空间
        delete[] buffer;
        return tlv.getLength();
    }

    int SocketIO::sendTLV(TLV &tlv){
        //编码
        std::string encoded = tlv.encode();
        const char* data = encoded.c_str();
        int total_length = encoded.length();
        int send_length = 0;
    
        // 确保所有数据都被发送
        int ret=sendn(encoded.data(),encoded.size());
        // while (send_length < total_length) {
        //     int ret = ::send(_fd, data + send_length, total_length - send_length, 0);
        //     if (ret <= 0) {
        //         return -1;
        //     }
        //     send_length += ret;
        // }
    
        //printf("send %d bytes.\n",send_length);
        return tlv.getLength();

    }
//确定读取len个字节的数据
int SocketIO::readn(char * buff, int len)
{
	int left = len;
	char * p = buff;
    //提问：left的值有可能小于0吗？不可能小于0，退出循环时，left等于0
	while(left > 0) {
		int ret = ::recv(_fd, p, left, 0);
		if(ret == -1 && errno == EINTR)
			continue;
		else if(ret == -1) {
			perror("read");
			return len - left;
		}
		else if(ret == 0) {//连接断开
			return len - left;
		} else {
			left -= ret;
			p += ret;
		}
	}
	return len - left;
}

//每一次读取一行数据, 每一行的长度不好界定，是以'\n'作为结尾
//char buff[1024] = {0}; // 下标为1023的值存放'\0'
int SocketIO::readline(char * buff, int maxlen)
{
	int left = maxlen - 1;
	char * p = buff; 
	int ret;
	int total = 0;
	while(left > 0) {
		ret = recvPeek(p, left);		
		//查找 '\n'
		for(int idx = 0; idx != ret; ++idx) {
			if(p[idx] == '\n') {
				int sz = idx + 1;
				readn(p, sz);//从内核缓冲区中移走
				total += sz;
				p += sz;
				*p = '\0';
				return total;
			}
		}
		//如果没有发现 '\n'
		readn(p, ret);
		left -= ret;
		p += ret;
		total += ret;
	}
	*p = '\0';// 最终没有发现'\n'
	return total;
}

int SocketIO::recvPeek(char * buff, int len) const
{
	int ret;
	do {
		ret = ::recv(_fd, buff, len, MSG_PEEK);
	} while(ret == -1 && errno == EINTR);
	return ret;
}

	
int SocketIO::sendn(const char * buff, int len)
{
	int left = len;
	const char * p = buff;
	while(left > 0) {
		int ret = ::send(_fd, p, left, 0);
		if(ret == -1 && errno == EINTR)
			continue;
		else if(ret == -1) {
			perror("write");
			return len - left;
		} else {
			left -= ret;
			p += ret;
		}
	}
	return len - left;
}
}
