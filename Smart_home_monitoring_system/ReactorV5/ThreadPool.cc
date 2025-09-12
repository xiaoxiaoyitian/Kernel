#include "ThreadPool.hh"
#include <unistd.h>
namespace Titan{
ThreadPool::ThreadPool(int threadnum,int quesize)
:_threads()
,_threadNum(threadnum)
,_que(quesize)
,_isExit(false)
{
    //预先为vector分配空间存储工作子线程
    _threads.reserve(_threadNum);
}
ThreadPool::~ThreadPool(){
    if(!_isExit){
        stop();
    }
}

void ThreadPool::start(){
    //创建工作子线程
    for(int i=0;i<_threadNum;++i){
         unique_ptr<Thread> thread(new Titan::Thread(bind(&ThreadPool::doTask,this)));
        _threads.push_back(std::move(thread));
    }

    //启动工作子线程
    for(auto &th:_threads){
        th->start();
    }
}

void ThreadPool::stop(){
    while(!_que.empty()){
        sleep(1);
    }

    _isExit=true;
    _que.wake();

    //等待工作子线程结束
    for(auto &th:_threads){
        th->join();
    }
}

void ThreadPool::addTask(FuncTask &&task){
    if(task){
        _que.push(std::move(task));
    }
}

void ThreadPool::doTask(){
    while(!_isExit){
        FuncTask ptask=_que.pop();
        if(ptask){
            ptask();
        }
    }
}
}
