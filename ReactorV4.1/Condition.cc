#include "Condition.hh"
#include "MutexLock.hh"
#include <iostream>
#include <pthread.h>
using std::cout;
using std::endl;


namespace ReactorV41{

Condition::Condition(MutexLock & m)
:_mutex(m)
{
    pthread_cond_init(&_cond,nullptr);


}


Condition::~Condition(){
    pthread_cond_destroy(&_cond);
}

void Condition::wait(){
    pthread_cond_wait(&_cond,_mutex.getMutexLockPtr());
}

void Condition::notify(){
    pthread_cond_signal(&_cond);
}

void Condition::notifyAll(){
    pthread_cond_broadcast(&_cond);
}

}


