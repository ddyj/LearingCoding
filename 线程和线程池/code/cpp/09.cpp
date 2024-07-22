#include<iostream>
#include<atomic>
#include<thread>
#include <cassert>
#include<vector>

#include<mutex>
#include<memory>

using namespace std;


//memory_order_relaxed 允许乱序执行，提高并发性能。
//std::memory_order_acquire 和 std::memory_order_release
//适用于具有读取和写入依赖关系的场景，确保正确的数据同步。

std::atomic<int> my_data(0);
std::atomic<bool> ready(false);
void writer_thread(){
    my_data.store(42,std::memory_order_relaxed);
    ready.store(true,std::memory_order_acquire);
}
void reader_thread(){
    while(!ready.load(std::memory_order_release)){
        std::cout << "Waiting for ready load..."<<std::endl;
    }
    int value = my_data.load(std::memory_order_relaxed);
    std::cout << "Value read by reader thread: " << value << std::endl;
}
void test_acq_rel(){
    std::thread writer(writer_thread);
    std::thread reader(reader_thread);
    writer.join();
    reader.join();
}


//std::memory_order_seq_cst：提供最严格的内存同步语义，适用于需要确保所有原子操作按顺序执行的场景。
std::atomic<int> my_data2(0);
void writer_2() {
    my_data2.store(92, std::memory_order_seq_cst);
}
void reader_2() {
    int value = my_data2.load(std::memory_order_seq_cst);
    std::cout << "Value read by reader thread: " << value << std::endl;
}
void test_orderd_seq(){
    std::thread writer2(writer_2);
    std::thread reader2(reader_2);
    writer2.join();
    reader2.join();
}


class SpinLock{
public:
    void lock(){
        //自旋等待，直到成功获取到锁
        while(flag.test_and_set(std::memory_order_acquire)){
            //在循环内 flag被test_and_set设置为true 返回false
        };   
    }
    void unlock(){
        flag.clear(std::memory_order_release);
    }
private:
    std::atomic_flag flag= ATOMIC_FLAG_INIT; //0
};
void test_spinlock(){
    SpinLock slock;
    std::thread t1([&slock](){
        slock.lock();
        for (int i = 0; i < 3; i++) {
            std::cout << "*";
            }
        std::cout << std::endl;
        slock.unlock();
    });
    std::thread t2([&slock](){
        slock.lock();
        for (int i = 0; i < 3; i++) {
            std::cout << "?";
            }
        std::cout << std::endl;
        slock.unlock();
    });

    t1.join();
    t2.join();
}   



std::atomic<bool> x, y;
std::atomic<int> z;
void write_x_then_y(){
    x.store(true,std::memory_order_relaxed);
    y.store(true,std::memory_order_relaxed);
}
void read_y_then_x(){
    while(!y.load(std::memory_order_relaxed)){
        std::cout<<" load y false "<<std::endl;
    }
    if(x.load(std::memory_order_relaxed)){
        ++z;
    }
}
void test_order_relaxed(){
    x = false;
    y = false;
    z = 0;
    std::thread t1(write_x_then_y);
    std::thread t2(read_y_then_x);
    t1.join();
    t2.join();
    assert(z.load()!=0);
}

void TestOderRelaxed2() {
    std::atomic<int> a{ 0 };
    std::vector<int> v3, v4;
        std::thread t1([&a]() {
            for (int i = 0; i < 10; i += 2) {
                a.store(i, std::memory_order_relaxed);
            }    
        });
        std::thread t2([&a]() {
            for (int i = 1; i < 10; i += 2)
                a.store(i, std::memory_order_relaxed);
            });
        std::thread t3([&v3, &a]() {
            for (int i = 0; i < 10; ++i)
                v3.push_back(a.load(std::memory_order_relaxed));
            });
        std::thread t4([&v4, &a]() {
            for (int i = 0; i < 10; ++i)
                v4.push_back(a.load(std::memory_order_relaxed));
            });
        t1.join();
        t2.join(); 
        t3.join(); 
        t4.join();
        for (int i : v3) {
            std::cout << i << " ";
        }
        std::cout << std::endl;
        for (int i : v4) {
            std::cout << i << " ";
        }
        std::cout << std::endl;
}




class SingleAutoRefine
{
private:
    SingleAutoRefine()
    {
    }
    SingleAutoRefine(const SingleAutoRefine&) = delete;
    SingleAutoRefine& operator=(const SingleAutoRefine&) = delete;
public:
    ~SingleAutoRefine()
    {
        std::cout << "single auto delete success " << std::endl;
    }
    static std::shared_ptr<SingleAutoRefine> GetInst()
    {
        if (_is_init.load(std::memory_order_acquire))
        {
            return single;
        }
        s_mutex.lock();
        if (_is_init.load(std::memory_order_acquire))
        {
            s_mutex.unlock();
            return single;
        }
        //这里不能使用make_shared,make_shared会出现无法查找ctor函数
        single = std::shared_ptr<SingleAutoRefine>(new SingleAutoRefine);
        _is_init.store(true,std::memory_order_release);
        s_mutex.unlock();
        return single;
    }
private:
    static std::shared_ptr<SingleAutoRefine> single;
    static std::mutex s_mutex;
    static std::atomic<bool> _is_init;
};
std::shared_ptr<SingleAutoRefine> SingleAutoRefine::single = nullptr;
std::mutex SingleAutoRefine::s_mutex;
std::atomic<bool> SingleAutoRefine::_is_init(false);


void testSingle_refine(){
    std::thread t1([](){
         std::cout << "thread t1 singleton address is 0x: " << SingleAutoRefine::GetInst() << std::endl;
    });
    std::thread t2([](){
         std::cout << "thread t2 singleton address is 0x: " << SingleAutoRefine::GetInst() << std::endl;
    });
    t1.join();
    t2.join();
}

int main(){
    test_acq_rel();
    test_orderd_seq();

    test_spinlock();
    test_order_relaxed();
    TestOderRelaxed2();

    testSingle_refine();
    return 0;
}