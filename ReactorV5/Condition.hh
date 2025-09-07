#ifndef  __Condition_H__
#define  __Condition_H__

#include <pthread.h>

namespace ReactorV5{

class MutexLock;
class Condition{
public:
    Condition(MutexLock &);
    ~Condition();

    void wait();
    void notify();
    void notifyAll();


private:
    pthread_cond_t  _cond;
    MutexLock &     _mutex;
};

}
#endif
