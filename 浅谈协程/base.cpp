#include <coroutine>
#include <iostream>
#include <thread>


// C++20

class IntReader {
public:
    bool await_ready() {
        return false;
    }
    void await_suspend(std::coroutine_handle<> handle) {
        std::thread thread([this, handle]() {
                static int seed = 0;
                value_ = ++seed;
                handle.resume();
            });
        thread.detach();
    }
    int await_resume() {
        return value_;
    }
private:
    int value_{};
};

class Task {
public:
    class promise_type {
    public:
        Task get_return_object() {
            return Task{ std::coroutine_handle<promise_type>::from_promise(*this) };
        }
        std::suspend_always yield_value(int value) {
            value_ = value;
            return {};
        }
        void return_void() { }
        std::suspend_never initial_suspend() { return {}; }
        std::suspend_never final_suspend() noexcept { return {}; }
        void unhandled_exception() {}

        int GetValue() const {
            return value_;
        }

    private:
        int value_{};
    };

public:
    Task(std::coroutine_handle<promise_type> handle) : coroutine_handle_(handle) {

    }
    int GetValue() const {
        return coroutine_handle_.promise().GetValue();
    }
    bool Next() {
        if(!coroutine_handle.done()){
            coroutine_handle_.resume();
            return true;
        }
        return false;
    }
private:
    std::coroutine_handle<promise_type> coroutine_handle_;
};

Task GetInt() {
    while (true) {
        IntReader reader;
        int value = co_await reader;
        co_yield value;
    }
}
int test_co() {
    auto task = GetInt();
    std::string line;
    while (std::cin >> line) {
        std::cout << task.GetValue() << std::endl;
        task.Next();
    }
    return 0;
}


// 用于生成一系列整数的异步生成器
class AsyncGenerator {
public:
    struct promise_type;
    using handle_type = std::coroutine_handle<promise_type>;

    struct promise_type {
        int current_value;

        AsyncGenerator get_return_object() {
            return AsyncGenerator(handle_type::from_promise(*this));
        }
        //std::suspend_never initial_suspend() noexcept { return {}; }
        std::suspend_always initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        void return_void() {}
        std::suspend_always yield_value(int value) {
            current_value = value;
            return {};
        }
        void unhandled_exception() {
            std::exit(1); // 简单的异常处理：直接退出程序
        }
    };

    AsyncGenerator(handle_type coroutine_handle) : handle_(coroutine_handle) {
        std::cout << "构造 AsyncGenerator" << std::endl;
    }

    ~AsyncGenerator() {
        if (handle_) handle_.destroy();
    }

    bool move_next(int i) {
        std::cout << i << std::endl;
        if (!handle_.done()) {
            handle_.resume();
            return true;
        }
        return false;
    }

    int current_value() const {
        return handle_.promise().current_value;
    }

private:
    handle_type handle_;
};

// 生成一系列数字的协程
AsyncGenerator generate_numbers(int start, int end) {
    for (int i = start; i <= end; ++i) {
        std::cout << "yield :" << i << std::endl;
        co_yield i; // 产生一个值
        std::cout << "resume :" << i << std::endl;
    }
}

int test_co2() {
    auto generator = generate_numbers(1, 5); // 生成1到5的整数
    int flag = 0;
    while (generator.move_next(flag)) {
        std::cout << "Generated number: " << generator.current_value() << std::endl;
        ++flag;
    }

    return 0;
}