//
// Created by dai on 2024/7/8.
//

#include <iostream>
#include <climits>
#include <cstdlib>

template <typename T>
class myAllocator{
public:
    static T* allocate(size_t size){
        if(size > max_size()){
            throw std::bad_alloc();
        }
        void *result = malloc(size * sizeof(T));
        if(result == nullptr){throw std::bad_alloc();}
        return static_cast<T*>(result);
    }
    static T* reallocate(T*p,size_t size){
        if(size > max_size()){
            throw std::bad_alloc();
        }
        void *result = realloc((void*)p,size*sizeof(T));
        if(result == nullptr){throw std::bad_alloc();}
        return static_cast<T*>(result);
    }
    static void deallocate(T* p){
        free((void*)p);
    }
    static size_t max_size(){
        //size_t(-1) 无符号类型整数 表示所有位都为1 在不同的系统上显示并不一样
        return static_cast<size_t>(size_t(-1) / sizeof(T));
    }
};

void test_myallocta(){
    using alloc = myAllocator<int>;
    size_t size = 5;
    int * arr = alloc::allocate(size);
    for(size_t i= 0;i<size;++i){
        arr[i] = i;
    }
    for(size_t i=0; i<size; i++){
        std::cout <<arr[i] << "\n";
    }
    std::cout << "reallocated test " << std::endl;
    arr = alloc::reallocate(arr,10);
    for(size_t i=5;i<10;++i){
        arr[i] = i;
    }
    for(size_t i=0; i<10; i++){
        std::cout <<arr[i] << "\n";
    }

    alloc::deallocate(arr);
}


int main(){
    test_myallocta();
    return 0;
}