#ifndef  __ThreadPool_H__
#define  __ThreadPool_H__

#include "Task.hh"
#include "Thread.hh"
#include "TaskQueue.hh"

#include <memory>
#include <vector>

using std::vector;
using std::unique_ptr;


namespace ReactorV5{

class ThreadPool
{
public:
    ThreadPool(int,int);
    ~ThreadPool();

    void start();
    void stop();
    void addTask(Task  &&cb);

private:

    void doTask();


private:
    vector<unique_ptr<Thread>>      _threads;
    size_t                          _threadNum;
    TaskQueue                       _taskque;
    bool                            _isExit;
};  

}



#endif
