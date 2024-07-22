//
// Created by dai on 2024/4/8.
//

#ifndef __INTERUPTHREAD_H__
#define __INTERUPTHREAD_H__
#pragma once
#include <thread>
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <iostream>
#include <exception>
#include <string>
#include <future>

extern void interruption_point();
class interrupt_flag
{
    std::atomic<bool> flag;
    //需要中断线程中的条件变量需要和这个指针对齐
    std::condition_variable* thread_cond;
    std::condition_variable_any* thread_cond_any;
    std::mutex set_clear_mutex;
public:
    interrupt_flag() :
           flag(false), thread_cond(0), thread_cond_any(0)
    {}
    void set()
    {
        flag.store(true, std::memory_order_relaxed);
        std::lock_guard<std::mutex> lk(set_clear_mutex);
        if (thread_cond)
        {
            thread_cond->notify_all();
        }
        else if (thread_cond_any) {
            thread_cond_any->notify_all();
        }
    }
    bool is_set() const
    {
        return flag.load(std::memory_order_relaxed);
    }
    void set_condition_variable(std::condition_variable& cv)
    {
        std::lock_guard<std::mutex> lk(set_clear_mutex);
        thread_cond = &cv;
    }
    void set_condition_variable_any(std::condition_variable_any& cv){
        std::lock_guard<std::mutex> lk(set_clear_mutex);
        thread_cond_any = &cv;
    }
    void clear_condition_variable()
    {
        std::lock_guard<std::mutex> lk(set_clear_mutex);
        thread_cond = 0;
    }
    void clear_condition_variable_any(){
        std::lock_guard<std::mutex> lk(set_clear_mutex);
        thread_cond_any = 0;
    }


    template<typename Lockable>
    void wait(std::condition_variable_any& cv, Lockable& lk) {
        struct custom_lock {
            interrupt_flag* self;
            Lockable& lk;
            custom_lock(interrupt_flag* self_, std::condition_variable_any& cond, Lockable& lk_) :
                    self(self_), lk(lk_) {
                self->set_clear_mutex.lock();
                self->thread_cond_any = &cond;
            }

            void unlock() {
                lk.unlock();
                self->set_clear_mutex.unlock();
            }

            void lock() {
                std::lock(self->set_clear_mutex, lk);
            }

            ~custom_lock() {
                self->thread_cond_any = 0;
                self->set_clear_mutex.unlock();
            }
        };

        custom_lock cl(this, cv, lk);
        interruption_point();
        cv.wait(cl);
        interruption_point();
    }
};


struct clear_cv_on_destruct {
    ~clear_cv_on_destruct();
};


//前置声明 告诉编译器在其他文件中获取声明的变量的定义
extern thread_local interrupt_flag this_thread_interrupt_flag;
//异常说明
class thread_interrupted : public std::exception
{
public:
    thread_interrupted() : message("thread interrupted.") {}
    ~thread_interrupted() throw () {
    }

    virtual const char* what() const throw () {
        return message.c_str();
    }

private:
    std::string message;
};

class interruptible_thread
{
private:
    std::thread internal_thread;
    //停止标记
    interrupt_flag* flag;
public:
    template<typename FunctionType,typename ...Args>
    interruptible_thread(FunctionType f,Args ...args)
    {
        using RetType = decltype(f(args...));
        std::promise<interrupt_flag*> p;
        auto f_ret = std::bind(std::forward<FunctionType>(f),std::forward<Args>(args)...);
        internal_thread = std::thread([f_ret, &p ]{
            p.set_value(&this_thread_interrupt_flag);
            f_ret();
        });
        flag = p.get_future().get();
    }

    void join() {
        internal_thread.join();
    }
    void interrupt()
    {
        if (flag)
        {
            flag->set();
        }
    }
};


extern void interruptible_wait(std::condition_variable& cv,
                               std::unique_lock<std::mutex>& lk);


template<typename Predicate>
void interruptible_wait(std::condition_variable& cv,
                        std::unique_lock<std::mutex>& lk,
                        Predicate pred)
{
    interruption_point();
    this_thread_interrupt_flag.set_condition_variable(cv);
    clear_cv_on_destruct guard;
    while (!this_thread_interrupt_flag.is_set() && !pred())
    {
        cv.wait_for(lk, std::chrono::milliseconds(1));
    }
    interruption_point();
}

template<typename Lockable>
void interruptible_wait(std::condition_variable_any& cv, Lockable& lk) {
    this_thread_interrupt_flag.wait(cv, lk);
}

template<typename T>
void interruptible_wait(std::future<T>& uf)
{
    while (!this_thread_interrupt_flag.is_set())
    {
        if (uf.wait_for(std::chrono::milliseconds(1)) ==
            std::future_status::ready)
            break;
    }
    interruption_point();
}


thread_local interrupt_flag this_thread_interrupt_flag;
void interruptible_wait(std::condition_variable& cv,
                        std::unique_lock<std::mutex>& lk)
{
    interruption_point();
    this_thread_interrupt_flag.set_condition_variable(cv);
    clear_cv_on_destruct guard;
    interruption_point();
    //当在执行interrup的时候 就会执行唤醒cv
    cv.wait_for(lk, std::chrono::milliseconds(1));
//    //也可以通过检查是否set来解开wait
//    cv.wait(lk,[](){
//        return this_thread_interrupt_flag.is_set();
//    });
    interruption_point();
}


//如果已经中断，就抛出异常
void interruption_point()
{
    if (this_thread_interrupt_flag.is_set())
    {
        throw thread_interrupted();
    }
}

clear_cv_on_destruct:: ~clear_cv_on_destruct() {
    this_thread_interrupt_flag.clear_condition_variable();
    this_thread_interrupt_flag.clear_condition_variable_any();
}

#endif __INTERUPTHREAD_H__
