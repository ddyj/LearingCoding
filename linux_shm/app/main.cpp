//
// Created by dai on 2024/7/9.
//
#include <iostream>
#include "ShareMemory.h"
#include <termios.h>
#include <unistd.h>
#include <cstdio>


using namespace std;

int main(){
    int key = 650;
    ShareMemory* shm = new ShareMemory(key);
    shm->getBufferHead();
    int write_value = 0;
    time_ms _temp[10]={};
    sharedata temp{write_value,10,15};
    shm->WriteFlag2Share(temp);

    char _buf[1024];
    while(1){
        fgets(_buf, sizeof(_buf), stdin);
        string _buf_str(_buf);
        if(_buf[0]=='c'||_buf[0]=='C'){
            std::cout << " add cur num "<<std::endl;
            ++write_value;
            time_ms _temp[10]={1,2,3,4,5,6,7,8,9,10};
            shm->WriteFlag2Share(write_value,_temp,10,15);
        }
        if(_buf[0]=='b'||_buf[0]=='B'){
            write_value = -1;
            time_ms _temp[10]={};
            shm->WriteFlag2Share(write_value,_temp,10,15);
            break;
        }
    }
    return 0;
}