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
    void Next() {
        coroutine_handle_.resume();
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