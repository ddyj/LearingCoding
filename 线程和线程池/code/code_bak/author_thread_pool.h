#ifndef __AUTHOR_THREAD_POOL_H__
#define __AUTHOR_THREAD_POOL_H__
#include <atomic>
#include <condition_variable>
#include <future>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>
#include <functional>

class ThreadPool  {
public:
    ThreadPool(const ThreadPool&) = delete;
    ThreadPool&operator=(const ThreadPool&) = delete;
    static ThreadPool& instance() {
        static ThreadPool ins;
        return ins;
    }
    static ThreadPool& instance(int thread_num) {
        static ThreadPool ins(thread_num);
        return ins;
    }
    using Task = std::packaged_task<void()>;
    ~ThreadPool() {
        if(!stop_.load()){
            wait();
            stop();
            printf("ThreadPool Destroyed\n");
        }
    }
    void wait_for_empty(){
        this->wait();
    }
    //这里属于C++新特性 使用auto作为函数返回值的时候，需要用->声明返回auto的对象
    //std::decltype(args) 返回args的类型
    template <class F, class... Args>
    auto commit(F&& f, Args&&... args) -> std::future<decltype(f(args...))> {
        using RetType = decltype(f(args...));
        if (stop_.load())
            return std::future<RetType>{};
        auto task = std::make_shared<std::packaged_task<RetType()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...));
        std::future<RetType> ret = task->get_future();
        {
            std::unique_lock<std::mutex> cv_mt(cv_mt_);
            //这里传入task的lamda表达式
            tasks_.emplace([task] { (*task)(); });
        }
        cv_lock_.notify_one();
        return ret;
    }
    int idleThreadCount() {
        return thread_num_;
    }
private:
    ThreadPool(unsigned int num = 5)
        : stop_(false) {
            {
                if (num < 1)
                    thread_num_ = 1;
                else
                    thread_num_ = num;
            }
            start();
    }
    void start() {
        for (int i = 0; i < thread_num_; ++i) {
            pool_.emplace_back([this]() {
                while (!this->stop_.load()) {
                    Task task;
                    {
                        std::unique_lock<std::mutex> cv_mt(cv_mt_);
                        //当线程池停止或者线程中有任务才会向下执行
                        this->cv_lock_.wait(cv_mt, [this] {
                            return this->stop_.load() || !this->tasks_.empty();
                        });
                        //如果此时队列中没有任务，就说明可以关闭了
                        if (this->tasks_.empty() && this->stop_.load())
                            return;
                        //此时是两种情况 1.stop 清空任务队列 2.没停 正常取任务出来
                        task = std::move(this->tasks_.front());
                        this->tasks_.pop();
                    }
                    //atomic_int的父类atomic_base重载了++ -- 所以这里可以并不用fetch
                    this->thread_num_--;
                    //调用仿函数的方式
                    task();
                    this->thread_num_++;
                    cv_wait_.notify_all();
                }
            });
        }
    }

    //注意这里的写法，每次有任务完成都会唤醒这个线程，while会重新判断task_是否为空，只有task_为空了才会结束wait
    void wait(){
        std::unique_lock<std::mutex>lck(cv_mt_);
        while(!tasks_.empty()){
            cv_wait_.wait(lck);
        }
    }


    void stop() {
        stop_.store(true);
        cv_lock_.notify_all();
        for (auto& td : pool_) {
            if (td.joinable()) {
                std::cout << "join thread " << td.get_id() << std::endl;
                td.join();
            }
        }
        //通知全局 避免wait还在阻塞
        cv_wait_.notify_all();
    }
private:
    std::mutex               cv_mt_;
    std::condition_variable  cv_lock_;
    std::condition_variable  cv_wait_;
    std::atomic_bool         stop_;
    std::atomic_int          thread_num_;
    std::queue<Task>         tasks_;
    std::vector<std::thread> pool_;
};
#endif  // !__THREAD_POOL_H__