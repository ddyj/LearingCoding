/*** 
 * @Descripttion: 
 * @version: 
 * @Author: lhw
 * @Date: 2023-06-29 19:39:21
 * @FilePath: /myCplusplusDesign/MyDesign/4_unique_ptr.cpp
 * @LastEditors: lhw
 * @LastEditTime: 2023-06-29 20:03:45
 */
#include <iostream>
#include <atomic>
#include <mutex>
#include <memory>
using namespace std;
namespace my{

template<typename T, typename Deleter = std::default_delete<T>>
class unique_ptr{
public:
    explicit unique_ptr(T* ptr=nullptr): _ptr(ptr){}
    ~unique_ptr() noexcept{
        if(_ptr){
            Deleter()(_ptr);
        }
    }
    // 拷贝构造和赋值
    unique_ptr(const unique_ptr& ) = delete;
    unique_ptr& operator = (const unique_ptr& ) = delete;
    // 移动构造和赋值
    unique_ptr(unique_ptr&& obj)noexcept : _ptr(obj.release()){}
    unique_ptr& operator = (unique_ptr&& obj) noexcept{
        reset(obj.release());
        return *this;
    }

    T* release(){
        T* temp = _ptr;
        _ptr = nullptr;
        return temp;
    }
    void reset(T* ptr=nullptr){
        if(_ptr){
            Deleter()(_ptr);
        }
        _ptr = ptr;
    }

    T* get(){
        return _ptr;
    }

    T& operator *() const{
        return *_ptr;
    }
    T* operator ->() const{
        return _ptr;
    }
    
    void swap(unique_ptr& obj){
        swap(_ptr, obj._ptr);
    }
    explicit operator bool(){
        return _ptr !=nullptr;
    }
private:
    T* _ptr;
};


};

class test{
public:
    test(){
        p = new int(9);
    }
    ~test(){
        delete p;
        std::cout << " ~ constru" << std::endl;
    }
    void print(){
        std::cout << "class test: *p:"<< *p << std::endl;

    }
    int* p;
};

struct deleter{
    void operator() (test* t) const{
        std::cout << "operator delete" << std::endl;
        delete t;
    }
};

// g++ ../4_uniquePtr.cpp -o 4_uniquePtr -g -fsanitize=address
int main(){
    int *p = new int(1);
    my::unique_ptr<int> my_unique = my::unique_ptr<int>(p);
    std::cout << "*my_unique:"<< *my_unique << std::endl;


    auto my_uniq2 = std::move(my_unique);
    std::cout << "*my_uniq2:"<< *my_uniq2 << std::endl;

    if (my_unique) {
        std::cout << "my_unique is not null" << std::endl;
    } else {
        std::cout << "my_unique is null" << std::endl;
    }
    
    my::unique_ptr<test, deleter> my_uniq3 = my::unique_ptr<test, deleter>(new test());
    my_uniq3->print();

}


//dyj 没有实现析构函数
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
