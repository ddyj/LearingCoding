#include"Thread_pool.h"

// the constructor just launches some amount of workers
//这里每一个thread也传入的是仿函数
ThreadPool::ThreadPool(size_t threads)
    :   stop(false),runing_task_num(0)
{
    for(size_t i = 0;i<threads;++i)
        workers.emplace_back(
            [this]
            {
                for(;;)
                {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(this->queue_mutex);
                        this->condition.wait(lock,
                            [this]{ return this->stop || !this->tasks.empty(); });   //wait--true--阻塞
                        if(this->stop && this->tasks.empty())
                            return;     //中止 直接返回 关闭线程
                        task = std::move(this->tasks.front());
                        this->tasks.pop();
                        ++runing_task_num;
                    }
 
                    task();
                    std::unique_lock<std::mutex> lock(queue_mutex);   //任务已经完成，暂停while循环
                    --runing_task_num;
                    wait_condition.notify_all();
                }
            }
        );
}



ThreadPool::~ThreadPool(){
    if(!stop){
        Wait();
        Stop();
        printf("ThreadPool Destroyed\n");
    }
}
// the destructor joins all threads
void ThreadPool::Stop()
{
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        stop = true;
    }
    condition.notify_all();
    for(std::thread &worker: workers)
        worker.join();
    wait_condition.notify_all();
}
 
void ThreadPool::Wait(){
    std::unique_lock<std::mutex>lck(queue_mutex);
    while(this->runing_task_num>0||this->tasks.size()>0){
        wait_condition.wait(lck);
    }
}


