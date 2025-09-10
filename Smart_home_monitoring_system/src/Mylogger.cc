#include "Mylogger.hh"

namespace Titan{

Mylogger *Mylogger::_pInstance=nullptr;
Category *Mylogger::_root=nullptr;

Mylogger::Mylogger(){
    //创建一个输出到控制台的日志输出源
    //OstreamAppender *ap1=new OstreamAppender("console",&cout);

    //创建一个输出到文件的日志输出源
    FileAppender *f1=new FileAppender("file","logger.log");

    //创建一个格式化器
    PatternLayout *p1=new PatternLayout();

    //为格式化器设置具体的输出格式模式
    p1->setConversionPattern("%d %c [%p] %m %n");

    //将格式化器与输出源绑定
    //ap1->setLayout(p1);

    //将格式化器与输出源绑定
    f1->setLayout(p1);

    //获取根日志记录器的引用。
    Category &root=Category::getRoot();

    //为根记录器添加我们创建的输出源
    //root.setAppender(ap1);

    //为根记录器添加我们创建的输出源
    root.setAppender(f1);


    //设置根记录器的最低日志级别。
    root.setPriority(Priority::DEBUG);
    _root=&root;
}
//静态创建Mylogger类
//单例模式
Mylogger *Mylogger::getInstance(){
    if(_pInstance==nullptr){
        _pInstance=new Mylogger();
    }
    return _pInstance;
}
//释放回收
Mylogger::~Mylogger(){
    if(_pInstance){
        delete _pInstance;
        _pInstance=nullptr;
    }
}
//注册信息
void Mylogger:: warn(const char * file, int line, const char * func, const char * msg){
    string content=file+std::to_string(line)+func+msg;
    _root->warn(content);
}
//错误信息
void Mylogger::error(const char * file, int line, const char * func, const char * msg){
    string content=file+std::to_string(line)+func+msg;
    _root->error(content);
}
//调试信息
void Mylogger::debug(const char * file, int line, const char * func, const char * msg){
    string content=file+std::to_string(line)+func+msg;
  
    _root->debug(content);
}
//通知信息
void Mylogger::info(const char * file, int line, const char * func, const char * msg){
    string content=file+std::to_string(line)+func+msg;
    _root->info(content);
}

}



