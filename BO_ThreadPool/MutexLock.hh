#ifndef __MUtexLock_H__
#define __MUtexLock_H__


#include <pthread.h>


namespace MyThreadPool{


class MutexLock{
public:
    MutexLock();
    ~MutexLock();

    void lock();
    void unlock();

    pthread_mutex_t *GetMutexLockPtr(){
        return &_mutex;
    }
    
private:
    pthread_mutex_t _mutex;

};

}
#endif
