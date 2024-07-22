#pragma once

#ifndef  _THREAD_POOL_H
#define _THREAD_POOL_H


#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>    //c++ 11 可调用对象包装器
#include <stdexcept>
#include<iostream>

class ThreadPool {
public:
    ThreadPool(size_t);
    template<class F, class... Args>
    auto enqueue(F&& f, Args&&... args) 
        -> std::future<typename std::result_of<F(Args...)>::type>;
    void Stop();
    void Wait(); //等待任务完成
    ~ThreadPool();
private:
    // need to keep track of threads so we can join them
    std::vector< std::thread > workers;
    // the task queue
    std::queue< std::function<void()> > tasks;
    int runing_task_num;
    // synchronization
    std::mutex queue_mutex;
    std::condition_variable condition;
    std::condition_variable wait_condition; //如果有任务正在执行 阻塞直到没有任务
    bool stop;
};
 
 // add new work item to the pool
template<class F, class... Args>
auto ThreadPool::enqueue(F&& f, Args&&... args) 
    -> std::future<typename std::result_of<F(Args...)>::type>
{
    using return_type = typename std::result_of<F(Args...)>::type;
 
    auto task = std::make_shared< std::packaged_task<return_type()> >(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );
        
    std::future<return_type> res = task->get_future();
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
 
        // don't allow enqueueing after stopping the pool
        if(stop)
            throw std::runtime_error("enqueue on stopped ThreadPool");
        //emplace通过传入的数据构造一个对象，并没有push的拷贝的过程
        tasks.emplace([task](){ (*task)(); });
    }
    condition.notify_all();
    return res;
}

#endif