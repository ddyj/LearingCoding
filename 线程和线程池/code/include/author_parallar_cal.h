//
// Created by dai on 2024/4/2.
//

#ifndef __AUTHOR_PARALLAR_CAL_H__
#define __AUTHOR_PARALLAR_CAL_H__


#include <iostream>
#include <thread>
#include <algorithm>
#include <vector>
#include <future>
#include <memory>
#include <atomic>

#include <numeric> // 包含 partial_sum 函数的头文件


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


#endif //__AUTHOR_PARALLAR_CAL_H__
