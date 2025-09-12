#include "ThreadPool.hh"
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <functional>
#include <memory>

using std::bind;
using std::unique_ptr;

class Task{
public:
void process(){
        srand(time(nullptr));
        int num=rand()%100;
        printf("make a number is %d\n",num);
        sleep(1);
}
};
void test(){

    unique_ptr<Task> ptask(new Task());
    Titan::ThreadPool pool(4,10);
    pool.start();
    int cnt=10;
    while(cnt-->0){
        pool.addTask(bind(&Task::process,ptask.get()));
    }
    pool.stop();
}

int main()
{
 test();
 return 0;
}
