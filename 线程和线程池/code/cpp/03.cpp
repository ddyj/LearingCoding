#include<iostream>
#include<thread>
#include <functional> 
#include<memory>
#include<mutex>
#include<stack>
#include <exception>
#include  <climits>

using namespace std;


int shared_data = 10;
std::mutex mtx;

void thread_word_01(){
    while(true){
        std::lock_guard<std::mutex>lock(mtx);
        shared_data ++;
        std::cout << "this thread id is "<<std::this_thread::get_id()<<" and shared_data is "<<shared_data<<std::endl;
        std::this_thread::yield();
    }
}


struct empty_stack : std::exception
{
    const char* what() const throw() override{
        return "Empty stack exception";
    };
};

template<typename T>
class threadsafe_stack1
{
private:
    std::stack<T> data;
    mutable std::mutex m;
public:
    threadsafe_stack1() {}
    threadsafe_stack1(const threadsafe_stack1& other)
    {
        std::lock_guard<std::mutex> lock(other.m);
        //①在构造函数的函数体（constructor body）内进行复制操作
        data = other.data;
    }
    threadsafe_stack1& operator=(const threadsafe_stack1&) = delete;
    void push(T new_value)
    {
        std::lock_guard<std::mutex> lock(m);
        data.push(std::move(new_value));
    }

    void pop(T& value)
    {
        std::lock_guard<std::mutex> lock(m);
        if(data.empty()){throw empty_stack();}
        value = data.top();
        data.pop();
    }
    shared_ptr<T> pop(){
        std::lock_guard<std::mutex>lock(m);
        if(data.empty()){return nullptr;}
        shared_ptr<T>res  = make_shared<T>(data.top());
        data.pop();
        return res;
    }
    bool empty() const
    {
        std::lock_guard<std::mutex> lock(m);
        return data.empty();
    }
};


//层级锁
class hierarchical_mutex {
public:
    explicit hierarchical_mutex(unsigned long value) :_hierarchy_value(value),
        _previous_hierarchy_value(0) {}
    hierarchical_mutex(const hierarchical_mutex&) = delete;
    hierarchical_mutex& operator=(const hierarchical_mutex&) = delete;
    void lock() {
        check_for_hierarchy_violation();
        _internal_mutex.lock();
        update_hierarchy_value();
    }
    void unlock() {
        if (_this_thread_hierarchy_value != _hierarchy_value) {
            throw std::logic_error("mutex hierarchy violated");
        }
        _this_thread_hierarchy_value = _previous_hierarchy_value;
        _internal_mutex.unlock();
    }
    bool try_lock() {
        check_for_hierarchy_violation();
        if (!_internal_mutex.try_lock()) {
            return false;
        }
        update_hierarchy_value();
        return true;
    }
private:
    std::mutex  _internal_mutex;
    //当前层级值
    unsigned long const _hierarchy_value;
    //上一次层级值
    unsigned long _previous_hierarchy_value;
    //本线程记录的层级值  表示其实权值 同时限制权值的范围
    static thread_local unsigned long  _this_thread_hierarchy_value;
    void check_for_hierarchy_violation() {
        if (_this_thread_hierarchy_value <= _hierarchy_value) {
            throw  std::logic_error("mutex  hierarchy violated");
        }
    }
    void  update_hierarchy_value() {
        _previous_hierarchy_value = _this_thread_hierarchy_value;
        _this_thread_hierarchy_value = _hierarchy_value;
    }
};
// 保证声明的变量在每个线程中都会有自己独立的实例，而不是所有线程共享一个实例。
thread_local unsigned long hierarchical_mutex::_this_thread_hierarchy_value(ULONG_MAX);

int main(){
    // std::thread t1 = thread(thread_word_01);
    // std::thread t2 = thread(thread_word_01);
    // t1.join();
    // t2.join();

    threadsafe_stack1<int>safe_stack;
    safe_stack.push(1024);
    std::thread t1 = thread([&safe_stack](){
        if(!safe_stack.empty()){
            std::this_thread::sleep_for(std::chrono::seconds(1));
            // int res = 0;
            // safe_stack.pop(std::ref(res));
            // std::cout << res <<std::endl;
            shared_ptr<int>ptr_res =safe_stack.pop();
            if(ptr_res!=nullptr){
                std::cout <<"this thread : "<<this_thread::get_id() <<" get pop "<< *ptr_res << std::endl;
            }
            else{
                std::cout <<"this thread : "<<this_thread::get_id()<< "stack empty"<<std::endl;
            }
        }
    });
    std::thread t2 = thread([&safe_stack](){
        if(!safe_stack.empty()){
            std::this_thread::sleep_for(std::chrono::seconds(1));
            // int res = 0;
            // safe_stack.pop(std::ref(res));
            // std::cout << res <<std::endl;
            shared_ptr<int>ptr_res =safe_stack.pop();
            if(ptr_res!=nullptr){
                std::cout <<"this thread : "<<this_thread::get_id()<<
                " get pop res is "<< *ptr_res << std::endl;
            }
            else{
                std::cout <<"this thread : "<<this_thread::get_id()<< "stack empty"<<std::endl;
            }
        }
    });
    t1.join();
    t2.join();



    hierarchical_mutex hmtx1(1000);
    hierarchical_mutex hmtx2(500);
    
    std::thread ht1([&hmtx1,&hmtx2](){
        hmtx1.lock();
        hmtx2.lock();
        hmtx2.unlock();
        hmtx1.unlock();
    });
    std::thread ht2([&hmtx1, &hmtx2]() {
        hmtx2.lock();
        hmtx1.lock();
        hmtx1.unlock();
        hmtx2.unlock();
    });

    ht1.join();
    ht2.join();
    std::unique_lock<std::mutex>lck(mtx);
}