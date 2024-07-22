#include<iostream>
#include <vector>
#include <algorithm>

//vector 扩容逻辑
void VecDoubleResize() {
    std::vector<int>vec;
    for (int i = 0; i < 10; ++i) {
        vec.push_back(i);
        std::cout << "size :" << vec.size() 
            << " capacity: " << vec.capacity() << "\n"
            << "first iterator: " << &(*vec.begin()) << std::endl;
    }
}

template<typename T>
class my_Vector {
private:
    T* _data;
    std::size_t _size;
    std::size_t _capacity;
public:
    my_Vector() :_size(0), _capacity(1), _data(new T[1]) {};
    my_Vector(size_t size,size_t capacity) :_size(size), _capacity(capacity), _data(new T[capacity]) {};
    my_Vector(const my_Vector& mV) {
        _capacity = mV.capacity();
        _size = mV.size();
        _data = new T[_capacity];
        std::copy_n(mV.get(), _size, _data);
    }
    my_Vector& operator=(const my_Vector& mV) {
        if (&mV == this) {
            return *this;
        }
        if (_data != nullptr) {
            delete[] _data;
        }
        _capacity = mV.capacity();
        _size = mV.size();
        _data = new T[_capacity];
        std::copy_n(mV.get(),_size,_data);
        return *this;
    }
    my_Vector(my_Vector&& mV) noexcept : _data(mV._data), _capacity(mV._capacity), _size(mV._size){
        mV._data = nullptr;
        mV._size = 0;
        mV._capacity = 0;
    }
    inline int size() const { return _size; }
    inline int capacity()const { return _capacity; }
    inline T* get() const { return _data; }
    
    void push_back(T val) {
        if (_size == _capacity) {
            m_resize(_capacity == 0 ? 1 : _capacity * 2);
        }
        _data[_size++] = val;
        return;
    }
    void pop_back() {
        if (_size <= 0) {
            throw std::out_of_range("pop_back on empty vector");
        }
        --_size;
        if (_size < _capacity / 4) {
            m_resize(_capacity / 2);
        }
        return;
    }
    void m_resize(size_t capacity) {
        if (capacity < _size) {
            capacity = _size;
        }
        T* new_data = new T[capacity];
       /* for (int i = 0; i < _size; ++i) {
            new_data[i] = _data[i];
        }   */    
        std::copy_n(_data,_size,new_data);
        delete[] _data;
        _data = new_data;
        _capacity = capacity;
    }
    T& operator[](int offset) {
        if (offset >= _size) {
            throw std::out_of_range("invaild index input");
        }
        return _data[offset];
    }
    void show_data() {
        for (int i = 0; i < _size; ++i) {
            std::cout << _data[i] << std::endl;
        }
    }
    ~my_Vector() {
        delete[] _data;
    }
};
//test
void my_Vec() {
    /*
        要求实现一个自定义的vec
        考虑扩容和缩容
    */
    my_Vector<int>vc;
    for (int i = 0; i < 10; ++i) {
        vc.push_back(i);
        std::cout << vc.capacity() << " " << vc.size() << std::endl;
    }
    vc.show_data();

    //for (int i = 0; i < 10; ++i) {
    //    vc.pop_back();
    //    std::cout << vc.capacity() << " " << vc.size() << std::endl;
    //}
    //my_Vector<RVO>vc_Rov;
    //for (int i = 0; i < 10; ++i) {
    //    RVO r;
    //    vc_Rov.push_back(r);
    //    std::cout << vc_Rov.capacity() << " " << vc_Rov.size() << std::endl;
    //}
    //vc_Rov.show_data();

    my_Vector<int>vc2 = vc;
    std::cout << "vc2 " << std::endl;
    vc2.show_data();
    my_Vector<int>vc3(vc);
    std::cout << "vc3 " << std::endl;
    vc3.show_data();
    my_Vector<int>vc4(std::move(vc2));
    std::cout << "vc4 " << std::endl;
    vc4.show_data();
    std::cout << "after move vc2 " << std::endl;
    vc2.show_data();

    for (int i = 0; i < 10; ++i) {
       vc.pop_back();
       std::cout << vc.capacity() << " " << vc.size() << std::endl;
    }
}

int main(){
    my_Vec();
    return 0;
}