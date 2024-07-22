#ifndef __AUTHOR_THREADSAFE_QUE_H__
#define __AUTHOR_THREADSAFE_QUE_H__


#include <iostream>
#include <atomic>
#include <thread>
#include <cassert>
#include <vector>
#include <functional>
#include <mutex>
#include <memory>
#include <condition_variable>
#include <queue>
#include <exception>


struct empty_queue : std::exception{
    const char* what() const throw();
};
template<typename T>
class SafeThreadQue{
private:
    std::queue<T>data;
    mutable std::mutex mtx;
    std::condition_variable condition_empty;
public:
    SafeThreadQue(){};
    SafeThreadQue(const SafeThreadQue& other){
        std::lock_guard<std::mutex>lock(other.mtx);
        data = other.data;
    };

    void push(const T& new_val){
        std::lock_guard<std::mutex>lock(mtx);
        data.push(new_val);
        condition_empty.notify_one();
    }

    std::shared_ptr<T> wait_and_pop(){
        std::unique_lock<std::mutex>lock(mtx);
        condition_empty.wait(lock,[this](){
            return !data.empty();
        });
        std::shared_ptr<T>const res = std::make_shared<T>(std::move(data.front()));
        data.pop();
        return res;
    }
    void wait_and_pop(T& value){
        std::unique_lock<std::mutex>lock(mtx);
        condition_empty.wait(lock,[this](){
            return !data.empty();
        });
        value = std::move(data.front());
        data.pop();
    }
    std::shared_ptr<T>pop(){
        std::lock_guard<std::mutex>lock(mtx);
        if(data.empty()) {return nullptr;}
        std::shared_ptr<T> s_ptr= std::make_shared<T>(data.front());
        data.pop();
        return s_ptr;
    }
    void pop(T& val){
        std::lock_guard<std::mutex>lock(mtx);
        if(data.empty())   throw empty_queue();
        val = data.front();
        data.pop();
    }
    bool empty() const
    {
        std::lock_guard<std::mutex> lock(mtx);
        return data.empty();
    }
};





//使用智能指针复制 降低开销
//使用list来替换
/*
开始创建一个虚节点 head和tail 都指向他
当有数据push时  tail节点更新data的智能指针 并把next指向new node，并把tail向后退一步
当有数据pop时 判断head是否和tail相同 -- 是否数据为空 
    然后pop 更新head 分别按照传参和智能指针返回
*/
template<typename T>
class threadsafe_queue_ht{
private:
    struct node
    {
        std::shared_ptr<T>data;
        std::unique_ptr<node>next;
    };
    std::mutex head_mutex;
    std::unique_ptr<node>head;
    std::mutex tail_mutex;
    node* tail;
    std::condition_variable data_cond;

    std::atomic<bool>_bstop;

    node* get_tail(){
        std::lock_guard<std::mutex>tail_lock(tail_mutex);
        return tail;
    }
    std::unique_ptr<node>pop_head(){
        //这里使用move模拟pop
        std::unique_ptr<node>old_head = std::move(head);
        //这里使用move移交 放置pop返回的值可以访问到list中的内容
        head = std::move(old_head->next);
        return old_head;
    }
    std::unique_lock<std::mutex> wait_for_data(){
        std::unique_lock<std::mutex> head_lock(head_mutex);
        //不为空
        data_cond.wait(head_lock,[&] {
                return (head.get() != get_tail() || _bstop.load() == true);
        });
        return std::move(head_lock);   
    }
    std::unique_ptr<node> wait_pop_head()
    {
        //条件锁
        std::unique_lock<std::mutex> head_lock(wait_for_data());
        if(_bstop.load() == true){return nullptr;}
        return pop_head();
    }
    std::unique_ptr<node> wait_pop_head(T& value)
    {
        std::unique_lock<std::mutex> head_lock(wait_for_data());
        if(_bstop.load() == true){return nullptr;}
        value = std::move(*head->data);
        return pop_head();
    }
    std::unique_ptr<node> try_pop_head()
    {
        std::lock_guard<std::mutex> head_lock(head_mutex);
        if (head.get() == get_tail() || _bstop.load() == true)
        {
            return std::unique_ptr<node>();
        }
        return pop_head();
    }
    std::unique_ptr<node> try_pop_head(T& value)
    {
        std::lock_guard<std::mutex> head_lock(head_mutex);
        if (head.get() == get_tail() || _bstop.load() == true)
        {
            return std::unique_ptr<node>();
        }
        value = std::move(*head->data);
        return pop_head();
    }
public:
    //在构造的时候就声明一个空节点
    threadsafe_queue_ht() : head(new node),tail(head.get()),_bstop(false){};
    threadsafe_queue_ht(const threadsafe_queue_ht& other) = delete;
    threadsafe_queue_ht& operator=(const threadsafe_queue_ht& other) = delete;
    threadsafe_queue_ht& operator=(const threadsafe_queue_ht& other) volatile = delete;
    void notify_stop(){
        _bstop = true;
        data_cond.notify_one();
    }
    std::shared_ptr<T>wait_and_pop(){
        std::unique_ptr<node>const old_head = wait_pop_head();
        if(old_head == nullptr) {
            return std::shared_ptr<T>();
        }
        //这里就不能调用old_head->next了 以及被move了
        return old_head->data;
    }
    void wait_and_pop(T& value){
        std::unique_ptr<node>const old_head = wait_pop_head(value);
    }

    std::shared_ptr<T> try_pop()
    {
        std::unique_ptr<node> old_head = try_pop_head();
        return old_head ? old_head->data : std::shared_ptr<T>();
    }
    bool try_pop(T& value)
    {
        std::unique_ptr<node> const old_head = try_pop_head(value);
        return old_head != nullptr;
    }
    void push(T new_value)  
    {
        if(_bstop.load() == true){return;}
        std::shared_ptr<T> new_data(
                std::make_shared<T>(std::move(new_value)));
        std::unique_ptr<node> p(new node);
        node* const new_tail = p.get();

        std::lock_guard<std::mutex> tail_lock(tail_mutex);
        tail->data = new_data;
        tail->next = std::move(p);
        tail = new_tail;
        data_cond.notify_one();
    }
    bool empty()
    {
        std::lock_guard<std::mutex> head_lock(head_mutex);
        return (head.get() == get_tail());
    }
};
#endif