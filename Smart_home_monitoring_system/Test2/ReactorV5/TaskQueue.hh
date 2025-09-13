#ifndef _TASKQUEUE_H
#define _TASKQUEUE_H

#include "MutexLock.hh"
#include "Condition.hh"
#include <queue>
#include <functional>

using namespace std;
namespace Titan{
using ElemType=function<void()>;
class TaskQueue{
public:
    TaskQueue(int);
    bool empty();
    bool full();
    void push(ElemType &&);
    ElemType pop();
    void wake();
private:
    queue<ElemType>  _que;//存放工作任务的队列
    int              _queSize;
    MutexLock        _mutex;
    Condition        _notEmpty;
    Condition        _notFull;
    bool             _flag;//用于判断是否结束任务队列
};
}
#endif

