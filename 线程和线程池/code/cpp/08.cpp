#include <iostream>
#include <queue>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <functional>
#include <future>

template <typename T>
class Channel {
private:
    std::queue<T> queue_;
    std::mutex mtx_;
    std::condition_variable cv_producer_;
    std::condition_variable cv_consumer_;
    size_t capacity_;
    bool closed_ = false;
public:
    Channel(size_t capacity = 0) : capacity_(capacity) {}
    ~Channel(){
        close();
    }
    bool send(T value) {
        std::unique_lock<std::mutex> lock(mtx_);
        cv_producer_.wait(lock, [this]() {
            // 对于无缓冲的channel，我们应该等待直到有消费者准备好
            return (capacity_ == 0 && queue_.empty()) || queue_.size() < capacity_ || closed_;
        });
        if (closed_) {
            return false;
        }
        queue_.push(value);
        cv_consumer_.notify_one();
        return true;
    }
    bool receive(T& value) {
        std::unique_lock<std::mutex> lock(mtx_);
        cv_consumer_.wait(lock, [this]() { return !queue_.empty() || closed_; });
        if (closed_ && queue_.empty()) {
            return false;
        }
        value = queue_.front();
        queue_.pop();
        cv_producer_.notify_one();
        return true;
    }
    void close() {
        std::unique_lock<std::mutex> lock(mtx_);
        closed_ = true;
        cv_producer_.notify_all();
        cv_consumer_.notify_all();
    }
};




// 示例使用
void test_csp() {
    Channel<int> ch(10);  // 10缓冲的channel
    std::thread producer([&]() {
        for (int i = 0; i < 15; ++i) {
            ch.send(i);
            printf("send %d\n",i);
        }
        });
    std::thread producer2([&]() {
        for (int i = 15; i < 30; ++i) {
            ch.send(i);
            printf("send %d\n",i);
        }
        });
    std::thread consumer([&]() {
        //std::this_thread::sleep_for(std::chrono::milliseconds(500)); // 故意延迟消费者开始消费
        int val;
        while (ch.receive(val)) {
            printf("receive %d\n",val);
        }
    });
   
    producer.join();
    producer2.join();
    consumer.join();
}



template<typename Func,typename ...Args>
auto ParallenExe(Func&&func,Args &&...args)->std::future<decltype(func(args...))>{
    typedef decltype(func(args...)) ReType;
    std::function<ReType()>  bind_func = std::bind(std::forward<Func>(func), std::forward<Args>(args)...);
    std::packaged_task<ReType()>task(bind_func);
    auto rt_future = task.get_future();
    std::thread t(std::move(task));
    t.detach();

    return rt_future;
}

template<class T>
void get_wait(T& rt_future){
    rt_future.wait();
    return;
}
void test_pall(){
    std::cout << "start test pall"<<std::endl;
    int i = 0;
    auto rt_future = ParallenExe([](int i){
        while (i<3)
        {
            i++;
            std::cout << "ParllenExe thread func " << i << " times" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    },i);
    std::cout <<"end test pall"<<std::endl;
    rt_future.wait();
    // get_wait(rt_future);
}

int main(){
    //test_csp();
    test_pall();
    return 0;
}