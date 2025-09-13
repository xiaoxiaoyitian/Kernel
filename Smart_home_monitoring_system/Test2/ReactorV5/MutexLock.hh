#ifndef _MUTEXLOCK_H
#define _MUTEXLOCK_H

#include "NoCopyable.hh"
#include <pthread.h>
namespace Titan{

class MutexLock:NoCopyable{

public:
    MutexLock();
    ~MutexLock();
    void lock();
    void unlock();
    pthread_mutex_t * getMutexLockPtr(){return &_mutex;};
private:
    pthread_mutex_t _mutex;
};


class MutexLockGuard{
public:
    MutexLockGuard(MutexLock &m)
    :_mutex(m){
        _mutex.lock();
    }

    ~MutexLockGuard(){
        _mutex.unlock();
    }
private:
    MutexLock    &   _mutex;
};
}

#endif

