//
// Created by dai on 2024/4/1.
//

#include <thread>
#include <list>
#include <future>
#include <memory>
#include <vector>
#include <algorithm>
#include <random>
#include "author_threadsafe_satck.h"
#include "author_thread_pool.h"
#include "author_threadsafe_que.h"


template<typename T>
class sorter{
private:
    struct chunk_to_sort{
        std::list<T>data;
        std::promise<std::list<T>> promise;
        chunk_to_sort() : data(), promise() {}
        // 移动构造函数和移动赋值运算符
        chunk_to_sort(chunk_to_sort&& other) noexcept
                : data(std::move(other.data)), promise(std::move(other.promise)) {}

        chunk_to_sort& operator=(chunk_to_sort&& other) noexcept {
            if (this != &other) {
                data = std::move(other.data);
                promise = std::move(other.promise);
            }
            return *this;
        }
    };
    SafeThreadStack<chunk_to_sort>chunks;
    std::vector<std::thread>threads;
    unsigned max_thread_count;
    std::atomic<bool> end_of_data;
public:
    sorter(){
        max_thread_count = std::thread::hardware_concurrency()-1;
        end_of_data = false;
    }
    ~sorter()
    {
        end_of_data = true;
        for (unsigned i = 0; i < threads.size(); ++i)
        {
            threads[i].join();
        }
    }
    void try_sort_chunk(){
        std::shared_ptr<chunk_to_sort>chunk = chunks.try_pop();
        if(chunk!= nullptr){
            sort_chunk(chunk);
        }
    }
    void sort_chunk(std::shared_ptr<chunk_to_sort> const &chunk){
        chunk->promise.set_value(do_sort(chunk->data));
    }
    void sort_thread(){
        while(!end_of_data){
            try_sort_chunk();
            std::this_thread::yield();
        }
    }

    std::list<T> do_sort(std::list<T>&data){
        if(data.empty()){
            return data;
        }
        std::list<T>result;
        //将data中第一个值移动到result中
        result.splice(result.begin(),data,data.begin());
        T const & partition_val = *result.begin();
        //std::partition 将满足条件的节点放到不满足val的结点之前
        typename std::list<T>::iterator divide_point =
                    std::partition(data.begin(),data.end(),[&](T const& val){
                        return val < partition_val;
                    });
        chunk_to_sort new_lower_chunk;
        new_lower_chunk.data.splice(new_lower_chunk.data.end(),data,data.begin(),divide_point);
        std::future<std::list<T>>new_lower = new_lower_chunk.promise.get_future();
        chunks.push(std::move(new_lower_chunk));
        //限制了任务执行的线程的数量
        if(threads.size() < max_thread_count){
            threads.emplace_back(std::thread(&sorter<T>::sort_thread,this));
        }
        std::list<T>new_higher(do_sort(data));
        result.splice(result.end(),new_higher);
        while(new_lower.wait_for(std::chrono::seconds(0))!= std::future_status::ready){
            //如果本线程排好了自己的半区，会去辅助栈内其他任务来处理数据，避免了盲等的问题
            try_sort_chunk();
        }
        result.splice(result.begin(),new_lower.get());
        return result;
    }
};

/*
1.splice() 针对list
result.splice(result.begin(), input, input.begin(),input.end());
将input从begin 到end 全部拷贝到 result.begin的位置

2.partion
auto divide_point = std::partition(input.begin(), input.end(),
    [&](int const& t) { return t < pivot; });
对于input从开始到结束 将满足lamda表达式的元素放到前面
返回第一个不满足lamda表达式的元素的迭代器
*/
std::list<int> create_random_list(const int &list_len){
    std::random_device rd;
    std::mt19937 gen(rd()); // 使用 Mersenne Twister 算法

    // 创建一个分布器
    std::uniform_int_distribution<int> dis(1, 1000); // 假设随机数范围为 1 到 1000

    // 创建一个空列表
    std::list<int> random_list;

    // 生成长度为 100000 的随机数列表
    for (int i = 0; i < list_len; ++i) {
        random_list.push_back(dis(gen)); // 将随机数插入列表尾部
    }
    return random_list;
}

template<typename T>
std::list<T> sequential_quick_sort(std::list<T> input)
{
    if (input.empty())
    {
        return input;
    }
    std::list<T> result;
    result.splice(result.begin(), input, input.begin());
    T const& pivot = *result.begin();
    //  std::partition 是一个标准库函数，用于将容器或数组中的元素按照指定的条件进行分区，
    // 使得满足条件的元素排在不满足条件的元素之前。
    // 所以经过计算divide_point指向的是input中第一个大于等于pivot的元素
    auto divide_point = std::partition(input.begin(), input.end(),
                                       [&](T const& t) {return t < pivot; });
    std::list<T> lower_part;
    lower_part.splice(lower_part.end(), input, input.begin(),
                      divide_point);

    //lower_part 中都是小于divide_point的值
    auto new_lower(
            sequential_quick_sort(std::move(lower_part)));

    auto new_higher(
            sequential_quick_sort(std::move(input)));

    //将new_higher 拼接到result的尾部
    result.splice(result.end(), new_higher);
    //将new_lower 拼接到result的头部
    result.splice(result.begin(), new_lower);
    return result;
}


