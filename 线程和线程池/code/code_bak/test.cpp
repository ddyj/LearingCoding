#include <iostream>
#include <list>
#include <vector>
#include <thread>
#include <future>
#include <algorithm>
#include <queue>
#include <functional>

// 线程池类的简化实现
class ThreadPool {
public:
    ThreadPool(size_t threads) : stop(false) {
        for (size_t i = 0; i < threads; ++i)
            workers.emplace_back([this] {
                for (;;) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(this->queue_mutex);
                        this->condition.wait(lock, [this] { return this->stop || !this->tasks.empty(); });
                        if (this->stop && this->tasks.empty())
                            return;
                        task = std::move(this->tasks.front());
                        this->tasks.pop();
                    }
                    task();
                }
            });
    }

    template<class F, class... Args>
    auto commit(F&& f, Args&&... args) -> std::future<decltype(f(args...))> {
        using return_type = decltype(f(args...));
        auto task = std::make_shared<std::packaged_task<return_type()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
        std::future<return_type> res = task->get_future();
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            tasks.emplace([task]() { (*task)(); });
        }
        condition.notify_one();
        return res;
    }

    ~ThreadPool() {
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            stop = true;
        }
        condition.notify_all();
        for (std::thread& worker : workers)
            worker.join();
    }

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop;
};

// 顺序快速排序函数
template<typename T>
std::list<T> sequential_quick_sort(std::list<T> input) {
    if (input.empty()) {
        return input;
    }
    std::list<T> result;
    result.splice(result.begin(), input, input.begin());
    T const& pivot = *result.begin();
    auto divide_point = std::partition(input.begin(), input.end(), [&](T const& t) { return t < pivot; });
    std::list<T> lower_part;
    lower_part.splice(lower_part.end(), input, input.begin(), divide_point);
    auto new_lower = sequential_quick_sort(std::move(lower_part));
    auto new_higher = sequential_quick_sort(std::move(input));
    result.splice(result.end(), new_higher);
    result.splice(result.begin(), new_lower);
    return result;
}

// 并发快速排序函数
template<typename T>
std::list<T> parallel_quick_sort(std::list<T> input, ThreadPool& pool) {
    if (input.empty()) {
        return input;
    }
    std::list<T> result;
    result.splice(result.begin(), input, input.begin());
    T const& pivot = *result.begin();
    auto divide_point = std::partition(input.begin(), input.end(), [&](T const& t) { return t < pivot; });
    std::list<T> lower_part;
    lower_part.splice(lower_part.end(), input, input.begin(), divide_point);
    auto new_lower = pool.commit(&parallel_quick_sort<T>, std::move(lower_part), std::ref(pool));
    auto new_higher = pool.commit(&parallel_quick_sort<T>, std::move(input), std::ref(pool));
    result.splice(result.end(), new_higher.get());
    result.splice(result.begin(), new_lower.get());
    return result;
}

int main() {
    std::list<int> input = {5, 2, 9, 3, 7, 1, 8, 6, 4};
    ThreadPool pool(std::thread::hardware_concurrency());
    auto sorted_list = parallel_quick_sort(std::move(input), pool);
    std::cout << "Sorted list: ";
    for (const auto& elem : sorted_list) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
    return 0;
}
