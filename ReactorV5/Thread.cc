#include "Thread.hh"
#include <cstdio>
#include <iostream>
#include <pthread.h>
using std::cout;
using std::endl;


namespace ReactorV5{

Thread::Thread(ThreadCallback && cb)
:_pthid(0)
,_isRunning(false)
,_cb(std::move(cb))
{

}

void Thread::start(){
    if(!_isRunning){
        int ret=pthread_create(&_pthid,nullptr,
                               start_routine,this);

        if(ret!=0){
            fprintf(stderr,"%s\n",strerror(ret));
            return;
        }
        _isRunning=true;
        return;
    }
}

void Thread::join(){
    if(_isRunning){
        pthread_join(_pthid,NULL);
        _isRunning=false;
    }
}

void * Thread::start_routine(void *arg)
{
    Thread *pthread=static_cast<Thread*>(arg);
    if(pthread){
        pthread->_cb();
    }
    return NULL;
}

}


