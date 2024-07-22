#include<iostream>
#include<thread>
#include <functional> 
#include<memory>
#include<mutex>
#include<stack>
#include <exception>
#include  <climits>
#include <shared_mutex>

using namespace std;

void test_static_fun(){
    static int count = 0;
    std::cout << "static count = "<<count<<std::endl;
    count++;
}


class Single2 {
private:
    Single2()
    {
        std::cout << "this is ctr "<<std::endl;
    }
    Single2(const Single2&) = delete;
    Single2& operator=(const Single2&) = delete;
public:
    static Single2& GetInst()
    {
        static Single2 single;
        return single;
    }
    static shared_ptr<Single2> GetInstant(){
        static shared_ptr<Single2> s_ptr = std::shared_ptr<Single2>(new Single2()); 
        return s_ptr;
    }
};






template<typename C>
class Singleton
{
public:
	Singleton(const Singleton &) = delete;
	Singleton(Singleton &&) = delete;
	Singleton &operator=(const Singleton &) = delete;
	Singleton &operator=(Singleton &&) = delete;
 
protected:
	Singleton()
	{
	}
	virtual ~Singleton()
	{
	}
public:
 
	template<class... Args>
	static std::shared_ptr<C> Instance(Args&&... args)
	{
        //使用static中的once_flag变量，用call_once函数保证在多线程中单例安全
        //就没有出现用锁来限制单一访问的情况
        /*
        当 std::call_once 函数被多个线程同时调用时，
        std::once_flag 会保证其中的函数只会被其中一个线程执行，
        其他线程会被阻塞直到该函数执行完成。
        */
       
		static std::once_flag oc;
		std::call_once(oc, [&]{
            (std::cout << "input id is "<<... <<args) <<std::endl;
            //这里需要保证C的创建函数在public中，这里属于类外调用
			instance = std::make_shared<C>(std::forward<Args>(args)...);
		});
		return instance;
	}
private:
	static std::shared_ptr<C> instance;
};

template<typename C> 
	std::shared_ptr<C> Singleton<C>::instance = nullptr;

class MyClass
{
public:
    MyClass()
        :m_value(10)
    {}
 
    MyClass(int value)
        :m_value(value)
    {}
 
    ~MyClass()
    {
        std::cout << "xigou" << std::endl;
    }
	
	void destory()
	{
		std::cout << "destory" << std::endl;
	}
    void print()
    {
        std::cout << m_value << std::endl;
    }
    MyClass* get_pointer(){
        return this;
    }
private:
    int m_value;
};


void thread_func_auto(int i)
{
    std::cout << "this is auto thread " << i << std::endl;
    Singleton<MyClass>::Instance(i)->print();
    std::cout << "inst is MyClass " << Singleton<MyClass>::Instance(i)->get_pointer() << std::endl;
}
void test_sing_leauto()
{
    for (int i = 0; i < 3; i++)
    {
        std::thread tid(thread_func_auto, i);
        tid.join();
    }
}






class Single2Hungry
{
private:
    Single2Hungry()
    {
    }
    Single2Hungry(const Single2Hungry&) = delete;
    Single2Hungry& operator=(const Single2Hungry&) = delete;
public:
    static Single2Hungry* GetInst()
    {
        if (single == nullptr)
        {
            single = new Single2Hungry();
        }
        return single;
    }
private:
    static Single2Hungry* single;
};
Single2Hungry* Single2Hungry::single = Single2Hungry::GetInst();

void thread_func_s2(int i)
{
    std::cout << "this is thread " << i << std::endl;
    std::cout << "inst is " << Single2Hungry::GetInst() << std::endl;
}
void test_single2hungry()
{
    std::cout << "s1 addr is " << Single2Hungry::GetInst() << std::endl;
    std::cout << "s2 addr is " << Single2Hungry::GetInst() << std::endl;
    for (int i = 0; i < 3; i++)
    {
        std::thread tid(thread_func_s2, i);
        tid.join();
    }
}

class SinglePointer
{
private:
    SinglePointer()
    {
    }
    SinglePointer(const SinglePointer&) = delete;
    SinglePointer& operator=(const SinglePointer&) = delete;
public:
    static SinglePointer* GetInst()
    {
        if (single != nullptr)
        {
            return single;
        }
        s_mutex.lock();
        if (single != nullptr)
        {
            s_mutex.unlock();
            return single;
        }
        single = new SinglePointer();
        s_mutex.unlock();
        return single;
    }
private:
    static SinglePointer* single;
    static std::mutex s_mutex;
};
SinglePointer* SinglePointer::single = nullptr;
std::mutex SinglePointer::s_mutex;
void thread_func_lazy(int i)
{
    std::cout << "this is lazy thread " << i << std::endl;
    std::cout << "inst is " << SinglePointer::GetInst() << std::endl;
}
void test_singlelazy()
{
    for (int i = 0; i < 3; i++)
    {
        std::thread tid(thread_func_lazy, i);
        tid.join();
    }
    //何时释放new的对象？造成内存泄漏
}

class SingleAuto
{
private:
    SingleAuto()
    {
    }
    SingleAuto(const SingleAuto&) = delete;
    SingleAuto& operator=(const SingleAuto&) = delete;
public:
    ~SingleAuto()
    {
        std::cout << "single auto delete success " << std::endl;
    }
    static std::shared_ptr<SingleAuto> GetInst()
    {
        if (single != nullptr)
        {
            return single;
        }
        s_mutex.lock();
        if (single != nullptr)
        {
            s_mutex.unlock();
            return single;
        }
        //这里不能使用make_shared,make_shared会出现无法查找ctor函数
        single = std::shared_ptr<SingleAuto>(new SingleAuto);
        s_mutex.unlock();
        return single;
    }
private:
    static std::shared_ptr<SingleAuto> single;
    static std::mutex s_mutex;
};
std::shared_ptr<SingleAuto> SingleAuto::single = nullptr;
std::mutex SingleAuto::s_mutex;
void test_singleauto()
{
    auto sp1 = SingleAuto::GetInst();
    auto sp2 = SingleAuto::GetInst();
    std::cout << "sp1  is  " << sp1 << std::endl;
    std::cout << "sp2  is  " << sp2 << std::endl;
    //此时存在隐患，可以手动删除裸指针，造成崩溃
    // delete sp1.get();
}


int main(){
    test_static_fun();
    test_static_fun();
    test_static_fun();
    test_static_fun();

    Single2& sgl = Single2::GetInst();
    shared_ptr<Single2>ptr = Single2::GetInstant();

    auto sg1 = Singleton<MyClass>::Instance();
    sg1->print();

    test_single2hungry();
    test_singlelazy();
    test_singleauto();
    //单例模板
    test_sing_leauto();
    return 0;
}