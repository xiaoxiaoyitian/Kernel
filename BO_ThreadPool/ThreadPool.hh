#ifndef __ThreadPool_H__
#define __ThreadPool_H__


#include "Thread.hh"
#include "TaskQueue.hh"
#include <memory>

#include <functional>

using std::vector;
using std::unique_ptr;


namespace MyThreadPool{

using Function=std::function<void ()>;

class ThreadPool{
public:
    ThreadPool(size_t threadNum,size_t queSize);
    ~ThreadPool();


    void start();
    void stop();
    void addTask(Function &&);
private:
    void doTask();

private:
    vector<unique_ptr<Thread>>_threads;
    size_t _threadNum;
    TaskQueue _taskQueue;
    bool        _isExit;

};

}

#endif
