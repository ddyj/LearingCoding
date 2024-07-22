//
// Created by dai on 2024/4/2.
//

#include <iostream>
#include <thread>
#include <algorithm>
#include <vector>
#include <future>
#include <memory>
#include <atomic>

#include <numeric> // 包含 partial_sum 函数的头文件


template<typename T>
class AddVal{
private:
    T _val;
public :
    AddVal(const T& val):_val(val){}
    void operator()(T& val){
        val += _val;
    }
};
void test_for_each(){
    std::vector<int>coll = {0,1,2,3,4,5,6,7};
    std::for_each(coll.begin(),coll.end(),AddVal<int>(50));
    auto print = [](int i){ std::cout << i << " "; };
    std::for_each(coll.begin(),coll.end(),print);
    std::cout << std::endl;
}


//线程管理类 管理线程避免过早退出
class join_threads{
private:
    std::vector<std::thread>&threads;
public:
    explicit join_threads(std::vector<std::thread>&_threads) : threads(_threads) {};
    ~join_threads(){
        for(int i=0;i<threads.size();++i){
            if(threads[i].joinable()){
                threads[i].join();
            }
        }
    }
};
template<typename Iterator,typename Func>
void parallel_for_each(Iterator first,Iterator last,Func f){
    unsigned long const length = std::distance(first,last);
    if(!length){return;}
    //处理的数量不超过25个则用单线程。否则根据处理的数量划分任务
    unsigned long const min_per_thread = 25;
    //计算一个长度为 length 的任务或数据块分配到多少个线程中比较合适
    unsigned long const max_per_thread = (length + min_per_thread -1 )/min_per_thread;
    unsigned long const hardware_thread = std::thread::hardware_concurrency();
    unsigned long const num_threads = std::min(hardware_thread!=0 ? hardware_thread : 2 ,max_per_thread);
    unsigned long const block_size = length / num_threads;
    std::vector<std::future<void>>futures(num_threads - 1);
    std::vector<std::thread> threads(num_threads - 1);
    join_threads joiner(threads);
    Iterator block_start = first;

    for (unsigned long i = 0; i < (num_threads - 1) ;++i){
        Iterator block_end = block_start;
        std::advance(block_end,block_size);
        std::packaged_task<void(void)>task([=](){
            std::for_each(block_start,block_end,f);
        });
        futures[i] = task.get_future();
        threads[i] = std::thread(std::move(task));
        block_start = block_end;
    }
    //使用num_threads-1个线程并行处理for_each，剩下的主线程处理余下的for_each，最后通过futures.get汇总
    std::for_each(block_start, last, f);
    for(unsigned long i = 0; i <(num_threads - 1); i++){
        futures[i].get();
    }
}

void test_pallar_for_each(){
    std::vector<int>coll = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,
                                17,18,19,20,21,22,23,24,25,26,27,28,29,30,31};
    parallel_for_each(coll.begin(),coll.end(),[=](const int& val ){
        std::cout << val << " ";
    });
    std::cout << std::endl;
    parallel_for_each(coll.begin(),coll.end(),[=](int& val ){
        val += 100;
    });
    parallel_for_each(coll.begin(),coll.end(),[=](const int& val ){
        std::cout << val << " ";
    });
    std::cout << std::endl;
}

template <typename Iterator, typename MatchType>
Iterator parallel_find(Iterator first, Iterator last,MatchType match){
    struct find_element{
        void operator()(Iterator begin,Iterator end,MatchType match,
                        std::promise<Iterator>*result,
                        std::atomic<bool>*done_flag)
        {
                try {
                    for(;(begin!=end)&&!done_flag->load();++begin){
                        if(*begin==match){
                            result->set_value(begin);
                            done_flag->store(true);
                            return;
                        }
                    }
                }
                catch (const std::exception&){
                    try {
                        result->set_exception(std::current_exception());
                        done_flag->store(true);
                    }
                    catch(...){}
                }
        }
    };
    unsigned long const length = std::distance(first, last);
    if (!length)
        return last;
    unsigned long const min_per_thread = 25;
    unsigned long const max_threads = (length + min_per_thread - 1) / min_per_thread;
    unsigned long const hardware_threads = std::thread::hardware_concurrency();
    unsigned long const num_threads = std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);
    unsigned long const block_size = length / num_threads;

    std::promise<Iterator>result;
    std::atomic<bool>done_flag(false);
    std::vector<std::thread>threads(num_threads - 1);
    {
        join_threads joiner(threads);
        Iterator block_start = first;
        for (unsigned long i = 0; i < num_threads - 1 ;++i){
            Iterator block_end = block_start;
            std::advance(block_end, block_size);
            threads[i] = std::thread(find_element(),block_start,block_end,match,&result,&done_flag);
            block_start = block_end;
        }
        find_element()(block_start, last, match, &result, &done_flag);
    }
    //没有找到就返回last
    if(!done_flag.load()){return last;}
    return result.get_future().get();
}