template<typename T>
std::list<T> sequential_quick_sort_thread(std::list<T> input)
{
    if (input.empty())
    {
        return input;
    }
    std::list<T> result;
    result.splice(result.begin(), input, input.begin());
    T const& pivot = *result.begin();
    auto divide_point = std::partition(input.begin(), input.end(),
                                       [&](T const& t) {return t < pivot; });
    std::list<T> lower_part;
    lower_part.splice(lower_part.end(), input, input.begin(),
                      divide_point);
    std::list<T> higher_part;
    higher_part.splice(higher_part.end(), input, divide_point,input.end());

    //改进 使用async修改
    std::future<std::list<T>> new_lower = std::async(std::launch::async|std::launch::deferred,&sequential_quick_sort_thread<T>,std::move(lower_part));
    std::list<T> new_higher = sequential_quick_sort_thread(std::move(higher_part));

    //将new_higher 拼接到result的尾部
    result.splice(result.end(), new_higher);
    //将new_lower 拼接到result的头部
    result.splice(result.begin(), new_lower.get());
    return result;
}


// 并发快速排序函数
template<typename T>
std::list<T> parallel_quick_sort(std::list<T> input) {
    if (input.empty()) {
        return input;
    }
    std::list<T> result;
    result.splice(result.begin(), input, input.begin());
    T const& pivot = *result.begin();
    auto divide_point = std::partition(input.begin(), input.end(), [&](T const& t) { return t < pivot; });
    std::list<T> lower_part;
    lower_part.splice(lower_part.end(), input, input.begin(), divide_point);
    auto new_lower = ThreadPool::instance().commit(&parallel_quick_sort<T>, std::move(lower_part));
    ThreadPool::instance().wait_for_empty();
    auto new_higher = parallel_quick_sort(move(input));
    result.splice(result.end(), new_higher);
    result.splice(result.begin(), new_lower.get());
    return result;
}


template<typename T>
void test_sequential_quick(std::list<T>& numlists) {
    auto start_time = std::chrono::high_resolution_clock::now();
    auto sort_result = sequential_quick_sort(numlists);
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    std::cout << "test_sequential_quick execution time: " << duration.count() << " milliseconds" << std::endl;
    std::cout << "sorted result is ";
    for (auto iter = sort_result.begin(); iter != sort_result.end(); iter++) {
     std::cout << " " << (*iter);
    }
    std::cout << std::endl;
}

template<typename T>
void test_sequential_thread_quick(std::list<T>& numlists) {
    auto start_time = std::chrono::high_resolution_clock::now();
    auto sort_result = sequential_quick_sort_thread(numlists);
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    std::cout << "test_sequential_thread_quick execution time: " << duration.count() << " milliseconds" << std::endl;
    std::cout << "sorted result is ";
    for (auto iter = sort_result.begin(); iter != sort_result.end(); iter++) {
     std::cout << " " << (*iter);
    }
    std::cout << std::endl;
}

template<typename T>
void test_sequential_threadpool_quick(std::list<T>& numlists) {

    auto start_time = std::chrono::high_resolution_clock::now();
    auto sort_result = parallel_quick_sort(numlists);
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    std::cout << "test_sequential_threadpool_quick execution time: " << duration.count() << " milliseconds" << std::endl;
    std::cout << "sorted result is ";
    for (auto iter = sort_result.begin(); iter != sort_result.end(); iter++) {
        std::cout << " " << (*iter);
    }
    std::cout << std::endl;
}


template <typename T>
void test_sequential_threadstacks_quick(std::list<T>& numlists){
    auto start_time = std::chrono::high_resolution_clock::now();
    sorter<T>s;
    std::list<T>ans = s.do_sort(numlists);
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    std::cout << "test_sequential_thread_quick execution time: " << duration.count() << " milliseconds" << "\n";
    std::cout << "sorted result is ";
    for (auto iter = ans.begin(); iter != ans.end(); iter++) {
        std::cout << " " << (*iter);
    }
    std::cout << std::endl;
}

//----------------------------------------------------------------
//-------------------------按任务划分-------------------------------
//----------------------------------------------------------------





