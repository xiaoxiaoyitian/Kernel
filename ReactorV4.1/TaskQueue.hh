#ifndef  __TaskQueue_H__
#define  __TaskQueue_H__

#include "Task.hh"
#include "Condition.hh"
#include "MutexLock.hh"

#include <cstddef>
#include <queue>
using std::queue;
using std::size_t;


namespace ReactorV41{

using ElemType =Task;

class TaskQueue{
public:
    TaskQueue(size_t sz);
    bool    empty() const;
    bool    full()  const;
    void    push(ElemType  && elem);
    ElemType pop();
    void    wakeup();
private:
    queue<ElemType>         _que;
    size_t                  _queSize;
    MutexLock               _mutex;
    Condition               _notEmpty;
    Condition               _notFull;
    bool                    _flag;
};

}
#endif
