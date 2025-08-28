#include "TaskQueue.hh"
#include <features.h>

namespace MyThreadPool{

TaskQueue::TaskQueue(size_t queSize)
    :_queue()
    ,_queSize(queSize)
    ,_mutex()
    ,_notFull(_mutex)
    ,_notEmpty(_mutex)
    ,_flag(true)
{
}

TaskQueue::~TaskQueue(){}


bool TaskQueue::empty() const{
    return 0==_queue.size();
}

bool TaskQueue::full()  const{
    return _queSize==_queue.size();
}


void TaskQueue::push(ElemType &&elem){
    _mutex.lock();
    while(full()){
        _notFull.wait();
    }
    _queue.push(std::move(elem));
    _notEmpty.notify();
    _mutex.unlock();
}

ElemType TaskQueue::pop(){
    _mutex.lock();
    while(empty()&&_flag){
        _notEmpty.wait();
    }
    if(_flag){
        ElemType temp=_queue.front();

        _queue.pop();
        _mutex.unlock();
        _notFull.notify();
        return temp;
    }else{
        _mutex.unlock();
        return nullptr;
    }
}

void TaskQueue::wakeup(){
    _flag=false;
    _notEmpty.notifyAll();
}


}
