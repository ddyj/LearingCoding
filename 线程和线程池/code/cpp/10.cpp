#include<iostream>
#include<atomic>
#include<thread>
#include <cassert>
#include<vector>

#include<mutex>
#include<memory>


using namespace std;


template<typename T,size_t Cap>
class circle_que_mutex{
public:
    //std::allocator<T>::allocator(_max_size) 分配空间大小
    circle_que_mutex():_max_size(Cap+1),_data(std::allocator<T>().allocate(_max_size)),_head(0),_tail(0){}
    circle_que_mutex(const circle_que_mutex&)=delete;
    circle_que_mutex& operator=(const circle_que_mutex&) volatile = delete;
    circle_que_mutex& operator=(const circle_que_mutex&) = delete;
    ~circle_que_mutex(){
        std::lock_guard<std::mutex>lock(_mtx);
        while(_head!=_tail){
            //调用destroy方法
            std::allocator<T>().destroy(_data + _head);
            _head = (_head+1)%_max_size;
        }
        //调用回收操作 -- 释放由 allocate() 方法分配的内存空间
        std::allocator<T>().deallocate(_data, _max_size);
    }


    template<typename ...Args>
    bool emplace(Args && ...args){
        std::lock_guard<std::mutex>lock(_mtx);
        //判断队列是否满了
        if((_tail + 1 )% _max_size == _head){
            std::cout << "circle que full ! "<<std::endl;
            return false;
        }
        //在尾部构造一个T类型对象 构造参数为args...
        std::allocator<T>().construct(_data+_tail,std::forward<Args>(args)...);
        _tail = (_tail + 1 )& _max_size;
        return true;
    }

    //push 实现两个版本 一个实现左值 一个实现右值
    bool push(const T&val){
        std::cout << "called push const T& version "<<std::endl;
        return emplace(std::move(val));
    }
    bool push(const T&&val){
        std::cout << "called push const T&& version "<<std::endl;
        return emplace(std::move(val));
    }
    //pop 实现出队列
    bool pop(T& val){
        std::lock_guard<std::mutex>lock(_mtx);
        //判断头部和尾部指针是否重合，重合则表示队列为空
        if(_head == _tail){
            std::cout << "circular que empty! "<<std::endl;
            return false;
        }
        //取出头部
        val = std::move(_data[_head]);
        //更新head指针
        _head = (_head+1)%_max_size;
        return true;
    }

private:
    size_t _max_size;
    T* _data;
    std::mutex _mtx;
    size_t _head = 0;
    size_t _tail = 0;
};


class MyClass{
private:
    int _val;
public:
    MyClass(){}
    MyClass(int val) : _val(val){
        std::cout << "MyClass "<<_val <<"is created" << std::endl;
    }
    ~MyClass(){
        //std::cout << "MyClass is delete" << std::endl;
    }
    inline int get_val() const{
        return _val;
    }
};

void test_circle_que_use_mutex(){
    circle_que_mutex<MyClass,4>cq_lk;
    for(int i=0;i<5;++i){
        MyClass mc(i);
        auto res = cq_lk.push(mc);
        if(!res){
            std::cout << "circle que is full "<<std::endl;
            break;
        }
    }
    for(int i=0;i<5;++i){
        MyClass mc;
        auto res = cq_lk.pop(mc);
        if(!res){
            std::cout << "circle que is full "<<std::endl;
            break;
        }
        std::cout << "pop sucess "<< mc.get_val() << std::endl;
    }
}


template<typename T,size_t Cap>
class circle_que_atomic{
public:
    circle_que_atomic():_max_size(Cap+1),
                    _data(std::allocator<T>().allocate(_max_size)),
                    _atomic_using(false),
                    _head(0),
                    _tail(0){}
    circle_que_atomic(const circle_que_atomic&) = delete;
    circle_que_atomic& operator=(const circle_que_atomic&) volatile = delete;
    circle_que_atomic& operator=(const circle_que_atomic&)= delete;
    ~circle_que_atomic(){
        //循环销毁
        bool use_expected = false;
        bool use_desire = true;
        //这里是实现了一个自旋锁  
        /*
            _atomic_using为false 和 use_expected一致 
            compare_exchange_strong 返回true 并将_atomic_using改为use_desire(true)
            而在_atomic_using为true的时候，返回为false，这里会一直阻塞
        */
        do{
            use_expected = false;
            use_desire = true;
        }
        while(!_atomic_using.compare_exchange_strong(use_expected,use_desire));
        
        //调用元素内部的析构函数
        while(_head != _tail){
            std::allocator<T>().destroy(_data + _head);
            _head = (_head + 1)% _max_size;
        }
        //调用收回
        std::allocator<T>().deallocate(_data,_max_size);

        //释放自旋锁
        do{
            use_expected = true;
            use_desire = false;
        }
        while(!_atomic_using.compare_exchange_strong(use_expected,use_desire));
    }

    template<typename ...Args>
    bool emplace(Args ...args){
        bool use_expected = false;
        bool use_desire = true;
        do{
            use_expected = false;
            use_desire = true;
        }
        while(!_atomic_using.compare_exchange_strong(use_expected,use_desire));

        //判断队列是否满了
        if((_tail + 1)% _max_size == _head){
            std::cout << "circle que is full !"<<std::endl;
            do{
                use_expected = true;
                use_desire = false;
            }
            while(!_atomic_using.compare_exchange_strong(use_expected,use_desire));
            return false;
        }
        std::allocator<T>().construct(_data + _tail,std::forward<Args>(args)...);
        _tail = (_tail+1)%_max_size;

        do{
            use_expected = true;
            use_desire = false;
        }
        while(!_atomic_using.compare_exchange_strong(use_expected,use_desire));
        return true;
    }

