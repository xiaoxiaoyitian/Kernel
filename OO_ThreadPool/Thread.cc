#include "Thread.hh"
#include <pthread.h>
#include <stdio.h>
#include <string.h>
namespace MyThreadPool{

Thread::Thread()
    :_pthid(0)
    ,_isRunning(false)
{

}

void Thread::start(){
    if(!_isRunning){
        int ret=pthread_create(&_pthid,NULL,start_routine,this);

        if(ret!=0){
            fprintf(stderr,"%s\n",strerror(ret));
            return;
        }
        _isRunning =true;
        return;
    }
}


void Thread::join(){
    if(_isRunning){
        pthread_join(_pthid,NULL);
        _isRunning=false;
    }
}

void *Thread::start_routine(void *arg){
    Thread *pthread =static_cast<Thread *>(arg);

    if(pthread){
        pthread->run();
    }

    return NULL;
}




}
