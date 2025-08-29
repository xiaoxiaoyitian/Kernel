#include "TaskQueue.hh"
#include "MutexLock.hh"
#include "Condition.hh"



namespace MyThreadPool{
TaskQueue::TaskQueue(int qSize)
:_queue()
,_queSize(qSize)
,_mutex()
,_flag(true)
,_notEmpty(_mutex)
,_notFull(_mutex)
{

}

bool TaskQueue::empty() const{
    return _queue.size()==0;
}

bool TaskQueue::full() const{
    return _queue.size()==_queSize;
}

void TaskQueue::push(ElemType elem){
    MutexLockGuard autolock(_mutex);
    while(full()){
        _notFull.wait();
    }
    _queue.push(elem);
    _notEmpty.notify();
}

ElemType TaskQueue::pop(){
    MutexLockGuard  autolock(_mutex);
    while(empty()&&_flag){
        _notEmpty.wait();
    }
    if(_flag){
        ElemType temp =_queue.front();
        _queue.pop();
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
