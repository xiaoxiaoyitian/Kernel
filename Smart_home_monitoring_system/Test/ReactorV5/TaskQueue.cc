#include "TaskQueue.hh"
#include "MutexLock.hh"

namespace Titan{
TaskQueue::TaskQueue(int size)
:_que()
,_queSize(size)
,_mutex()
,_notEmpty(_mutex)
,_notFull(_mutex)
,_flag(true)
{

}

bool TaskQueue::empty(){
    return  0==_que.size();
}

bool TaskQueue::full(){
    return _queSize==static_cast<int>(_que.size());
}

void TaskQueue::push(ElemType &&task){
    //加锁
    MutexLockGuard autolock(_mutex);

    while(full()){
        _notFull.wait();//等待不满条件变量唤醒
    }

    _que.push(std::move(task));
    _notEmpty.notifyOne();//唤醒不空条件变量

    //解锁
    _mutex.unlock();
}

ElemType TaskQueue::pop(){
    //加锁
    MutexLockGuard autolock(_mutex);


    while(empty()&&_flag){
        _notEmpty.wait();//等待不满条件变量唤醒
    }
    if(_flag){
        ElemType temp=_que.front();
        _que.pop();
        _notFull.notifyOne();//唤醒不满条件变量
                             //解锁
        _mutex.unlock();
        return temp;
    }else{
        _mutex.unlock();
        return nullptr;
    }
}

void TaskQueue::wake(){
    //唤醒所有在不空条件上等待的条件变量
    _flag=false;
    _notEmpty.notifyAll();
}
}
