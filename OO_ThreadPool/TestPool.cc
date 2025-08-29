#include "Task.hh"
#include "ThreadPool.hh"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <memory>

using std::cout;
using std::endl;
using std::unique_ptr;


class MyTask:public MyThreadPool::Task{
public:
    void process() override{
        srand(time(nullptr));
        int number =rand()%100;
        cout<<"make a number :"<<number <<endl;
        sleep(1);
    }
};


void test(){
    MyThreadPool::ThreadPool threadpool(4,10);

    threadpool.start();

    unique_ptr<MyThreadPool::Task> ptask(new MyTask);

    int count=20;
    while(count--){
        threadpool.addTask(ptask.get());
    }

    threadpool.stop();
}

int main()
{
    test();
    return 0;
}