    bool push(const T&val){
        return emplace(val);
    }
    bool push(const T&&val){
        return emplace(std::move(val));
    }

    bool pop(T& val){
        bool use_expected = false;
        bool use_desire = true;
        do{
            use_expected = false;
            use_desire = true;
        }
        while(!_atomic_using.compare_exchange_strong(use_expected,use_desire));

        //判断是否队列空
        if(_head == _tail){
            std::cout << "circle que is empty "<<std::endl;
            do{
                use_expected = true;
                use_desire = false;
            }
            while(!_atomic_using.compare_exchange_strong(use_expected,use_desire));
            return false;
        }

        val = std::move(_data[_head]);
        _head = (_head + 1)% _max_size;
        do{
            use_expected = true;
            use_desire = false;
        }
        while(!_atomic_using.compare_exchange_strong(use_expected,use_desire));
        return true;
    }

private:
    size_t _max_size;
    T* _data;
    std::atomic<bool>_atomic_using;
    size_t _head = 0;
    size_t _tail = 0;
};
void test_circle_que_use_atomic(){
    circle_que_atomic<MyClass,5>cq_lk;
    for(int i=0;i<5;++i){
        MyClass mc(i);
        auto res = cq_lk.push(mc);
        if(!res){
            std::cout << "circle que is full "<<std::endl;
            break;
        }
    }
    for(int i=0;i<5;++i){
        MyClass mc;
        auto res = cq_lk.pop(mc);
        if(!res){
            std::cout << "circle que is full "<<std::endl;
            break;
        }
        std::cout << "pop sucess "<< mc.get_val() << std::endl;
    }
}




template<typename T,size_t Cap>
class circle_que_thread_atomic{
public:
    circle_que_thread_atomic():_max_size(Cap+1),
                                                            _data(std::allocator<T>().allocate(_max_size)),
                                                            _head(0),_tail(0),_tail_update(0){}
    circle_que_thread_atomic(const circle_que_thread_atomic&)=delete;
    circle_que_thread_atomic &operator=(const circle_que_thread_atomic&) volatile = delete;
    circle_que_thread_atomic &operator=(const circle_que_thread_atomic&)  = delete;

    ~circle_que_thread_atomic(){
        while(_head!=_tail){
            std::allocator<T>().destroy(_data+_head);
            _head = (_head +1)%_max_size;
        }
        std::allocator<T>().deallocate(_data,_max_size);
    }

    bool push(const T&val){
        size_t t;
        //使用while循环 避免当多线程竞争的时候，导致_tail的不同步
        do{
            t = _tail.load(std::memory_order_acquire);
            if((t+1)%_max_size == _head.load(std::memory_order_acquire)){
                std::cout <<"circle que is full "<<std::endl;
                return false;
            }
        }while(!_tail.compare_exchange_strong (t,(t+1)%_max_size,
                                            std::memory_order_release,std::memory_order_release));
        //如果上述while循环能跳出 此时tail就已经指向下一位空缺位置了
        _data[t] = val;
        size_t tailup;
        do{
            tailup = t;
        }while(_tail_update.compare_exchange_strong(tailup,(tailup+1)%_max_size,
                                             std::memory_order_release,std::memory_order_release));
        return true;
    }

    bool pop(T& val){
        size_t h;
        do{
            h = _head.load(std::memory_order_relaxed);
            if(h == _tail.load(std::memory_order_acquire)){
                std::cout << "circular que empty ! " << std::endl;
                return false;
            }

             //判断如果此时要读取的数据和tail_update是否一致，如果一致说明尾部数据未更新完
            if (h == _tail_update.load(std::memory_order_acquire)) 
            {
                return false;
            }
            val = _data[h]; // 2处
        }while(!_head.compare_exchange_strong(h,(h+1)%_max_size,
                             std::memory_order_release,std::memory_order_release));
        return true;
    }

private:
    size_t _max_size;
    T* _data;
    std::atomic<size_t>_head;
    std::atomic<size_t>_tail;
    /*
    增加另一个原子变量_tail_update来标记尾部数据是否修改完成，
    如果尾部数据没有修改完成，此时其他线程pop时获取的数据就是不安全的，
    所以pop要返回false。
    */
    std::atomic<size_t>_tail_update;
};

void test_circle_que_use_thread_atomic(){
    circle_que_thread_atomic<MyClass,5>cq_lk;
    for(int i=0;i<5;++i){
        MyClass mc(i);
        auto res = cq_lk.push(mc);
        if(!res){
            std::cout << "circle que is full "<<std::endl;
            break;
        }
    }
    for(int i=0;i<5;++i){
        MyClass mc;
        auto res = cq_lk.pop(mc);
        if(!res){
            std::cout << "circle que is full "<<std::endl;
            break;
        }
        std::cout << "pop sucess "<< mc.get_val() << std::endl;
    }
}
int main(){
    //test_circle_que_use_mutex();
    //test_circle_que_use_atomic();
    test_circle_que_use_thread_atomic();
}