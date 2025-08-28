#ifndef __Thread_H__
#define __Thread_H__

#include <pthread.h>
#include <functional>

using std::function;

namespace MyThreadPool{


using ThreadCallBack=function<void ()>;

class Thread{

public:
    Thread(ThreadCallBack &&);
    ~Thread();

    void start();
    void join();

private:
    static void *start_routine(void *arg);

private:
    pthread_t _pthid;
    bool _isRunning;
    ThreadCallBack _tcb;
};

}


#endif
