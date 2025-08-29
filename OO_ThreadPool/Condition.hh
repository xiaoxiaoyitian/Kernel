#ifndef __Condition_H__
#define __Condition_H__

#include "MutexLock.hh"
#include <pthread.h>

namespace MyThreadPool{

class Condition {
public:
    Condition(MutexLock &m);
    ~Condition();

    void wait();
    void notify();
    void notifyAll();
private:
    MutexLock &_mutex;
    pthread_cond_t _cond;

};

}
#endif
