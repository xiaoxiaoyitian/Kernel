#ifndef _THREAD_H
#define _THREAD_H

#include "NoCopyable.hh"
#include <pthread.h>
#include <functional>

using std::function;

namespace Titan{

class Thread:NoCopyable{
    using ThreadCallBack=function<void()>;
public:
    Thread(ThreadCallBack &&);
    void start();
    void join();
private:
    static  void * start_routine(void *);
private:
    pthread_t _pthid;
    bool _isrunning;
    ThreadCallBack _cb;
};
}
#endif

