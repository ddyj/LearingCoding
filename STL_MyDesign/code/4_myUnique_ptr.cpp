//
// Created by dai on 2024/7/8.
//

#include <iostream>
#include <atomic>
#include <mutex>
#include <memory>

template<class T>
class MyUnique_Ptr{
private:
    T* _ptr;
public:
    MyUnique_Ptr():_ptr(nullptr) {};
    MyUnique_Ptr(T* ptr) : _ptr(ptr) {};
    MyUnique_Ptr(const MyUnique_Ptr& other) = delete;
    MyUnique_Ptr& operator=(const MyUnique_Ptr& other) = delete;
    MyUnique_Ptr(MyUnique_Ptr&& other){
        _ptr = other.get();
        other.clear();
    }
    MyUnique_Ptr& operator=(MyUnique_Ptr&& other){
        if(&other == this){return *this;}
        _ptr = other.get();
        other.clear();
    }
    void reset(T* new_ptr = nullptr){
        if(_ptr != nullptr){
            delete _ptr;
            _ptr = nullptr;
        }
        _ptr = new_ptr;
    } 
    void clear(){
        _ptr = nullptr;
    }
    /*
     * 那么会发生什么呢？std::swap 会尝试使用赋值运算符或移动操作来交换 ptr1 和 ptr2，
     * 但由于你禁用了拷贝构造函数和拷贝赋值运算符，
     * 并且你的类不支持默认的移动操作，这会导致编译错误。
     */
    void swap(MyUnique_Ptr<T>&obj) noexcept {
        T* temp = _ptr;
        _ptr = obj._ptr;
        obj._ptr = temp;
    }
    T& operator *() const{
        return *_ptr;
    }
    T* operator ->() const{
        return _ptr;
    }
    bool is_unique() const{
        return _ptr != nullptr;
    }
    inline T* get(){
        return _ptr;
    }
    ~MyUnique_Ptr(){
        if(_ptr != nullptr){
            delete _ptr;
            _ptr = nullptr;
        }
    }
};

void test_MyUnique_ptr(){
    MyUnique_Ptr<int> u_p1(new int(10));
    std::cout << *u_p1.get() << std::endl;
    MyUnique_Ptr<int> u_p2(new int(15));
    std::cout << *u_p2.get() << std::endl;
    u_p1.swap(u_p2);
    std::cout << *u_p1.get()<<" "<< *u_p2.get() << std::endl;
    MyUnique_Ptr<int> u_p3(std::move(u_p2));
    std::cout << u_p2.is_unique() << " " << *u_p3.get()<< std::endl;
    u_p2.reset(new int( 17));
    std::cout << u_p2.is_unique() << " " << *u_p2.get()<< std::endl;
}


class A{
private:
    int a;
public:
    A(int val = 0):a(val){};
    void t(A& b){
        std::cout << b.a<< std::endl;
    }
};

void test(){
    A a(10);
    A b(15);
    a.t(b);
    //std::cout << a.a<<std::endl; // error
}
int main(){
    test_MyUnique_ptr();
    test();
    return 0;
}