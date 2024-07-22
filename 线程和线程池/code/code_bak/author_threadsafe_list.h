#ifndef __AUTHOR_THREADSAFE_LIST_H__
#define __AUTHOR_THREADSAFE_LIST_H__


#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <list>

//定义一个源节点
template<typename T>
class threadsafe_list{
private:
    struct node{
        std::mutex mtx;
        std::shared_ptr<T> data;
        std::unique_ptr<node> next;
        //next() unique_ptr默认构造
        node() : next(){}
        node(T const& val):
                data(std::make_shared<T>(val)){};
    };

    node head;
    node* last_node;
    std::mutex last_mtx;
    //尾部指针 用来实现尾插

public:
    threadsafe_list(){
        last_node = &head;
    };
    ~threadsafe_list(){
        remove_if([](const T& value) {return true; });
    }
    threadsafe_list(const threadsafe_list& other) = delete;
    threadsafe_list&operator=(const threadsafe_list) = delete;

    //头插 向头结点(虚节点)后插入一个元素
    void push_front(T const &val){
        std::unique_ptr<node> new_node(new node(val));
        std::lock_guard<std::mutex>lck(head.mtx);
        new_node->next = std::move(head.next);
        head.next = std::move(new_node);

        if(head.next->next == nullptr){
            std::lock_guard<std::mutex>lck(last_mtx);
            last_node  = head.next.get();
        }
    }
    //尾插 向尾部节点后插入一个元素
    void push_back(const T& val){
        //防止和头插同时进行
        std::unique_ptr<node>next_node(new node(val));
        std::lock(last_node->mtx,last_mtx);
        std::lock_guard<std::mutex>last_nck(last_node->mtx,std::adopt_lock);
        std::lock_guard<std::mutex>last_ck(last_mtx,std::adopt_lock);
        last_node->next = std::move(next_node);
        last_node = last_node->next.get();

    }
    template <typename Function>
    void for_each(Function f){
        node* current = &head;
        std::unique_lock<std::mutex>lck(head.mtx);
        //node* const next 常量指针 指向对象可变 但是指向对象的地址不能改变
        while(node* const next = current->next.get()){
            std::unique_lock<std::mutex>next_lk(next->mtx);
            lck.unlock();
            f(*next->data);
            current = next;
            lck = std::move(next_lk);
        }
    }
    //查询
    template <typename Prediect>
    std::shared_ptr<T>find_first_if(Prediect p){
        node* current = &head;
        std::unique_lock<std::mutex>lck(head.mtx);
        while(node* const next = current->next.get()){
            std::unique_lock<std::mutex>next_lck(next->mtx);
            next_lck.lock();
            if(p(*next->data)){
                return next->data;
            }
            current = next;
            next_lck.unlock();
        }
        return std::shared_ptr<T>();
    }
    //删除
    template <typename Prediect>
    void remove_if(Prediect p){
        node* current = &head;
        std::unique_lock<std::mutex>lck(head.mtx);
        while(node* const next = current->next.get()){
            std::unique_lock<std::mutex>next_lck(next->mtx);
            if(p(*next->data)){
                std::unique_ptr<node>old_next = std::move(current->next);
                current->next = std::move(next->next);
                if(current->next == nullptr){
                    std::unique_lock<std::mutex>last_lck(last_mtx);
                    last_node = current;
                }
                next_lck.unlock();
            }
            current = next;
            lck.unlock();
            lck = std::move(next_lck);
        }
    }
    //删除 包含返回值的删除
    template <typename Prediect>
    bool remove_first(Prediect p){
        node* current = &head;
        std::unique_lock<std::mutex>lck(head.mtx);
        while(node* const next = current->next.get()){
            std::unique_lock<std::mutex>next_lck(next->mtx);
            if(p(*next->data)){
                std::unique_ptr<node>old_next = std::move(current->next);
                current->next = std::move(next->next);
                if(current->next == nullptr){
                    std::unique_lock<std::mutex>last_lck(last_mtx);
                    last_node = current;
                }
                next_lck.unlock();
                return true;
            }
            current = next;
            lck.unlock();
            lck = std::move(next_lck);
        }
        return false;
    }
    //插入
    template<typename Prediect>
    void insert_if(Prediect f,const T& value){
        node* current = &head;
        std::unique_lock<std::mutex>lck(head.mtx);
        while(node* next = current->next.get()){
            std::unique_lock<std::mutex>next_lck(next->mtx);
            if(f(*next->data)){
                std::unique_ptr<node>add_node(new node(value));
                auto old_next = std::move(current->next);
                add_node->next = std::move(old_next);
                current->next = std::move(add_node);
                return;
            }
            current = next;
            lck.unlock();
            lck = std::move(next_lck);
        }
    }
};
#endif