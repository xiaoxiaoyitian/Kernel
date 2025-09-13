#ifndef __Message_H__
#define __Message_H__

#include <string>

using std::string;

namespace Titan
{

enum Type
{
    TEST = 0,                       // 0 - 9, 测试自用

    NESTING = 10,                   // 消息类型为 嵌套TLV

    SIGN_PART1 = 100,               // 客户端用，表示发起注册阶段1
    SIGN_PART1_SUCCESS = 101,       // 服务器用，表示注册阶段1成功，前缀选项盐值可以在此时作为msg发送
    SIGN_PART1_FAIL = 102,          // 服务器用，表示注册阶段1失败

    SIGN_PART2 = 110,               // 注册阶段2，同上
    SIGN_PART2_SUCCESS = 111,     
    SIGN_PART2_FAIL = 112,

    LOGIN_PART1 = 120,              // 登录阶段1，同上
    LOGIN_PART1_SUCCESS = 121,
    LOGIN_PART1_FAIL = 122,

    LOGIN_PART2 = 130,              // 登录阶段2，同上
    LOGIN_PART2_SUCCESS = 131,
    LOGIN_PART2_FAIL  =132,
    

    DELETE_PART1 = 140,             // 删除账号阶段1，同上
    DELETE_PART1_SUCCESS = 141,
    DELETE_PART1_FAIL = 142,


    DELETE_PART2 = 150,             // 删除账号阶段2，同上
    DELETE_PART2_SUCCESS = 151,
    DELETE_PART2_FAIL = 152,
/*================================================================================================ */
/*================================================================================================ */
/*================================================================================================ */

    SAVE_CAMERA_MSG = 200,          //存摄像头信息，待定

    VEDIO_TRANSCRIBE = 300,         //录视频，待定

    SAVE_KITCHEN_MSG = 400,         //保存特定信息，待定

    CLOSE = 800                    //双端使用，表示欲关闭连接
};

// struct TLV
// {
//     int type;
//     int length;
//     char data[1024];
// };

// struct Packet
// {
//     int type;
//     int length;
//     string msg;
// };

}//end of namespace wd

#endif
