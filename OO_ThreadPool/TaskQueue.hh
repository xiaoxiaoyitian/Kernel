#ifndef __TaskQueue_H__
#define __TaskQueue_H__

#include "MutexLock.hh"
#include "Condition.hh"
#include "Task.hh"
#include <queue>
#include <iostream>
using std::cout;
using std::endl;
using std::queue;
using ElemType=MyThreadPool::Task *;

namespace  MyThreadPool{

class TaskQueue{
public:
    TaskQueue(int qSize);

    void push(ElemType elem);
    ElemType  pop();
    bool empty() const;
    bool full() const;
    void wakeup();

private:
    queue<ElemType> _queue;
    int             _queSize;
    bool            _flag;
    MutexLock       _mutex;
    Condition       _notEmpty;
    Condition       _notFull;
};

}


#endif
