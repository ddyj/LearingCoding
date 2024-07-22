#ifndef __AUTHOR_THREAD_CIRCLE_QUE_H__
#define __AUTHOR_THREAD_CIRCLE_QUE_H__

#include<thread>
#include<memory>
#include<atomic>
#include<iostream>

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
        //使用while循环 避免当多线程竞争的时候，导致_tail的不同步，保证多线程进入的时候只有一个线程能从这个循环退出
        do{
            t = _tail.load(std::memory_order_acquire);
            if((t+1)%_max_size == _head.load(std::memory_order_acquire)){
                std::cout <<"circle que is full "<<std::endl;
                return false;
            }
        }while(!_tail.compare_exchange_strong (t,(t+1)%_max_size,
                                            std::memory_order_release,std::memory_order_release));
        //如果上述while循环能跳出 此时tail就已经指向下一位空缺位置了，而t和指向当前插入位置
       
        _data[t] = val;
        size_t tailup;

        /*
            按push的逻辑 是tail先移动，在更新data[t]
            如果在没来得及更新的情况下
            如果此时有别的线程读取head值，就会读到覆盖之前的值
            所以额外增加_tail_update检查尾部的数据是是否是最新的
            如果不是就要pop返回false
            此时始终保证_tail_update是插入位置的下一位
        */
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
            val = _data[h]; 
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
        增加另一个原子变量_tail_update来标记当前指向位置数据是否修改完成，
        如果尾部数据没有修改完成，此时其他线程pop时获取的数据就是不安全的，
        所以pop要返回false。
    */
    std::atomic<size_t>_tail_update;
};


/*
Q:为什么引入了_tail_update 
A:_tail_update保证如果多线程执行 一个执行pop 一个执行push 的情况下  
pop中val = _data[h]; 始终执行在push的_data[t] = val;之后
*/

#endif