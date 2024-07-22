#ifndef __AUTHOR_THREADSAFE_SATCK_H__
#define __AUTHOR_THREADSAFE_SATCK_H__


#include<iostream>
#include<atomic>
#include<thread>
#include <cassert>
#include<vector>

#include<mutex>
#include<memory>
#include <condition_variable>
#include <stack>
#include<exception>

struct empty_stack : std::exception
{
    const char* what() const throw();
};

template<typename T>
class SafeThreadStack{
private:
    std::stack<T>data;
    mutable std::mutex mtx;
    std::condition_variable condition_empty;
public:
    SafeThreadStack(){}
    SafeThreadStack(const SafeThreadStack& other){
        std::lock_guard<std::mutex>lock(other.mtx);
        data = other.data;
    }
    SafeThreadStack& operator=(const SafeThreadStack&)=delete;
    void push(T &new_val){
        std::lock_guard<std::mutex>lock(mtx);
        data.push(std::move(new_val));
        condition_empty.notify_one();
    }

    std::shared_ptr<T> wait_and_pop(){
        std::unique_lock<std::mutex>lock(mtx);
        condition_empty.wait(lock,[this](){
            return !data.empty();
        });
        std::shared_ptr<T>const res = std::make_shared<T>(std::move(data.top()));
        data.pop();
        return res;
    }
    void wait_and_pop(T& value){
        std::unique_lock<std::mutex>lock(mtx);
        condition_empty.wait(lock,[this](){
            return !data.empty();
        });
        value = std::move(data.top());
        data.pop();
    }
    std::shared_ptr<T>pop(){
        std::lock_guard<std::mutex>lock(mtx);
        if(data.empty()) {return nullptr;}
        std::shared_ptr<T> s_ptr= std::make_shared<T>(data.top());
        data.pop();
        return s_ptr;
    }
    void pop(T& val){
        std::lock_guard<std::mutex>lock(mtx);
        if(data.empty())   throw empty_stack();
        val = data.top();
        data.pop();
    }
    bool empty() const
    {
        std::lock_guard<std::mutex> lock(mtx);
        return data.empty();
    }
};


template <typename T>
class SafeThreadStackWithoutMutex{
private:
    struct StackNode{
        std::shared_ptr<T> data;
        StackNode* next;
        StackNode(T const& data_): data(std::make_shared<T>(data_)){}
    };
    std::atomic<StackNode*>head;
    std::atomic<StackNode*>to_be_deleted;
    std::atomic<int>threads_in_pop;
public:
    SafeThreadStackWithoutMutex(){
        head = nullptr;
        to_be_deleted = nullptr;
        threads_in_pop = 0;
    };
    SafeThreadStackWithoutMutex(const SafeThreadStackWithoutMutex&) =delete;
    SafeThreadStackWithoutMutex& operator=(const SafeThreadStackWithoutMutex&) = delete;

    void push(const T & value){
        StackNode* const new_node = new StackNode(value);
        do{
            new_node->next = head.load(std::memory_order_acquire);
        }
        //compare_exchange_weak 当比较失败的时候不会让exchange的值赋值给compare的值 并且开销也小
        while(!head.compare_exchange_weak(new_node->next,new_node));
    }

    std::shared_ptr<T>pop(){
        ++threads_in_pop;
        StackNode* old_node = nullptr;
        do{
            old_node = head.load(std::memory_order_relaxed);
            if(old_node == nullptr){
                --threads_in_pop;
                return nullptr;
            }
        }while (!head.compare_exchange_weak(old_node,old_node->next));

        std::shared_ptr<T> res;
        if(old_node!= nullptr){
            res.swap(old_node->data);
        }
        try_reclaim(old_node);
        return res;
    }

    void try_reclaim(StackNode* node){
        if(threads_in_pop == 1){
            StackNode* nodes_to_delete = to_be_deleted.exchange(nullptr);
            //如果只有一个线程加入在执行 这时就可以把所有带删除队列都删除掉
            if(--threads_in_pop == 0){
                delete_nodes(nodes_to_delete);
            }
                //此时删除队列还是多线程队列的或本来就没有任务 (另一个线程读取为空的 可能是exchange后的) 就把他放回删除队列
            else if(nodes_to_delete!= nullptr ){
                chain_pending_nodes(nodes_to_delete);
            }
            delete node;
        }
        else{
            //如果多个线程在pop就把这个节点放入删除队列中
            chain_pending_node(node);
            --threads_in_pop;
        }
    }
    static void delete_nodes(StackNode* nodes)
    {
        while (nodes!=nullptr)
        {
            StackNode* next = nodes->next;
            delete nodes;
            nodes = next;
        }
    }
    void chain_pending_node(StackNode* n)
    {
        chain_pending_nodes(n, n);
    }

    void chain_pending_nodes(StackNode* first, StackNode* last)
    {
        //1 先将last的next节点更新为待删列表的首节点
        last->next = to_be_deleted;
        //2  借循环保证 last->next指向正确
        // 将待删列表的首节点更新为first节点
        while (!to_be_deleted.compare_exchange_weak(
                last->next, first));
    }

    void chain_pending_nodes(StackNode* nodes)
    {
        StackNode* last = nodes;
        //1 沿着next指针前进到链表末端
        while (StackNode* const next = last->next)
        {
            last = next;
        }
        //2 将链表放入待删链表中
        chain_pending_nodes(nodes, last);
    }
};
#endif