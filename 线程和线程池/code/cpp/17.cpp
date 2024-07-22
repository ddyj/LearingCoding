//
// Created by dai on 2024/4/8.
//

#include "author_threadsafe_que.h"
#include "interupthread.h"

class join_threads{
private:
    std::vector<std::thread>& threads;
public:
    join_threads(std::vector<std::thread>& _threads) : threads(_threads){}
    ~join_threads(){
        for(int i=0;i<threads.size();++i){
            if(threads[i].joinable()){
                threads[i].join();
            }
        }
    }
};
//在实现线程池后 我们这里基于多线程并发队列实现轮询线程池
class simple_thread_pool{
private:
    std::atomic_bool done;
    threadsafe_queue_ht<std::function<void()>> work_queue;
    std::vector<std::thread>threads;
public:
    simple_thread_pool():done(false) {
        unsigned const thread_count = 1;
        try{
            for(unsigned i = 0; i < thread_count;++i){
                threads.emplace_back(std::thread(&simple_thread_pool::worker_thread,this));
            }
        }
        catch(std::exception){
            done = true;
            throw;
        }
    }
    void worker_thread(){
        //线程轮询逻辑:不断从队列中获取任务，如果无法获取 就让当前线程放弃时间片
        std::function<void(void)>task;
        while(!done){
            if(work_queue.try_pop(task)){
                task();
            }
            else{
                std::this_thread::yield();
            }
        }
        // 处理线程池被销毁时的情况
        while(work_queue.try_pop(task)){
            task();
        }
    }
    template<class Function>
    void submit(Function f){
        work_queue.push(std::move(std::function<void()>(f)));
    }

    ~simple_thread_pool(){
        done = true;
        for(auto& thread : threads) {
            if (thread.joinable()) {
                thread.join();
            }
        }
    }
};


void test_simple_thread_pool(){
    simple_thread_pool pool;
    for(int i=0;i<10;++i){
        pool.submit([i](){
            std::cout << "Lambda task " << i << " running on thread " << std::this_thread::get_id() << std::endl;
        });
    }
}



std::vector<interruptible_thread> background_threads;
std::mutex mtx1;
std::mutex mtx2;
std::condition_variable cv1;
std::condition_variable_any cv2;
void start_background_processing() {
    int count = 17;
    background_threads.push_back(interruptible_thread([](int count) {
        try {
            std::cout << "count is "<<count <<std::endl;
            std::unique_lock<std::mutex> lock(mtx1);
            interruptible_wait(cv1, lock);
        }
        catch (std::exception& ex) {
            std::cout << "catch exception is " << ex.what() << std::endl;
        }
    },count));
    ++count;
    background_threads.push_back(interruptible_thread([](int count) {
        try {
            std::cout << "count is "<<count <<std::endl;
            std::unique_lock<std::mutex> lock(mtx2);
            this_thread_interrupt_flag.wait(cv2, mtx2);
        }
        catch (std::exception& ex) {
            std::cout << "catch exception is " << ex.what() << std::endl;
        }
    },count));
}

void test_interupthread()
{
    start_background_processing();
    for (unsigned i = 0; i < background_threads.size(); i++) {
        background_threads[i].interrupt();
    }
    for (unsigned i = 0; i < background_threads.size(); i++) {
        background_threads[i].join();
    }
}

void reference_sharedptr()
{
    class task_data {
    public:
        task_data(int i) :_data(new int(i)) {}
        ~task_data() { delete _data; }
        int* _data;
    };
    std::queue<std::function<void()>> task_que;
    for (int i = 0; i < 10; i++) {
        std::shared_ptr<task_data> taskptr = std::make_shared<task_data>(i);
        task_que.push([taskptr]() {
            (*( taskptr->_data))++;
            std::cout << "data is " << *(taskptr->_data) << std::endl;
        });
    }
    auto res_future = std::async([&task_que]() {
        for (;;) {
            if (task_que.empty()) {
                break;
            }
            auto& task = task_que.front();
            task();
            task_que.pop();
        }
    });
    res_future.wait();
}
void normal_copy() {
    class task_data {
    public:
        task_data(int i) :_data(new int(i)) {}
        ~task_data() {
            std::cout << "call task_data destruct" << std::endl;
            delete _data;
        }
        task_data(const task_data& src) {
            if(this->_data == src._data){return;}
            _data = new int(*(src._data));
        }
        task_data(task_data&& src) {
            if(this->_data == src._data){return;}
            _data = new int(*(src._data));
        }
        task_data& operator=(const task_data& other){
            if(this != &other){
                delete _data;
                _data = new int(*(other._data));
            }
            return *this;
        }
        int* _data;
    };
    auto task_call = []() -> task_data {
        task_data data(100);
        return data;
    };
    task_call();
}
int main(){
    test_simple_thread_pool();
    test_interupthread();
    reference_sharedptr();
    normal_copy();
    return 0;
}