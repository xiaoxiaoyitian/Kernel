#ifndef __WorkerThread_H__
#define __WorkerThread_H__

#include "Thread.hh"
#include "ThreadPool.hh"

namespace MyThreadPool{
class WorkerThread:public Thread{
public:
    WorkerThread(ThreadPool &threadpool)
    :_threadpool(threadpool){

    }
private:
    void run() override{
        _threadpool.doTask();
    };
private:
    ThreadPool &_threadpool;
};
}

#endif
