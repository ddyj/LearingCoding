#include <iostream>
#include <atomic>
#include <mutex>


class shared_count{
private:
    long long int cnts;
public:
    shared_count():cnts(1) {};
    shared_count(const shared_count&) = delete;
    shared_count& operator=(const shared_count& other) = delete;
    void add_count(){
        ++cnts;
    }
    long long int reduce_count(){
        --cnts;
        return cnts;
    }
    long long int get_count()const{
        return cnts;
    }
};


template<class T>
class MySharedPtr{
private:
    T* _ptr;
    shared_count* _count_ptr;
public:
    MySharedPtr() : _ptr(nullptr),_count_ptr(nullptr) {};
    //禁止隐式构造
    explicit MySharedPtr(T* ptr): _ptr(ptr){
        if(ptr!= nullptr){
            _count_ptr = new shared_count();
        }
    }
    //直接拷贝指针 实现共享数据、共享计数器
    MySharedPtr(MySharedPtr<T>& obj):_ptr(obj.get()),_count_ptr(obj.get_num()){
        _count_ptr->add_count();
    }
    MySharedPtr(MySharedPtr<T>&& obj):_ptr(obj.get()),_count_ptr(obj.get_num()){
        obj.clear();
    }
    MySharedPtr& operator=(MySharedPtr<T>& obj){
        if(&obj == this){return *this;}
        //这里reduce_count已经执行了--cnts的操作 所以覆盖不会有影响 
        //即使这里不等于0 如果此时this被覆盖 也需要减减的
        if(_ptr != nullptr && _count_ptr->reduce_count() == 0 ){
            delete _ptr;
            delete _count_ptr;
            clear();
        }
        if(obj.get()!=nullptr){
            _ptr = obj.get();
            _count_ptr = obj.get_num();
            _count_ptr->add_count();
        }
        return *this;
    }
    MySharedPtr& operator=(MySharedPtr<T>&&obj){
        if(&obj == this){
            std::cout<<"warning: you have used right reference with same ans ptr"<<std::endl;
            return *this;
        }
        if(_ptr != nullptr && _count_ptr->reduce_count() == 0 ){
            delete _ptr;
            delete _count_ptr;
            clear();
        }
        if(obj.get()!=nullptr){
            _ptr = obj.get();
            _count_ptr = obj.get_num();
            obj.clear();
        }
        return *this;
    }
    T*operator ->(){
        return _ptr;
    }
    T&operator*(){
        return *_ptr;
    }
    bool unique()const{
        if(_ptr!= nullptr && _count_ptr->get_count()!=0){
            return true;
        }
        return false;
    }

    inline T* get(){
        return _ptr;
    }
    inline shared_count* get_num(){
        return _count_ptr;
    }
    inline long long int used_count()const{
        if(_ptr == nullptr){
            return 0;
        }
        return _count_ptr->get_count();
    }
    void clear(){
        _ptr = nullptr;
        _count_ptr = nullptr;
    }
    void swap(MySharedPtr<T>&obj){
        swap(_ptr,obj.get());
        swap(_count_ptr,obj.get_num());
    }
    void reset(T* new_ptr=nullptr){
        if(new_ptr == nullptr){
            _count_ptr->reduce_count();
            _ptr = nullptr;
            _count_ptr = nullptr;
        }
        if(_ptr == nullptr){
            _ptr = new_ptr;
            _count_ptr = new shared_count();
        }
    }
    ~MySharedPtr(){
        if(_ptr != nullptr && _count_ptr->reduce_count()==0){
            delete _count_ptr;
            delete _ptr;
            clear();
        }
    };
};



void test_SharedPtr(){
    MySharedPtr<int>sp_1(new int(15));
    std::cout <<"sq1: default ct "<<sp_1.unique()<<" "<<sp_1.used_count() << std::endl;
    MySharedPtr<int>sp_2 = sp_1;
    std::cout <<"sq2: copy ct "<<sp_2.unique()<<" "<<sp_2.used_count() << std::endl;
    MySharedPtr<int>sp_3(std::move(sp_2));
    std::cout <<"sq3: move ct "<<sp_3.unique()<<" "<<sp_3.used_count() << std::endl;
    std::cout <<"sq2: after move ct "<<sp_2.unique()<<" "<<sp_2.used_count() << std::endl;
    MySharedPtr<int>sp_4(new int(10));

    //question
    MySharedPtr<int>sp_5(new int(12));
    MySharedPtr<int>sp_6 = sp_5;
    MySharedPtr<int>sp_7(std::move(sp_6));
    std::cout << sp_5.used_count()<< " "<< *sp_5.get() <<" " <<sp_7.used_count()<<" "<< *sp_7.get() << std::endl;
    sp_5.reset();
    std::cout << sp_5.used_count()<< " " <<" " <<sp_7.used_count()<<" "<< *sp_7.get() << std::endl;
    sp_5.reset(new int(17));
    std::cout << sp_5.used_count()<< " "<< *sp_5.get() <<" " <<sp_7.used_count()<<" "<< *sp_7.get() << std::endl;
}

int main(){
    test_SharedPtr();
    return 0;
}