template <typename ClassType,typename QueType>
class ActorSingle{
public:
    static ClassType& instance(){
        static ClassType as;
        return as;
    }
    ~ActorSingle(){

    }
    void PostMsg(const QueType& data){
        _que.push(data);
    }

protected:
    ActorSingle():_bstop(false){
    }
    ActorSingle(const ActorSingle&) = delete;
    ActorSingle(ActorSingle&&) = delete;
    ActorSingle& operator = (const ActorSingle&) = delete;


    std::atomic<bool> _bstop;
    threadsafe_queue_ht<QueType>  _que;
    std::thread _thread;
};



struct MsgClassC {
    std::string name;
    friend std::ostream& operator << (std::ostream& os, const MsgClassC& ca) {
        os << ca.name;
        return os;
    }
};
class ClassC : public ActorSingle<ClassC, MsgClassC> {
    friend class ActorSingle<ClassC, MsgClassC>;
public:
    ~ClassC() {
        _bstop = true;
        _que.notify_stop();
        _thread.join();
        std::cout << "ClassC destruct " << std::endl;
    }
    void DealMsg(std::shared_ptr<MsgClassC> data) {
        std::cout << "class C deal msg is " << *data << std::endl;
}
private:
    ClassC(){
        _thread = std::thread([this]() {
            std::cout << "thread C " << std::this_thread::get_id() << std::endl;
            for (; (_bstop.load() == false);) {
                std::shared_ptr<MsgClassC> data = _que.wait_and_pop();
                if (data == nullptr) {
                    continue;
                }
                DealMsg(data);
            }
            std::cout << "ClassC thread exit " << std::endl;
        });
    }
};
struct MsgClassB {
    std::string name;
    friend std::ostream& operator << (std::ostream& os, const MsgClassB& ca) {
        os << ca.name;
        return os;
    }
};
class ClassB : public ActorSingle<ClassB, MsgClassB> {
    friend class ActorSingle<ClassB, MsgClassB>;
public:
    ~ClassB() {
        _bstop = true;
        _que.notify_stop();
        _thread.join();
        std::cout << "ClassB destruct " << std::endl;
    }
    void DealMsg(std::shared_ptr<MsgClassB> data) {
        std::cout << "class B deal msg is " << *data << std::endl;
        MsgClassC msga;
        msga.name = "llfc_C";
        ClassC::instance().PostMsg(msga);
    }
private:
    ClassB(){
        _thread = std::thread([this]() {
            std::cout << "thread B " << std::this_thread::get_id() << std::endl;
            for (; (_bstop.load() == false);) {
                std::shared_ptr<MsgClassB> data = _que.wait_and_pop();
                if (data == nullptr) {
                    continue;
                }
                DealMsg(data);
            }
            std::cout << "ClassB thread exit " << std::endl;
        });
    }
};
struct MsgClassA {
    std::string name;
    friend std::ostream& operator << (std::ostream& os, const MsgClassA& ca) {
        os << ca.name;
        return os;
    }
};
class ClassA : public ActorSingle<ClassA, MsgClassA> {
    friend class ActorSingle<ClassA, MsgClassA>;
public:
    ~ClassA() {
        _bstop = true;
        _que.notify_stop();
        _thread.join();
        std::cout << "ClassA destruct " << std::endl;
    }
    void DealMsg(std::shared_ptr<MsgClassA> data) {
        std::cout << "class A deal msg is " << *data << std::endl;
        MsgClassB msga;
        msga.name = "llfc_B";
        ClassB::instance().PostMsg(msga);
    }
private:
    ClassA(){
        _thread = std::thread([this]() {
            for (; (_bstop.load() == false);) {
                std::cout << "thread A " << std::this_thread::get_id() << std::endl;
                std::shared_ptr<MsgClassA> data = _que.wait_and_pop();
                if (data == nullptr) {
                    continue;
                }
                DealMsg(data);
            }
            std::cout << "ClassA thread exit " << std::endl;
        });
    }
};

void test_ABC(){
    MsgClassA msga;
    msga.name = "llfc_A";
    ClassA::instance().PostMsg(msga);

    std::this_thread::sleep_for(std::chrono::seconds(5));
    std::cout << "main process exited!\n";
}


int main(){
    std::list<int> numlists_1 = create_random_list(10);
    test_sequential_quick(numlists_1);
    std::list<int> numlists_2 = create_random_list(10);
    test_sequential_thread_quick(numlists_2);
    //线程池这版有问题
//    std::list<int> numlists_3 = create_random_list(10);
//    test_sequential_threadpool_quick(numlists_3);
    std::list<int> numlists_4 = create_random_list(10);
    test_sequential_threadstacks_quick(numlists_4);

    test_ABC();
    return 0;
}