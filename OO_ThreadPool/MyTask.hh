#ifndef __MyTask_H__
#define __MyTask_H__

#include "Task.hh"

namespace MyThreadPool{
class MyTask:public Task{
public:
    void process() override;
};
}
#endif
