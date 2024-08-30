#include "a.h"
#include "common.h"
void func_a(){
    a::num += 1;
}
void show_a(){
    std::cout << a::num << std::endl;
}