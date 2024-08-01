//使用c语言实现多态 -- 没有编译
#include <stdio.h>
namespace mutil {
    typedef void(*fun_ptr)();
    void f_base() {
        printf("this is base struct\n");
    }
    void f_son() {
        printf("this is son struct\n");
    }
    struct _Base {
        fun_ptr _f_b;
        int _val_base;
    };
    struct _Son
    {
        //通过组合的方式模拟继承
        _Base b;
        int _val_son;
    };
}
//用C模拟C++多态实现
void c2cpp_mutil() {
    mutil::_Base b;
    b._f_b = mutil::f_base;
    mutil::_Son a;
    a.b._f_b = mutil::f_son;
    mutil::_Base* p1 = &b;
    p1->_f_b();
    p1 = (mutil::_Base*)&a;
    p1->_f_b();
}