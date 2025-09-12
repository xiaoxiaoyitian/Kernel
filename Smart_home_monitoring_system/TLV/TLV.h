#ifndef _TLV_H
#define _TLV_H
#include <stdio.h>
#include <header.h>
#include <string>

using std::string;
// 定义数据缓冲区大小
const int DATA_BUFFER_SIZE = 1024;

//自定义TLV的数据类型
enum TaskType{
    TASK_REGISTER=1,
    TASK_LOGIN=2,
    TASK_CAMERA=3
};

namespace Titan{
class TLV {
public:
    // 构造函数
    TLV();
    TLV(int type, const char* data, int length);

    // 析构函数
    ~TLV(){};

    //拷贝构造函数
    TLV(const TLV &);

    //复制运算符函数
    TLV &operator=(const TLV &);
    // 设置TLV数据
    void  setData(const char* data, int length);
    void  setType(int Type){_type=Type;}

    // 获取TLV数据
    int getType() const {return _type;};
    int getLength() const {return _length;};
    const char* getData() const {return _data;};

    // 编码TLV为网络传输格式
    string encode() const;

    // 从网络数据解码TLV
    bool decode(const char* data, int length);

    // 发送TLV数据
    bool send(int sockfd) const;

    // 接收TLV数据
    bool receive(int sockfd);

private:
    int  _type;                  // TLV类型
    int  _length;                // 数据长度
    char _data[DATA_BUFFER_SIZE]; // 数据缓冲区
};
}
#endif //!_TLV_H
