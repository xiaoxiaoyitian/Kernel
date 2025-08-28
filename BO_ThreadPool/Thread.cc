#include "Thread.hh"
#include <cctype>
#include <cstddef>
#include <string.h>
#include <stdio.h>


namespace MyThreadPool{

Thread::Thread(ThreadCallBack && tcb)
:_pthid(0)
,_isRunning(false)
,_tcb(std::move(tcb))
{}
Thread::~Thread(){}

void Thread::start(){
    if(!_isRunning){
        int ret=pthread_create(&_pthid,NULL,start_routine,this);
        if(ret!=0){
            fprintf(stderr,"%s\n",strerror(ret));
            return ;
        }
        _isRunning=true;
    }
}

void Thread::join(){
    if(_isRunning){
        pthread_join(_pthid,NULL);
        _isRunning=false;
    }
}

void * Thread:: start_routine(void *arg){
    Thread *pthread=static_cast<Thread *>(arg);
    if(pthread){
        pthread->_tcb();
    }
    return NULL;
}


}

