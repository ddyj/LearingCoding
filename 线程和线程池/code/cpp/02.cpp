#include<iostream>
#include<thread>
#include <functional> 
#include<memory>


using namespace std;


void do_some_work(){
    while(true){
        std::cout << " this thread id is "<<this_thread::get_id()<<std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
void do_some_other_work(){
    while(true){
        std::cout << " this thread id is other work "<<this_thread::get_id()<<std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

class joining_thread {
    std::thread  _t;
public:
    joining_thread() noexcept = default;
    template<typename Callable, typename ...  Args>
    explicit  joining_thread(Callable&& func, Args&& ...args):
        _t(std::forward<Callable>(func),  std::forward<Args>(args)...){}
    explicit joining_thread(std::thread  t) noexcept: _t(std::move(t)){}
    joining_thread(joining_thread&& other) noexcept: _t(std::move(other._t)){}

    joining_thread& operator=(joining_thread&& other) noexcept
    {
        //如果当前线程可汇合，则汇合等待线程完成再赋值
        if (joinable()) {
            join();
        }
        _t = std::move(other._t);
        return *this;
    }
    //下面代码会和上面构造产生重复，但是作为线程嫁接，被嫁接的线程应该是右值
    // joining_thread& operator=( joining_thread other) noexcept
    // {
    //     //如果当前线程可汇合，则汇合等待线程完成再赋值
    //     if (joinable()) {
    //         join();
    //     }
    //     _t = std::move(other._t);
    //     return *this;
    // }
    ~joining_thread() noexcept {
        if (joinable()) {
            join();
        }
    }
    void swap(joining_thread& other) noexcept {
        _t.swap(other._t);
    }
    std::thread::id   get_id() const noexcept {
        return _t.get_id();
    }
    bool joinable() const noexcept {
        return _t.joinable();
    }
    void join() {
        _t.join();
    }
    void detach() {
        _t.detach();
    }
    std::thread& as_thread() noexcept {
        return _t;
    }
    const std::thread& as_thread() const noexcept {
        return _t;
    }
};

void thread_work_seq(int max_index){
    for(int i=0;i<max_index;i++){
        std::cout << "this thread id is "<<this_thread::get_id() <<std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int main(){
    // thread t1 = std::thread(do_some_work);    
    // thread t2 = std::move(t1);

    // //此时t1无效 t1可以再绑定一个函数
    // t1 = std::thread(do_some_other_work);   
    // std::this_thread::sleep_for(std::chrono::seconds(2000));

    int max_index = 10;
    auto func = std::bind(thread_work_seq,std::placeholders::_1);
 
    joining_thread tj1(std::thread(func,40));
    joining_thread tj2(func,max_index);
    joining_thread tj3(func,max_index);
    tj1 = std::move(tj2);


    std::cout << thread::hardware_concurrency()<<std::endl;



    return 0; 
}