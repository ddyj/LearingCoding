#include<iostream>
#include<future>
#include<chrono>
#include<memory>
#include<mutex>
#include<stack>
#include<queue>
#include <exception>
#include  <climits>
#include <shared_mutex>
#include <condition_variable>


#include "author_thread_pool.h"

using namespace std;

// 定义一个异步任务
std::string fetchDataFromDB(std::string query) {
    std::this_thread::sleep_for(std::chrono::seconds(5));
    return "Data: " + query;
}

int my_task(int val){
    std::this_thread::sleep_for(std::chrono::seconds(5));
    std::cout << "my task run 5s "<<std::endl;
    return 42;
}
void use_package(){
    //int()表示返回了一个无参的int类型 
    //int(int)表示my_task含有一个参数 然后在创建线程的时候传入参数
    std::packaged_task<int(int)>task(my_task);
    std::future<int>result = task.get_future();
    //这里最好使用move转移成右值
    std::thread t(std::move(task),10);
    t.detach();
    //此处还是会阻塞主线程等待任务完成
    int value = result.get();
    std::cout << "result is "<<value<<std::endl;
}

void set_value(std::promise<int>prom){
    prom.set_value(10);
}
void use_promise(){
    std::promise<int>prom;
    std::future<int>fut = prom.get_future();
    std::thread t(set_value,std::move(prom));
    std::cout << "value set by the thread :" << fut.get()<<std::endl;
    t.join();
}


void set_exception(std::promise<void>prom){
    try
    {
        //抛出一个异常
       throw std::runtime_error("An error occurred!");
    }
    catch(const std::exception& e)
    {
        //使用current_exception获取try中抛出异常的错误
        prom.set_exception(std::current_exception());
    }
}

void use_promise_exception(){
    std::promise<void>prom;
    std::future<void>fut = prom.get_future();
    std::thread t(set_exception,std::move(prom));
    try
    {
        fut.get();
    }
    catch(const std::exception& e)
    {
        std::cout << "Exception set by thread "<<e.what() << '\n';
    }
    t.join();   
}



void test_async(){
    std::future<std::string>fut = std::async(std::launch::async,fetchDataFromDB,"DataBase");
    std::cout << "Doing something in main ..."<<std::endl;
    if(fut.wait_for(std::chrono::seconds(10)) == std::future_status::ready){
        std::cout <<"process finished "<<std::endl;
    }
    else{
        std::cout << "process not finished"<<std::endl;
    }
    std::cout <<fut.get()<<std::endl;
}





void set_shared_value(promise<int>prom){
    std::this_thread::sleep_for(std::chrono::seconds(1));
    int cur_prom = 110;
    prom.set_value(cur_prom);
}
template<typename T>
void get_shared(std::shared_future<T>fut,std::promise<T>res){
    try
    {
        res.set_value(fut.get());
    }
    catch(const std::future_error &e)
    {
        std::cerr << e.what() << '\n';
    }
}
void test_promise_with2thread(){
    std::promise<int>prom;
    std::shared_future<int>fut = prom.get_future();
    std::thread t1(set_shared_value,std::move(prom));
    t1.join();

    std::promise<int>res_1,res_2;
    std::future<int>f_1 = res_1.get_future();
    std::future<int>f_2 = res_2.get_future();
    std::thread t2(get_shared<int>,fut,std::move(res_1));
    std::thread t3(get_shared<int>,fut,std::move(res_2));

    std::cout << f_1.get() << std::endl;
    std::cout << f_2.get() << std::endl;
    t2.join();
    t3.join();
}

template<typename T>
T test_async_promise_work(T input_value){
    //get res 
    input_value.set_value(30);
    return input_value;
}   
void test_async_promise(){
    auto func = std::bind(test_async_promise_work<std::promise<int>>,std::placeholders::_1);
    std::promise<int> cal_thread;
    std::future<std::promise<int>> fut_p = std::async(std::launch::async,func,std::move(cal_thread));
    auto ans = std::move(fut_p.get());
    std::future<int> cal_thread_fu = ans.get_future();
    std::cout << "Thread cal is "<<cal_thread_fu.get()<<std::endl;
}






void test_pool_func(int i){
    std::this_thread::sleep_for(std::chrono::seconds(2));
    printf("this thread process %d\n",i);
}
void test_thread_pool(){
    ThreadPool& pool = ThreadPool::instance(10);
    auto func = std::bind(test_pool_func,std::placeholders::_1);
    for(int i=0;i<20;++i){
        pool.commit(func,i);
    }
    
    for(int i=25;i<45;++i){
        pool.commit(func,i);
    }
    //可以加 也可以不加 用这个接口可以手动调节
    pool.wait_for_empty();
}

int main() {
    test_async();
    use_package();
    use_promise();
    use_promise_exception();
    test_promise_with2thread();
    test_async_promise();
    test_thread_pool();
    return 0;
}
