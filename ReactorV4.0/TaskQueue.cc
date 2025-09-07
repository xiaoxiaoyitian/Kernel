#include "TaskQueue.hh"
#include "MutexLock.hh"
#include <iostream>
using std::cout;
using std::endl;


namespace ReactorV40{

TaskQueue::TaskQueue(size_t sz)
:_que()
,_queSize(sz)
,_mutex()
,_notEmpty(_mutex)
,_notFull(_mutex)
,_flag(true)
{}

bool TaskQueue::empty() const{
    return _que.size()==0;
}

bool TaskQueue::full() const{
    return _que.size()==_queSize;
}

void TaskQueue::push(ElemType && elem){
    MutexLockGuard autolock(_mutex);

    while(full()){
        _notFull.wait();
    }

    _que.push(std::move(elem));
    _notEmpty.notify();
}

ElemType TaskQueue::pop(){
    MutexLockGuard autolock(_mutex);
    while(_flag&&empty()){
        _notEmpty.wait();
    }

    if(_flag){
        ElemType temp=_que.front();
        _que.pop();

        _notFull.notify();
        return temp;
    }else{
        return nullptr;
    }
}

void TaskQueue::wakeup(){
    _flag=false;
    _notEmpty.notifyAll();
}

}


