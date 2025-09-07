#include "ThreadPool.hh"
#include <unistd.h>
#include <iostream>
using std::cout;
using std::endl;

namespace  ReactorV40{

ThreadPool::ThreadPool(int threadNum,int queSize)
:_threads()
,_threadNum(threadNum)
,_taskque(queSize)
,_isExit(false)
{
    _threads.reserve(_threadNum);
}

ThreadPool::~ThreadPool(){
    if(!_isExit){
        stop();
    }
}

//开启n个子线程的执行
void ThreadPool::start(){
    for(int i=0;i<_threadNum;++i){
        unique_ptr<Thread> up(new Thread(
            std::bind(&ThreadPool::doTask,this)));
        _threads.push_back(std::move(up));
    }

    for(auto &pthread :_threads){
        pthread->start();
    }
}


//添加任务
void ThreadPool::addTask(Task && task){
    if(task){
        _taskque.push(std::move(task));
    }
}


void ThreadPool::stop(){
    while(!_taskque.empty()){
        sleep(1);
    }
    
    //修改线程池退出时的状态标志位
    _isExit=true;
    //再唤醒所有子线程
    _taskque.wakeup();
    cout<<"stop:wakeup...."<<endl;

    //回收每一个子线程
    for(auto & pthread:_threads){
        pthread->join();
    }
}

void ThreadPool::doTask(){
    while(!_isExit){

        Task task=_taskque.pop();
        if(task){
            task();
        }
    }
}

}



