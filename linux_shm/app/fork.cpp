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
    int fork_value = 0;

    while(1){
        sharedata read_value = shm->ReadFlagFromShare();

        if(read_value.flag > fork_value){
            std::cout << "is start arr is "<<std::endl;
            for(size_t i=0; i<sizeof(read_value.is_start)/sizeof(read_value.is_start[0]); i++){
                std::cout << read_value.is_start[i] << " ";
            }
            std::cout << std::endl;
            std::cout << "is _re arr is "<<std::endl;
            for(size_t i=0; i<sizeof(read_value._re)/sizeof(read_value._re[0]); i++){
                std::cout << read_value._re[i] << " ";
            }
            std::cout << std::endl;
            ++fork_value;
        }
        if(read_value.flag < 0){
            break;
        }
    }
    return 0;
}