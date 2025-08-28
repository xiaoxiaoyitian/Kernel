#ifndef __TaskQueue_H__
#define __TaskQueue_H__

#include "MutexLock.hh"
#include "Condition.hh"
#include <pthread.h>
#include <functional>
#include <queue>

using std::queue;

namespace MyThreadPool{

using ElemType =std::function<void()>;

class TaskQueue{
public:
    TaskQueue(size_t queSize);
    ~TaskQueue();

    bool empty() const;
    bool full() const;
    void push(ElemType &&);
    ElemType pop();
    void wakeup();

private:
    queue<ElemType> _queue;
    size_t          _queSize;
    MutexLock       _mutex;
    Condition       _notFull;
    Condition       _notEmpty;
    bool            _flag;
};

}

#endif
