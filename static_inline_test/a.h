#ifndef __A_H__
#define __A_H__
#include <iostream>

namespace a{
    inline int num = 10;
    inline void fun(){
        std::cout << "this is a" << std::endl;
    }
}
// static int num = 10;
// static void fun(){
//     std::cout << "this is a" << std::endl;
// }
// inline int num = 10;
// inline void fun(){
//     std::cout << "this is a" << std::endl;
// }


#endif

