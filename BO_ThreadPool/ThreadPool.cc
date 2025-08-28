#include "ThreadPool.hh"
#include "Thread.hh"
#include <functional>
#include <iostream>
#include <unistd.h>

using std::cout;
using std::endl;



namespace MyThreadPool{

ThreadPool::ThreadPool(size_t threadNum,size_t queSize)
    :_threads()
    ,_threadNum(threadNum)
    ,_taskQueue(queSize)
    ,_isExit(false)
{
    _threads.reserve(10);
}
ThreadPool::~ThreadPool(){
    if(_isExit){
        stop();
    }
}

void ThreadPool::start(){
    for(int i=0;i<_threadNum;++i){
        unique_ptr<Thread> ptr(new Thread(std::bind(&ThreadPool::doTask,this)));
        _threads.push_back(std::move(ptr));
    }

    for(auto &pthread:_threads){
        pthread->start();
    }
}


void ThreadPool::addTask(Function &&func){
    if(func){
        _taskQueue.push(std::move(func));
    }

}

void ThreadPool::stop(){
    while(!_taskQueue.empty()){
        sleep(1);
    }
    _isExit=true;
    _taskQueue.wakeup();
    cout<<"wakeup()"<<endl;

    for(auto &pthread:_threads){
        pthread->join();
    }
}

void ThreadPool::doTask(){
    while(!_isExit){
        Function func=_taskQueue.pop();
        if(func){
            func();
        }
    }
}

}
