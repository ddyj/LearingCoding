#include <iostream>
#include <algorithm>
template<typename T>
class myVec{
protected:
    T* pdata;
    int _capacity;
    int _size;
public:
    using value_type = T;
    myVec():_capacity(1), _size(0){
        pdata = new T[_capacity];
        std::cout << "default construction"<<std::endl;

    }
    ~myVec(){
        delete [] pdata;
        std::cout << "~ construction"<<std::endl;

    }
    myVec(const myVec& obj){
        std::cout << "copy construction"<<std::endl;
        _capacity = obj._capacity;
        _size = obj._size;
        pdata = new T[obj._capacity];       // 第一次构造，不需要先delete
        std::copy_n(obj.pdata, obj._size, pdata);
    }
    myVec& operator=(const myVec& obj){
        if(&obj == this) return *this;
        std::cout << "copy = construction"<<std::endl;
        if(pdata!=nullptr){
            delete [] pdata;
        }
        _capacity = obj._capacity;
        _size = obj._size;
        pdata = new T[obj._capacity];
        std::copy_n(obj.pdata, obj._size, pdata);
        return *this;
    }
    myVec(myVec&& obj): pdata(obj.pdata), _capacity(obj._capacity), _size(obj._size){
        std::cout << "move construction"<<std::endl;
        obj.pdata = nullptr;
        obj._size = 0;
        obj._capacity = 0;
    }

    void push_back(const T& val){
        if(_size == _capacity){
            T* newpdata = new T[_capacity*2];
            std::copy_n(pdata, _size, newpdata);
            delete[] pdata;
            pdata = newpdata;
            pdata[_size++] = val;
            _capacity *= 2;
            return ;
        }
        pdata[_size++] = val;
        return ;
    }
    void pop_back(){
        if(_size > 0){
            --_size;
        }
        return;
    }
    T& operator[](int index){
        return pdata[index];
    }
    int size() const{
        return _size;
    }
    int capacity() const{
        return _capacity;
    }
    void printdata(std::ostream& out = std::cout) {
        for(int i=0; i< _size; ++i){
            out << pdata[i] << " ";
        }
        out<<std::endl;
    }
};


int main(){
    myVec<int> q;
    for(int i=0; i< 10; i++){
        q.push_back(i);
        std::cout << "i:" << q[i] << " size:"<<q.size() << " cap:"<< q.capacity()<< std::endl;
    }
    std::cout << std::endl;

    myVec<int> p(q);
    p.printdata();


    std::cout << std::endl;
    myVec<int> m ;
    m = p;
    m.printdata();

    
    std::cout << std::endl;
    myVec<int> n(std::move(m));
    n.printdata();

}



/*dyj更新
实现了缩容 当size*4 小于容积 容积减半
*/

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
}


/*
关于vector的扩容
底层是怎么实现的:
这里给出vstudio的底层
*/
//进入push_back()函数之后:
_CONSTEXPR20_CONTAINER void push_back(const _Ty& _Val) { // insert element at end, provide strong guarantee
    emplace_back(_Val);
}

_CONSTEXPR20_CONTAINER void push_back(_Ty&& _Val) {
    // insert by moving into element at end, provide strong guarantee
    emplace_back(_STD move(_Val));
}
//进入emplace_back()函数之后
_CONSTEXPR20_CONTAINER decltype(auto) emplace_back(_Valty&&... _Val) {
        // insert by perfectly forwarding into element at end, provide strong guarantee
        auto& _My_data   = _Mypair._Myval2;
        pointer& _Mylast = _My_data._Mylast;
        if (_Mylast != _My_data._Myend) {
            return _Emplace_back_with_unused_capacity(_STD forward<_Valty>(_Val)...);
        }

        _Ty& _Result = *_Emplace_reallocate(_Mylast, _STD forward<_Valty>(_Val)...);
#if _HAS_CXX17
        return _Result;
#else // ^^^ _HAS_CXX17 ^^^ // vvv !_HAS_CXX17 vvv
        (void) _Result;
#endif // _HAS_CXX17
}
//如果此时容量不足，就会进入_Emplace_reallocate函数，这里我们只给出节选
const size_type _Newsize     = _Oldsize + 1;
const size_type _Newcapacity = _Calculate_growth(_Newsize);
const pointer _Newvec           = _Al.allocate(_Newcapacity);
//计算_Newcapacity新的容积大小_Calculate_growth
_CONSTEXPR20_CONTAINER size_type _Calculate_growth(const size_type _Newsize) const {
    // given _Oldcapacity and _Newsize, calculate geometric growth
    const size_type _Oldcapacity = capacity();
    const auto _Max              = max_size();
    //扩容不允许超过 最大元素容量
    if (_Oldcapacity > _Max - _Oldcapacity / 2) {
        return _Max; // geometric growth would overflow
    }
    
    const size_type _Geometric = _Oldcapacity + _Oldcapacity / 2;
    //扩容值要期满满足_size+1的条件 所以此时取max
    if (_Geometric < _Newsize) {
        return _Newsize; // geometric growth would be insufficient
    }

    return _Geometric; // geometric growth is sufficient
}
//这里可以看到其实这里是实现的1.5倍扩容，max是最大的容器容量，