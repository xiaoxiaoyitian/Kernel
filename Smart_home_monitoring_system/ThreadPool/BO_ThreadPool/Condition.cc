#include "Condition.hh"
#include "MutexLock.hh"
namespace Titan{
Condition::Condition(MutexLock &rhs)
:mutex(rhs)
{
    pthread_cond_init(&_cond,NULL);

}
Condition::~Condition(){
    pthread_cond_destroy(&_cond);
}
void Condition::wait(){
    pthread_cond_wait(&_cond,mutex.getMutexLockPtr());
}
void Condition::notifyOne(){
    pthread_cond_signal(&_cond);
}
void Condition::notifyAll(){
    pthread_cond_broadcast(&_cond);
}
}
