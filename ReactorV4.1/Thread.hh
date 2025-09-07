#ifndef  __Thread_H__
#define  __Thread_H__

#include <functional>
#include <func.h>
#include <pthread.h>


namespace ReactorV41{
using ThreadCallback=std::function<void()>;

class Thread{
public:
    Thread(ThreadCallback  && cb);

    void start();
    void join();

private:
    static void *start_routine(void *);
private:
    pthread_t       _pthid;
    bool            _isRunning;
    ThreadCallback  _cb;
};

}


#endif
