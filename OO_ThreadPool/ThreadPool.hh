#ifndef __ThreadPool_H__
#define __ThreadPool_H__
#include "Task.hh"
#include "Thread.hh"
#include "TaskQueue.hh"

#include <pthread.h>
#include <vector>
#include <memory>
using std::vector;
using std::unique_ptr;

namespace MyThreadPool{

class ThreadPool{

    friend class WorkerThread;
public:
    ThreadPool(int threadNum,int queSize);
    void start();
    void stop();
    void addTask(Task *task);

private:
    void doTask();
private:
    vector<unique_ptr<Thread>> _threads;
    int _threadNum;
    TaskQueue _taskqueue;
    int _queSize;
    bool _isExit;
}; 

}


#endif
