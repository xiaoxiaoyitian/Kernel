#include "Configuration.hh"


namespace Titan{
Configuration* Configuration::_ps=nullptr;
    void Configuration::put(string key,string value){
        auto it=_map.find(key);
        //判断key值是否存在于_map中
        if(it!=_map.end()){
            //执行更新操作
            it->second=value;
        }else{
            //执行插入操作
            _map.insert(std::make_pair(key,value));
        }
    }

    string Configuration::get(string key){
        auto it=_map.find(key);
                if(it!=_map.end()){
                    return it->second;
                }else{
                    cout<<"不存在！"<<endl;
                    static string s;
                    return s;
                }
    }


}