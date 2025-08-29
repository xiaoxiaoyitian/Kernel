#ifndef __Task_H__
#define __Task_H__

namespace MyThreadPool{
class Task{
public:
    virtual void process()=0;
};
}
#endif
