#include "Thread.hh"
#include <string.h>
#include <stdio.h>
namespace Titan{
Thread::Thread(ThreadCallBack &&cb)
:_pthid(0),_isrunning(false),_cb(std::move(cb))
{

}

void Thread::start(){
    if(!_isrunning){
        int ret=pthread_create(&_pthid,NULL,start_routine,this);
        if(ret!=0){
            fprintf(stderr,"%s\n",strerror(ret));
        }
    }
    _isrunning=true;
}
void Thread::join(){
    if(_isrunning){
        pthread_join(_pthid,NULL);
        _isrunning=false;
    }
}

void *Thread::start_routine(void *arg){
    Thread* th=static_cast<Thread *> (arg);
    if(th){
        th->_cb();
    }
    return NULL;
}
}


