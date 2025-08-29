#include "ThreadPool.hh"
#include "Thread.hh"
#include "WorkerThread.hh"
#include <unistd.h>



namespace MyThreadPool{
ThreadPool::ThreadPool(int threadNum, int queSize)
:_threads()
,_threadNum(threadNum)
,_taskqueue(queSize)
,_queSize(queSize)
,_isExit(false)
{
    _threads.reserve(_threadNum);
}



void ThreadPool::start(){
    for(int i=0;i<_threadNum;++i){
        unique_ptr<Thread> ptr(new WorkerThread(*this));
        _threads.push_back(std::move(ptr));
    }

    for(auto &thread:_threads){
        thread->start();
    }
}

void ThreadPool::stop(){
    while(!_taskqueue.empty()){
        sleep(1);
    }

    if(!_isExit){
        _isExit=true;

        _taskqueue.wakeup();

        for(auto &thread:_threads){
            thread->join();
        }
    }

}

void ThreadPool::addTask(Task *task){
    if(task){
        _taskqueue.push(task);
    }
}

void ThreadPool::doTask(){
    while(!_isExit){
        Task *ptask=_taskqueue.pop();

        if(ptask){
            ptask->process();
        }
    }
}

}



