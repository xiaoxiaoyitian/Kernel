#ifndef __Configuration_H__
#define __Configuration_H__

#include <map>
#include <string>
#include <iostream>
using std::map;
using std::string;
using std::cout;
using std::endl;



namespace Titan{
    class Configuration{
        public:
            //创建单例
            static Configuration *getInstance(){
                if(_ps==nullptr){
                    _ps=new Configuration();
                }
                return _ps;
            }

            //销毁
            static void destory(){
                if(_ps){
                    delete _ps;
                    _ps=nullptr;
                }
            }
            
            //根据key值，找到对应的value值
            string get(string key);

            //将key，value键值对加入到_map中
            void put(string key,string value);
        private:
            //构造函数私有化
            Configuration()
            {
                
            }
            map<string,string>      _map;//用来存储key，value键值对
            static Configuration *  _ps;//
    };
   


    //利用RAii的思想，创建一个类，通过这个类的析构现自动销毁
    class ConfigurationGuard{
        public:
            ConfigurationGuard(){
                Configuration::getInstance();
            }
            ~ConfigurationGuard(){
                Configuration::destory();
            }
    };
}

#endif