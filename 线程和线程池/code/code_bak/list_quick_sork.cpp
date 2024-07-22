#include<iostream>
#include<list>
#include<vector>
#include <algorithm>
#include <random>
#include<chrono>


#include"author_thread_pool.h"

using namespace std;


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
    std::future<std::list<T>> new_higher =std::async(std::launch::async|std::launch::deferred,&sequential_quick_sort_thread<T>,std::move(higher_part));

    //将new_higher 拼接到result的尾部
    result.splice(result.end(), new_higher.get());    
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
    auto new_higher =ThreadPool::instance().commit(&parallel_quick_sort<T>, std::move(input));
    result.splice(result.end(), new_higher.get());
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
    // std::cout << "sorted result is ";
    // for (auto iter = sort_result.begin(); iter != sort_result.end(); iter++) {
    //     std::cout << " " << (*iter);
    // }
    std::cout << std::endl;
}

template<typename T>
void test_sequential_thread_quick(std::list<T>& numlists) {
    auto start_time = std::chrono::high_resolution_clock::now();
    auto sort_result = sequential_quick_sort_thread(numlists);
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    std::cout << "test_sequential_thread_quick execution time: " << duration.count() << " milliseconds" << std::endl;
    // std::cout << "sorted result is ";
    // for (auto iter = sort_result.begin(); iter != sort_result.end(); iter++) {
    //     std::cout << " " << (*iter);
    // }
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

//速度: test_sequential_quick > test_sequential_thread_quick > test_sequential_threadpool_quick
int main(){
    std::list<int> numlists = create_random_list(10);
    test_sequential_quick(numlists);
    test_sequential_thread_quick(numlists);
    test_sequential_threadpool_quick(numlists);
}