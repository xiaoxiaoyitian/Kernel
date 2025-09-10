#ifndef __Mylogger_H__
#define __Mylogger_H__

#include <string>
#include <iostream>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/PatternLayout.hh>
#include <log4cpp/Category.hh>
#include <log4cpp/FileAppender.hh>

using std::string;
using std::cout;
using std::endl;
using namespace log4cpp;


//#define LOG_WARN(msg) Mylogger::getInstance()->warn(__FILE__,__LINE__,__FUNCTION__,msg)
//#define LOG_ERROR(msg) Mylogger::getInstance()->error(__FILE__,__LINE__,__FUNCTION__,msg)
//#define LOG_DEBUG(msg) Mylogger::getInstance()->debug(__FILE__,__LINE__,__FUNCTION__,msg)
//#define LOG_INFO(msg) Mylogger::getInstance()->info(__FILE__,__LINE__,__FUNCTION__,msg)

namespace Titan{

    class Mylogger{
    public:
        void warn(const char * file, int line, const char * func, const char * msg);
        void error(const char * file, int line, const char * func, const char * msg);
        void debug(const char * file, int line, const char * func, const char * msg);
        void info(const char * file, int line, const char * func, const char * msg);

        static Mylogger * getInstance();
        static void destroy();


        inline void LOG_WARN(const char *msg){
            Mylogger::getInstance()->warn(__FILE__,__LINE__,__FUNCTION__,msg);
        }
        inline void LOG_ERROR(const char *msg){
            Mylogger::getInstance()->error(__FILE__,__LINE__,__FUNCTION__,msg);
        }
        inline void LOG_DEBUG(const char *msg){
            Mylogger::getInstance()->debug(__FILE__,__LINE__,__FUNCTION__,msg);
        }
        inline void LOG_INFO(const char *msg){
            Mylogger::getInstance()->info(__FILE__,__LINE__,__FUNCTION__,msg);
        }
    private:
        Mylogger();
        ~Mylogger();
        Mylogger(const Mylogger &) = delete;
        Mylogger & operator=(const Mylogger &) = delete;
        static Mylogger * _pInstance;
        static Category * _root;
    };

}

#endif