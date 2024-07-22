#include<iostream>
#include<thread>
#include <functional> 
#include<memory>
#include<mutex>
#include<stack>
#include <exception>
#include  <climits>
#include <shared_mutex>


using namespace std;

std::mutex mtx;
int shared_data = 10;

std::unique_lock <std::mutex> && get_lock() {
    std::unique_lock<std::mutex>  lock(mtx);
    shared_data++;
    std::cout<<"cur shared_data is "<<shared_data << std::endl;
    return std::move(lock);
}
void use_return() {
    std::unique_lock<std::mutex> lock(get_lock());
    shared_data++;
    std::cout<<"cur shared_data is "<<shared_data << std::endl;
}

class thread_guard{
    private:
        thread _t;
    public:
        template<typename _Callable, typename... _Args>
        explicit thread_guard(_Callable&& __f, _Args&&... __args){
            _t = thread(std::forward<_Callable>(__f),std::forward< _Args>( __args)...);
        }
        explicit thread_guard(thread && t1): _t(std::move(t1)){};
        ~thread_guard(){
            if(_t.joinable()){
                _t.join();
            }
        }
};


class read_write{
    private:
      mutable std::shared_mutex _shared_mutex;
    public:
        read_write(){}
        void read_shared_data(){
            while(true){
                std::shared_lock<std::shared_mutex>_shared_lock(_shared_mutex);
                std::cout << "this thread is "<<this_thread::get_id() << " read shared_data "<<shared_data<<std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
        }
        void write_shared_data(){
            while (true)
            {
                std::unique_lock<std::shared_mutex>_unique_lock(_shared_mutex);
                shared_data++;
                std::cout << "this thread is "<<this_thread::get_id() << " write shared_data "<<shared_data<<std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(1));
             }       
        }
};

class recursive_lock_test{
    private:
        std::recursive_mutex r_mtx;
    public:
        void fun1(){
            std::lock_guard<std::recursive_mutex>r_f1(r_mtx);
            std::cout << "in func1 "<<"\n";
        }
        void fun2(){
            std::lock_guard<std::recursive_mutex>r_f1(r_mtx);
            std::cout << "in func2 before func1"<<"\n";
            fun1();
            std::cout << "in func2 after func1"<<"\n";
        }
};


int main(){
    // try
    // {
    //     thread_guard gt_1(use_return);
    // }
    // catch(const std::exception& e)
    // {
    //     std::thread t1 = thread(use_return);
    //     thread_guard gt_1(std::move(t1));
    // }
    
    // read_write rw_1;
    // auto func_Read = std::bind(&read_write::read_shared_data,&rw_1);
    // auto func_Write = std::bind(&read_write::write_shared_data,&rw_1);
    // std::thread t1 = std::thread(func_Read);
    // std::thread t2 = std::thread(func_Read);
    // std::thread t3 = std::thread(func_Write);

    // thread_guard gt_1(std::move(t1));
    // thread_guard gt_2(std::move(t2));
    // thread_guard gt_3(std::move(t3));
    
    recursive_lock_test rlt1;
    auto func_rec1 = std::bind(&recursive_lock_test::fun1, &rlt1); 
    auto func_rec2 = std::bind(&recursive_lock_test::fun2, &rlt1); 
    thread_guard gt_1(func_rec1);
    thread_guard gt_2(func_rec2);
    return 0;
}
