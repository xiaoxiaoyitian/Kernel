#ifndef __Thread_H__
#define __Thread_H__

#include <pthread.h>


namespace MyThreadPool{

class Thread{
public:
    Thread();
    virtual ~Thread(){}

    void start();
    void join();

private:
    virtual void run()=0;
    static void *start_routine(void *);


private:
    pthread_t _pthid;
    bool      _isRunning;  
};

}

#endif
