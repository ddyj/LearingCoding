#include<iostream>
#include<atomic>
#include<thread>
#include <cassert>
#include<vector>

#include<mutex>
#include<memory>


using namespace std;

atomic<bool>x;
atomic<bool>y;
atomic<int>z;
void write_x_then_y_fence()
{
    x.store(true, std::memory_order_relaxed);  
    std::atomic_thread_fence(std::memory_order_release);  
    y.store(true, std::memory_order_relaxed);  
}
void read_y_then_x_fence()
{
    while (!y.load(std::memory_order_relaxed));  
    std::atomic_thread_fence(std::memory_order_acquire); 
    if (x.load(std::memory_order_relaxed)) 
        ++z;
}
void TestFence()
{
    x = false;
    y = false;
    z = 0;
    std::thread a(write_x_then_y_fence);
    std::thread b(read_y_then_x_fence);
    a.join();
    b.join();
    assert(z.load() != 0);   //7
}
int main(){
    TestFence();
    return 0;
}