# 前言
在面试中有这么一个问题，说出C++的常见的锁类型，这里根据笔记做一个归纳



# 自旋锁
对于自旋锁因为是不断进行轮询阻塞，但是比较轻量化，易于实现但是针对与复杂的场景不容易处理(长时间占用CPU)

实现 使用原子变量实现
```c++
class SpinLock {
public:
    SpinLock() : flag_(false)
    {}
    void lock()
    {
        bool expect = false;
        /*
            a.compare_exchange_weak(b,c)
            a==b return true a=c
            a!=b return false b=a
        */
        while (!flag_.compare_exchange_weak(expect, true))
        {
            //这里一定要将expect复原，执行失败时expect结果是未定的
            expect = false;
        }
    }
    void unlock()
    {
        flag_.store(false);
    }
private:
    std::atomic<bool> flag_;
};
```
flag等于false表示没上锁，获取锁的时候while循环会让falg变为ture，循环结束
当上锁后另一个线程发出获取请求，此时就会应为flag!=expect让循环死等

# 互斥锁
这就是mutex了
对于这个来讲,就一些细节问题：
1. lock_guard和unique_lock 的区别
lock_guard和unqiue_lock都是针对范围的，在作用域范围内实现上锁，离开作用于释放。但是区别是unique_lock支持deffer的延迟上锁和自动释放锁unlock。但是lock_guard则是声明直接上锁，功能少于unique_lock，但是性能上较好一点

2. 有unique_lock，有shared_lock吗
有 这个是在读写锁实现的

# 读写锁
诚然我们是知道读写锁是放宽了读的锁的条件，但是是怎么实现的？
是通过shared_mutex和shared_lock实现的
```c++
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
```

# 层级锁
层级锁是我们自己实现的一个class，保证了当前锁一定按照某一种顺序添加的，这种方式保证在多锁条件下线程上锁顺序性。

```c++
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
    void update_hierarchy_value() {
        _previous_hierarchy_value = _this_thread_hierarchy_value;
        _this_thread_hierarchy_value = _hierarchy_value;
    }
};
// 保证声明的变量在每个线程中都会有自己独立的实例，而不是所有线程共享一个实例。
thread_local unsigned long hierarchical_mutex::_this_thread_hierarchy_value(ULONG_MAX);
```
```c++
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
```

# 递归锁
在线程任务中，如果出现一fun内调用另一个fun，但两个fun都用一把锁上锁，此时用互斥锁就会死锁。针对这种场景，C++提供了递归锁recursive_mutex
```c++
胖成猪还吃  10:55:50
class recursive_lock_test {
private:
	std::recursive_mutex r_mtx;
public:
	void fun1() {
		std::lock_guard<std::recursive_mutex>r_f1(r_mtx);
		std::cout << "in func1 " << "\n";
	}
	void fun2() {
		std::lock_guard<std::recursive_mutex>r_f1(r_mtx);
		std::cout << "in func2 before func1" << "\n";
		fun1();
		std::cout << "in func2 after func1" << "\n";
	}
};
void test_recursive_mutex() {
	recursive_lock_test rlt1;
	auto func_rec1 = std::bind(&recursive_lock_test::fun1, &rlt1);
	auto func_rec2 = std::bind(&recursive_lock_test::fun2, &rlt1);
	std::thread t1 = std::thread(std::move(func_rec1));
	std::thread t2 = std::thread(std::move(func_rec2));
	t1.join();
	t2.join();
}
```

# 信号量
信号量本质上是内核态的计数器，sem库
```c++
#include <semaphore.h>
class sem
{
public:
    sem()
    {
        if (sem_init(&m_sem, 0, 0) != 0)
        {
            throw std::exception();
        }
    }
    // 第三个参数(num)为信号量的初始值。
    // 如果一个信号量的初始值设置为N，那么在没有其他线程调用post的情况下，最多允许N个线程通过wait操作而不被阻塞。
    sem(int num)
    {
        if (sem_init(&m_sem, 0, num) != 0)
        {
            throw std::exception();
        }
    }
    ~sem()
    {
        sem_destroy(&m_sem);
    }
    bool wait()
    {
        return sem_wait(&m_sem) == 0;
    }
    bool post()
    {
        return sem_post(&m_sem) == 0;
    }

private:
    sem_t m_sem;
};
```
sem库实现的sem_wait\sem_pose的pv操作、sem_init注册sem_destroy销毁
调用时候上锁用wait 解锁用post