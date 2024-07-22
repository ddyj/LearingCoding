#ifndef _SHAREMEMORY_H
#define _SHAREMEMORY_H
#include <iostream>
#include <cassert>
#include <stdlib.h>
#include <sys/shm.h>
#include "string.h"
#include <vector>
#include <chrono>

//for random
#include <cstdlib>
#include <ctime>
using namespace std;

constexpr int num = 4;
using time_ms = std::chrono::milliseconds::rep;

typedef struct sharedata
{
    int flag;
    time_ms is_start[num];
    double _re[15] = {0.0};

    // 定义带参数的构造函数
    sharedata(int a,size_t time_size,size_t re_size) : flag(a){
            for(size_t i=0; i<time_size; ++i) {
                is_start[i] = 0;
            }

            std::srand(static_cast<unsigned>(std::time(0)));
            double random_double = static_cast<double>(std::rand());

            for(size_t i=0; i<re_size; ++i){
                _re[i] = random_double;
            }
    }
    sharedata(int a, time_ms* start,double* re,size_t time_size,
              size_t re_size) : flag(a){
        for(size_t i=0; i<time_size; ++i) {
            is_start[i] = start[i];
        }
        for(size_t i=0; i<re_size; ++i){
            _re[i] = re[i];
        }
    }
    sharedata() : flag(0){
        for(int i=0; i<num; ++i) {
            is_start[i] = 0;
        }
    }
}sharedata;

class ShareMemory
{
    private:
        int _key;
        int shm_id;
        sharedata* buffer_head;
        enum CAMERA_NUM{stand=num};
    public:
        ShareMemory(int key):_key(key){};
        ~ShareMemory(){
            if (shmdt(buffer_head) == -1) {
                std::cerr << "shmdt error" << std::endl;
            }
            // 删除共享内存
            if (shmctl(shm_id, IPC_RMID, nullptr) == -1) {
                std::cerr << "shmctl error" << std::endl;
            }
        };
        void WriteFlag2Share(sharedata temp);
        void WriteFlag2Share(int value,time_ms* res,size_t t_size,size_t r_size);

        sharedata ReadFlagFromShare(sharedata &temp);
        sharedata ReadFlagFromShare();
        void getBufferHead();
};
#endif
