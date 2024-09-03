#include<iostream>
#include<thread>
#include<chrono>

using namespace std;

int x = 0,y = 0;

void run_1(){
    for(int i=0;i<1000;++i){
        x = i;
        std::cout << "in thread 1 x is "<<x<<std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}   
void run_2(){
    for(int i=0;i<1000;++i){
        y = i;
        std::cout << "in thread 2 y is "<<y<<std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}   


int main(){
    std::cout << "start craet threaed 1"<<std::endl;
    std::thread t1(run_1);
    std::cout << "start craet threaed 2"<<std::endl;
    std::thread t2(run_2);


    std::cout << "this is main thread"<<std::endl;
    t1.join();
    t2.join();
    return 0;
}