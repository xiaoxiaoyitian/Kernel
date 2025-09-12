#ifndef _CONDITION_H
#define _CONDITION_H

#include "NoCopyable.hh"
#include <pthread.h>
namespace Titan{
class MutexLock;
class Condition: NoCopyable{
public:
    Condition(MutexLock &);
    ~Condition();
    void wait();
    void notifyOne();
    void notifyAll();
private:
    pthread_cond_t _cond;
    MutexLock & _mutex;
};
}
#endif

