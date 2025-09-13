#ifndef _THREADPOOL_H
#define _THREADPOOL_H
#include "TaskQueue.hh"
#include "Thread.hh"
#include <vector>
#include <functional>
#include <memory>
using namespace std;
namespace Titan{
using FuncTask=function<void()>;
class ThreadPool{
public:
    ThreadPool(int,int);
    ~ThreadPool();
    void start();
    void stop();
    void doTask();
    void addTask(FuncTask &&);
private:
    vector<unique_ptr<Thread>>    _threads;
    int                           _threadNum;
    TaskQueue                     _que;
    bool                          _isExit;
};
}
#endif //!_THREADPOOL_H
