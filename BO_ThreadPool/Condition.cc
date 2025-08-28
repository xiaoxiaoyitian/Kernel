#include "Condition.hh"
#include "MutexLock.hh"
#include <pthread.h>

namespace MyThreadPool{

Condition::Condition(MutexLock &m)
:_mutex(m)
,_cond()
{
    pthread_cond_init(&_cond,nullptr);
}

Condition::~Condition(){
    pthread_cond_destroy(&_cond);
}

void Condition::wait(){
    pthread_cond_wait(&_cond,_mutex.GetMutexLockPtr());
}

void Condition::notify(){
    pthread_cond_signal(&_cond);
}

void Condition::notifyAll(){
    pthread_cond_broadcast(&_cond);
}


}
