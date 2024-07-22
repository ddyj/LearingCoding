#include<iostream>
#include<thread>
#include <functional> 
#include<memory>

class thread_bind_test{
    public:
        void run(const std::string str){
             std::cout << "this class is in thread use bind "<<std::this_thread::get_id() 
                <<" print str is "<<str <<std::endl;
        }
};
class thread_work_2{
    public:
        void operator()(const std::string str){
            std::cout << "this class is in thread "<<std::this_thread::get_id() 
                <<" print str is "<<str <<std::endl;
        }
};
class thread_guard{
    private:
        std::thread &_t;
    public:
        //这里传输的必须是右值
        explicit thread_guard(std::thread &t) : _t(t){};
        ~thread_guard(){
            if(_t.joinable()){
                _t.join();
            }
        }
        
        //禁止复制构造函数和拷贝构造函数
        thread_guard(thread_guard const&) = delete;
        thread_guard& operator=(thread_guard const&) = delete;
};


void thread_work(std::string str){
    std::cout << "this is in thread "<<std::this_thread::get_id() 
            <<" print str is "<<str <<std::endl;
}
void thread_work_ref(int &a){
    a = a+15;
}
void thread_work_3(std::unique_ptr<int>ptr){
    std::cout << "unique ptr data is " << *ptr << std::endl;
    (*ptr)++;
    std::cout << "after unique ptr data is " << *ptr << std::endl;
}


int main(){
    std::string hellow_str = "hellow str";
    std::thread t1(thread_work,hellow_str);
    t1.join();
    std::thread t2(thread_work_2(),hellow_str);
    t2.join();
    std::thread t3([](const std::string str){
        std::cout << "this lamda is in thread "<<std::this_thread::get_id() 
            <<" print str is "<<str <<std::endl;
    },hellow_str);
    t3.join();
    thread_bind_test base_;
    auto fun_t4 = std::bind(&thread_bind_test::run,&base_,std::placeholders::_1);
    std::thread t4(fun_t4,hellow_str);

    std::shared_ptr<thread_guard>g_ptr = std::make_shared<thread_guard>(std::ref(t4));
    thread_guard * g_t = g_ptr.get();
    
    
    std::cout << "thread_guard finished "<<std::endl;
    g_ptr.reset();
    
    int t_5 = 10;
    std::cout << "in main thread :"<<std::this_thread::get_id() << " before t_5 is "<<t_5<<std::endl;
    std::thread t5(thread_work_ref,std::ref(t_5));
    t5.join();
    std::cout << "in main thread :"<<std::this_thread::get_id() << " after t_5 is "<<t_5<<std::endl;


    auto p_tr = std::make_unique<int>(100);
    std::thread t6(thread_work_3,std::move(p_tr));
    t6.join();
    //此处会带来错误
    //std::cout << "after unique ptr data is " << *p_tr << std::endl;  

    return 0;
}