void test_pallar_find(){
    std::vector<int>coll = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,
                            17,18,19,20,21,22,23,24,25,26,27,28,29,30,31};
    auto it = parallel_find(coll.begin(),coll.end(),30);
    if( it == coll.end() ){
        std::cout << "not found" << std::endl;
        return;
    }
    std::cout << std::distance(coll.begin(),it) << std::endl;
}


// 实现并发的 std::partial_sum
template<typename Iterator>
void parallel_partial_sum(Iterator first,Iterator last){
    //获取传入数据的类型
    typedef typename Iterator::value_type value_type;
    struct process_chunk{
        void operator()(Iterator begin,Iterator last,
            std::future<value_type>* previous_end_value,
            std::promise<value_type>* end_value)
        {
            try{
                Iterator end = last;
                ++end;
                 /* std::partial_sum 是 C++ 标准库中的一个算法
                 *  用于计算一个序列的部分和,将结果存放在第三个参数定义的位置
                 *  如果想要计算到a[i]出 partial_sum的第二个参数必须输入到i+1 所以定义++end
                 *  2 0 2 4 2 2
                 *  2 2 4 8 10 12
                 *  此时last位置就是begin 到 last的部分和
                 */
                std::partial_sum(begin,end,begin);
                // 如果previous_end_value 不是0 也就是这不是第一个区间
                // 那么会将前一个块的结束值加到当前块的最后一个元素上
                // 这是为了保证每个块的部分和是连续的。
                if(previous_end_value){
                    value_type addend = previous_end_value->get();
                    // 此处相当于 前面数据和 + 当前区间数据和
                    *last += addend;
                    if(end_value){
                        //将结果写入到end_value中
                        end_value->set_value(*last);
                    }
                    // 将前一个块的结束值加到当前块中的每个元素上 保证当前位置的数据结果是含有前面部分和的
                    std::for_each(begin,end,[addend](value_type& item){
                        item += addend;
                    });
                }
                //如果是第一个值，就将end_value设置为last的值
                else if(end_value){
                    end_value->set_value(*last);
                }
            }
            catch(...){
                if(end_value){
                    end_value->set_exception(std::current_exception());
                }
                else{
                    throw;
                }
            }
        }
    };

    unsigned int const length = std::distance(first,last);
    if (!length) {
        return;
    }
    unsigned long const min_per_thread = 25;     //⇽-- - 12
    unsigned long const max_threads = (length + min_per_thread - 1) / min_per_thread;
    unsigned long const hardware_threads = std::thread::hardware_concurrency();
    unsigned long const num_threads = std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);
    unsigned long const block_size = length / num_threads;
    typedef typename Iterator::value_type value_type;

    std::vector<std::thread>threads(num_threads -1 );
    std::vector<std::promise<value_type>>end_values(num_threads -1);
    std::vector<std::future<value_type>>previous_end_values(num_threads -1);

    join_threads joiner(threads);
    Iterator block_start = first;
    for(unsigned int i = 0; i < num_threads -1 ;++i){
        Iterator block_last = block_start;
        std::advance(block_last,block_size);
        threads[i] = std::thread(process_chunk(),block_start,block_last,
                                 (i!=0) ? &previous_end_values[i-1]: 0,
                                 &end_values[i]);
        block_start = block_last;
        // block_last是已经被加过的了 所以此处block_start就应该向前进一位
        ++block_start;
        previous_end_values.emplace_back(end_values[i].get_future());
    }
    Iterator final_element = block_start;
    std::advance(final_element, std::distance(block_start, last) - 1);
    process_chunk()(block_start, final_element,
            (num_threads > 1) ? &previous_end_values.back() : 0,
                    0);
}




void test_pallar_sum(){
    std::vector<int>coll = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,
                            17,18,19,20,21,22,23,24,25,26,27,28,29,30,31};
    for_each(coll.begin(),coll.end(),[&](const int &val){
        std::cout << val << " ";
    });
    std::cout << std::endl;
    parallel_partial_sum(coll.begin(),coll.end());
    for_each(coll.begin(),coll.end(),[&](const int &val){
        std::cout << val << " ";
    });
    std::cout << std::endl;
}


int main(){
    test_for_each();
    test_pallar_for_each();
    test_pallar_find();
    test_pallar_sum();
    return 0;
}