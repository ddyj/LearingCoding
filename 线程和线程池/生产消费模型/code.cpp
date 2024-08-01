#include<iostream>
#include<thread>
#include<mutex>
#include<condition_variable>
#include<queue>
#include<functional>
#include<atomic>
#include<random>

class producer_comsuerMode {
private:
    std::mutex m_tx;
    std::condition_variable cond_overMax;
    std::condition_variable cond_isEmpty;
    std::condition_variable cond_stop;
    using fun_type = std::function<int(int, int)>;
    const int max_size = 20;
    const int proVecNum = 3;
    const int comVecNum = 2;
    std::queue<fun_type>task_queue;
    std::vector<std::thread>prodecuerVec;
    std::vector<std::thread>comsuerVec;
    std::atomic_bool is_done = false;

public:
    producer_comsuerMode() {
        prodecuerVec.resize(proVecNum);
        comsuerVec.resize(comVecNum);
        for (int i = 0; i < proVecNum; ++i) {
            std::thread t = std::thread(&producer_comsuerMode::prodecuer,
                this,std::ref(task_queue), i, std::ref(is_done));
            prodecuerVec[i] = (std::move(t));
        }
        for (int i = 0; i < comVecNum; ++i) {
            std::thread t = std::thread(&producer_comsuerMode::comsuer,
                this, std::ref(task_queue), i, std::ref(is_done));
            comsuerVec[i] = (std::move(t));
        }
        for (int i = 0; i < proVecNum; ++i) {
            prodecuerVec[i].detach();
        }
        for (int i = 0; i < comVecNum; ++i) {
            comsuerVec[i].detach();
        }
    }
    void prodecuer(std::queue<fun_type>& task_queue, int i, std::atomic_bool& is_done) {
        while (!is_done) {
            std::this_thread::sleep_for(std::chrono::microseconds(800));
            std::unique_lock<std::mutex>lck(m_tx);
            cond_overMax.wait(lck, [&]() {
                return task_queue.size() < max_size;
            });
            if (is_done) { break; }
            auto func = std::bind(&producer_comsuerMode::add,this,
                std::placeholders::_1, std::placeholders::_2);
            task_queue.push(func);
            printf("producer id is %d add func,cur task num is %d\n",i,task_queue.size());
            cond_isEmpty.notify_one();
        }
    }
    void comsuer(std::queue<fun_type>& task_queue, int i, std::atomic_bool& is_done) {
        while (!is_done || !task_queue.empty()) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::unique_lock<std::mutex>lck(m_tx);
            cond_isEmpty.wait(lck, [&]() {
                return !task_queue.empty() || is_done;
            });
            if(!task_queue.empty()){
                auto func = task_queue.front();
                task_queue.pop();
                printf("comsuer id is %d get func,cur task num is %d\n",i,task_queue.size());
                cond_overMax.notify_one();
                if(is_done){
                    cond_stop.notify_one();
                }
                lck.unlock();
                std::random_device rd; // 获取随机数种子
                std::mt19937 gen(rd()); // 采用梅森旋转算法生成器
                std::uniform_int_distribution<> dis(0, 10); // 生成0到10之间的整数
                func(i, dis(gen));
            }
        }
    }
    int add(int x, int y) {
        printf("x is %d ,y is %d\n", x, y);
        return x + y;
    }
    void stop(){
        is_done = true;
        cond_isEmpty.notify_all();
        cond_overMax.notify_all();
        std::unique_lock<std::mutex>lck(m_tx);
        while(!task_queue.empty()){
            cond_stop.wait(lck);
        }
        cond_stop.notify_all();
    }
    ~producer_comsuerMode() {
        std::cout << 1 << std::endl;
    }
};




#include <termio.h>
#include <stdio.h> 
int scanKeyboard()
{
    int in;
    struct termios new_settings;
    struct termios stored_settings;
    tcgetattr(0,&stored_settings);
    new_settings = stored_settings;
    new_settings.c_lflag &= (~ICANON);
    new_settings.c_cc[VTIME] = 0;
    tcgetattr(0,&stored_settings);
    new_settings.c_cc[VMIN] = 1;
    tcsetattr(0,TCSANOW,&new_settings);

    in = getchar();
    tcsetattr(0,TCSANOW,&stored_settings);
    return in;
}

void producer_comsuer() {
    producer_comsuerMode* p_c = new producer_comsuerMode();
    while(1)
    {
        int ascii = scanKeyboard();
        printf(":%d\n",ascii);
        if(ascii==27) {
            p_c->stop();
            break;
        };
    }
    delete p_c;
    p_c = nullptr;
}

int main(){
    producer_comsuer();
}