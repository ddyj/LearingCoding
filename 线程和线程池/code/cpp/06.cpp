#include<iostream>
#include<thread>
#include <functional> 
#include<memory>
#include<mutex>
#include<stack>
#include<queue>
#include <exception>
#include  <climits>
#include <shared_mutex>
#include <condition_variable>
using namespace std;


void condition_test_AB(){
    std::mutex mtx;
    std::condition_variable data_A;
    std::condition_variable data_B;
    int num = 1;
    std::thread t1([&](){
        for(;;){
            std::unique_lock<std::mutex>lck(mtx);
            data_A.wait(lck,[&](){
                return num == 1;
            });
            num++;
            printf("thread A add num\n");
            data_B.notify_one();
        }
    });
    std::thread t2([&](){
        for(;;){
            std::unique_lock<std::mutex>lck(mtx);
            data_B.wait(lck,[&](){
                return num == 2;
            });
            num--;
            printf("thread B mins num\n");
            data_A.notify_one();
        }
    });

    t1.join();
    t2.join();
}

template<typename T>
class thread_safe_queue{
    private:
        std::mutex mtx;
        int max_size = 1000;
        std::condition_variable is_full;
        std::condition_variable is_empty;
        std::queue<T>data_queue;

        thread_safe_queue(){};
        thread_safe_queue(thread_safe_queue&) = delete;
        thread_safe_queue& operator=(const thread_safe_queue&) = delete;
    public:
        static thread_safe_queue& getInst(){
            static thread_safe_queue ts_q;
            return ts_q;
        };

        void push(const T& val){
            std::unique_lock<std::mutex>lck(mtx);
            is_full.wait(lck,[this](){
                return data_queue.size() < max_size;
            });
            data_queue.push(val);
            is_empty.notify_one();
        };
        void pop(shared_ptr<T>&ptr){
            std::unique_lock<std::mutex>lck(mtx);
            is_empty.wait(lck,[this](){
                return !data_queue.empty();
            });
            ptr = std::make_shared<T>(data_queue.front());
            data_queue.pop();
            is_full.notify_one();
        }
};

void test_thread_safe_queue(){
    thread_safe_queue<int>& ts_q = thread_safe_queue<int>::getInst();
    std::thread produer([&ts_q](){
        for(int i=0;;++i){
            ts_q.push(i);
            printf("producer push data %d\n",i);
        }
    });
    std::thread comsuer1([&ts_q](){
        for(;;){
            shared_ptr<int>s_ptr = nullptr;
            ts_q.pop(s_ptr);
            printf("comsuer1 pop data %d\n",*s_ptr);
        }
    });
    std::thread comsuer2([&ts_q](){
        for(;;){
            shared_ptr<int>s_ptr = nullptr;
            ts_q.pop(s_ptr);
            printf("comsuer2 pop data %d\n",*s_ptr);
        }
    });

    produer.join();
    comsuer1.join();
    comsuer2.join();
}


int main(){
    //condition_test_AB();
    test_thread_safe_queue();
    return 0;
}