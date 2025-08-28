#include "Thread.hh"
#include "ThreadPool.hh"
#include <time.h>
#include <unistd.h>
#include <iostream>
using std::cout;
using std::endl;


class Task{
public:
    void process(){
        srand(time(NULL));
            int number=rand()%100;
            cout<<"number is "<<number<<endl;
            sleep(1);
    }
};

void test(){
    MyThreadPool::ThreadPool threadpool(4,10);
    threadpool.start();
    unique_ptr<Task> ptask(new Task);
    int num=10;
    while(num--){
        threadpool.addTask(std::bind(&Task::process,ptask.get()));
    }
    threadpool.stop();
    
}

int main()
{
    test();
    return 0;
}

