#include<iostream>
#include<atomic>
#include<thread>
#include <cassert>
#include<vector>

#include<mutex>
#include<memory>
#include<condition_variable>
#include<stack>
#include<queue>
#include<exception>
#include<atomic>
#include<set>

struct empty_stack : std::exception
{
    const char* what() const throw();
};

template<typename T>
class SafeThreadStack{
private:
    std::stack<T>data;
    mutable std::mutex mtx;
    std::condition_variable condition_empty;
public:
    SafeThreadStack(){}
    SafeThreadStack(const SafeThreadStack& other){
        std::lock_guard<std::mutex>lock(other.mtx);
        data = other.data;
    }
    SafeThreadStack& operator=(const SafeThreadStack&)=delete;
    
    void push(T &new_val){
        std::lock_guard<std::mutex>lock(mtx);
        data.push(std::move(new_val));
        condition_empty.notify_one();
    }

    std::shared_ptr<T> wait_and_pop(){
        std::unique_lock<std::mutex>lock(mtx);
        condition_empty.wait(lock,[this](){
            return !data.empty();
        });
        std::shared_ptr<T>const res = std::make_shared<T>(std::move(data.top()));
        data.pop();
        return res;
    }
    void wait_and_pop(T& value){
        std::unique_lock<std::mutex>lock(mtx);
        condition_empty.wait(lock,[this](){
            return !data.empty();
        });
        value = std::move(data.top());
        data.pop();
    }
    std::shared_ptr<T>pop(){
        std::lock_guard<std::mutex>lock(mtx);
        if(data.empty()) {return nullptr;}
        std::shared_ptr<T> s_ptr= std::make_shared<T>(data.top());
        data.pop();
        return s_ptr;
    }
    void pop(T& val){
        std::lock_guard<std::mutex>lock(mtx);
        if(data.empty())   throw empty_stack();
        val = data.top();
        data.pop();
    }
    bool empty() const
    {
        std::lock_guard<std::mutex> lock(mtx);
        return data.empty();
    }
};

void test_thread_safe_stack(){
    SafeThreadStack<int> st_stack;
    std::thread t1([&st_stack](){
        for(int i=0;i<10;++i){
            st_stack.push(i);
            printf("push data%d\n",i);
        }
    });
    std::thread t2([&st_stack](){
        for(int i=0;i<10;++i){
            std::shared_ptr<int>ans =  st_stack.wait_and_pop();
            printf("pop data%d\n",*ans.get());
        }
    });
    t1.join();
    t2.join();
}


template <typename T>
class SafeThreadStackWithoutMutex{
private:
    struct StackNode{
        std::shared_ptr<T> data;
        StackNode* next;
        StackNode(T const& data_): data(std::make_shared<T>(data_)){}
    };
    std::atomic<StackNode*>head;
    std::atomic<StackNode*>to_be_deleted;
    std::atomic<int>threads_in_pop;
public:
    SafeThreadStackWithoutMutex(){
        head = nullptr;
        to_be_deleted = nullptr;
        threads_in_pop = 0;
    };
    SafeThreadStackWithoutMutex(const SafeThreadStackWithoutMutex&) =delete;
    SafeThreadStackWithoutMutex& operator=(const SafeThreadStackWithoutMutex&) = delete;

    void push(const T & value){
        StackNode* const new_node = new StackNode(value);
        do{
            new_node->next = head.load(std::memory_order_acquire);
        }
        //compare_exchange_weak 当比较失败的时候不会让exchange的值赋值给compare的值 并且开销也小
        while(!head.compare_exchange_weak(new_node->next,new_node));
    }

    std::shared_ptr<T>pop(){
        ++threads_in_pop;
        StackNode* old_node = nullptr;
        do{
            old_node = head.load(std::memory_order_relaxed);
            if(old_node == nullptr){
                --threads_in_pop;
                return nullptr;
            }
        }while (!head.compare_exchange_weak(old_node,old_node->next));

        std::shared_ptr<T> res;
        if(old_node!= nullptr){
            res.swap(old_node->data);
        }
        try_reclaim(old_node);
        return res;
    }

    void try_reclaim(StackNode* node){
        if(threads_in_pop == 1){
            StackNode* nodes_to_delete = to_be_deleted.exchange(nullptr);
            //如果只有一个线程加入在执行 这时就可以把所有带删除队列都删除掉
            if(--threads_in_pop == 0){
                delete_nodes(nodes_to_delete);
            }
                //此时删除队列还是多线程队列的或本来就没有任务 (另一个线程读取为空的 可能是exchange后的) 就把他放回删除队列
            else if(nodes_to_delete!= nullptr ){
                chain_pending_nodes(nodes_to_delete);
            }
            delete node;
        }
        else{
            //如果多个线程在pop就把这个节点放入删除队列中
            chain_pending_node(node);
            --threads_in_pop;
        }
    }
    static void delete_nodes(StackNode* nodes)
    {
        while (nodes!=nullptr)
        {
            StackNode* next = nodes->next;
            delete nodes;
            nodes = next;
        }
    }
    void chain_pending_node(StackNode* n)
    {
        chain_pending_nodes(n, n);
    }

    void chain_pending_nodes(StackNode* first, StackNode* last)
    {
        //1 先将last的next节点更新为待删列表的首节点
        last->next = to_be_deleted;
        //2  借循环保证 last->next指向正确
        // 将待删列表的首节点更新为first节点
        while (!to_be_deleted.compare_exchange_weak(
                last->next, first));
    }

    void chain_pending_nodes(StackNode* nodes)
    {
        StackNode* last = nodes;
        //1 沿着next指针前进到链表末端
        while (StackNode* const next = last->next)
        {
            last = next;
        }
        //2 将链表放入待删链表中
        chain_pending_nodes(nodes, last);
    }
};



void TestLockFreeStack() {

    SafeThreadStackWithoutMutex<int> lk_free_stack;
    std::set<int>  rmv_set;
    std::mutex set_mtx;

    std::thread t1([&]() {
        for (int i = 0; i < 200; i++) {
            lk_free_stack.push(i);
            std::cout << "push data " << i << " success!" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
    });

    std::thread t2([&]() {
        for (int i = 0; i < 100;) {
            auto head = lk_free_stack.pop();
            if (!head) {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                continue;
            }
            std::lock_guard<std::mutex> lock(set_mtx);
            rmv_set.insert(*head);
            std::cout << "pop data " << *head << " success!" << std::endl;
            i++;
        }
    });

    std::thread t3([&]() {
        for (int i = 0; i < 100;) {
            auto head = lk_free_stack.pop();
            if (!head) {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                continue;
            }
            std::lock_guard<std::mutex> lock(set_mtx);
            rmv_set.insert(*head);
            std::cout << "pop data " << *head << " success!" << std::endl;
            i++;
        }
    });

    t1.join();
    t2.join();
    t3.join();
    std::cout << rmv_set.size() << std::endl;
    assert(rmv_set.size() == 200);
}



struct empty_queue : std::exception{
    const char* what() const throw();
};
template<typename T>
class SafeThreadQue{
private:
    std::queue<T>data;
    mutable std::mutex mtx;
    std::condition_variable condition_empty;
public:
    SafeThreadQue(){};
    SafeThreadQue(const SafeThreadQue& other){
        std::lock_guard<std::mutex>lock(other.mtx);
        data = other.data;
    };

    void push(const T& new_val){
        std::lock_guard<std::mutex>lock(mtx);
        data.push(new_val);
        condition_empty.notify_one();
    }

    std::shared_ptr<T> wait_and_pop(){
        std::unique_lock<std::mutex>lock(mtx);
        condition_empty.wait(lock,[this](){
            return !data.empty();
        });
        std::shared_ptr<T>const res = std::make_shared<T>(std::move(data.front()));
        data.pop();
        return res;
    }
    void wait_and_pop(T& value){
        std::unique_lock<std::mutex>lock(mtx);
        condition_empty.wait(lock,[this](){
            return !data.empty();
        });
        value = std::move(data.front());
        data.pop();
    }
    std::shared_ptr<T>pop(){
        std::lock_guard<std::mutex>lock(mtx);
        if(data.empty()) {return nullptr;}
        std::shared_ptr<T> s_ptr= std::make_shared<T>(data.front());
        data.pop();
        return s_ptr;
    }
    void pop(T& val){
        std::lock_guard<std::mutex>lock(mtx);
        if(data.empty())   throw empty_stack();
        val = data.front();
        data.pop();
    }
    bool empty() const
    {
        std::lock_guard<std::mutex> lock(mtx);
        return data.empty();
    }
};

//使用list来替换
template<typename T>
class threadsafe_queue_ht{
private:
    struct node
    {
        std::shared_ptr<T>data;
        std::unique_ptr<node>next;
    };
    std::mutex head_mutex;
    std::unique_ptr<node>head;
    std::mutex tail_mutex;
    node* tail;
    std::condition_variable data_cond;

    node* get_tail(){
        std::lock_guard<std::mutex>tail_lock(tail_mutex);
        return tail;
    }
    std::unique_ptr<node>pop_head(){
        std::unique_ptr<node>old_head = std::move(head);
        head = std::move(old_head->next);
        return old_head;
    }
    std::unique_lock<std::mutex> wait_for_data(){
        std::unique_lock<std::mutex> head_lock(head_mutex);
        //不为空
        data_cond.wait(head_lock,[&] {return head.get() != get_tail(); }); 
        return std::move(head_lock);   
    }
    std::unique_ptr<node> wait_pop_head()
    {
        std::unique_lock<std::mutex> head_lock(wait_for_data());   
        return pop_head();
    }
    std::unique_ptr<node> wait_pop_head(T& value)
    {
        std::unique_lock<std::mutex> head_lock(wait_for_data());  
        value = std::move(*head->data);
        return pop_head();
    }
    std::unique_ptr<node> try_pop_head()
    {
        std::lock_guard<std::mutex> head_lock(head_mutex);
        if (head.get() == get_tail())
        {
            return std::unique_ptr<node>();
        }
        return pop_head();
    }
    std::unique_ptr<node> try_pop_head(T& value)
    {
        std::lock_guard<std::mutex> head_lock(head_mutex);
        if (head.get() == get_tail())
        {
            return std::unique_ptr<node>();
        }
        value = std::move(*head->data);
        return pop_head();
    }
public:
    //在构造的时候就声明一个空节点
    threadsafe_queue_ht() : head(new node),tail(head.get()){};
    threadsafe_queue_ht(const threadsafe_queue_ht& other) = delete;
    threadsafe_queue_ht& operator=(const threadsafe_queue_ht& other) = delete;
    threadsafe_queue_ht& operator=(const threadsafe_queue_ht& other) volatile = delete;

    std::shared_ptr<T>wait_and_pop(){
        std::unique_ptr<node>const old_head = wait_pop_head();
        return old_head->data;
    }
    void wait_and_pop(T& value){
        std::unique_ptr<node>const old_head = wait_pop_head(value);
    }

    std::shared_ptr<T> try_pop()
    {
        std::unique_ptr<node> old_head = try_pop_head();
        return old_head ? old_head->data : std::shared_ptr<T>();
    }
    bool try_pop(T& value)
    {
        std::unique_ptr<node> const old_head = try_pop_head(value);
        return old_head == nullptr;
    }
    void push(T new_value)  
    {
        std::shared_ptr<T> new_data(
        std::make_shared<T>(std::move(new_value)));
        std::unique_ptr<node> p(new node);
        node* const new_tail = p.get();

        std::lock_guard<std::mutex> tail_lock(tail_mutex);
        tail->data = new_data;
        tail->next = std::move(p);
        tail = new_tail;
        data_cond.notify_one();
    }
    bool empty()
    {
        std::lock_guard<std::mutex> head_lock(head_mutex);
        return (head.get() == get_tail());
    }
};

void test_thread_safe_queue(){
    //SafeThreadQue<int> st_stack;
    threadsafe_queue_ht<int> st_stack;
    std::thread t1([&st_stack](){
        for(int i=0;i<10;++i){
            st_stack.push(i);
            printf("push data%d\n",i);
        }
    });
    std::thread t2([&st_stack](){
        for(int i=0;i<10;++i){
            std::shared_ptr<int>ans =  st_stack.wait_and_pop();
            printf("pop data%d\n",*ans.get());
        }
    });
    t1.join();
    t2.join();
}

template<typename T>
class lock_free_queue
{
private:
    struct node_counter
    {
        unsigned internal_count : 30;
        unsigned external_counters : 2;
    };
    struct node;
    struct counted_node_ptr
    {
        //存在破坏trivial class 的风险
        /*bool operator == (const counted_node_ptr& cnp) {
            return (external_count == cnp.external_count && ptr == cnp.ptr);
        }*/
        //构造初始化各成员
        counted_node_ptr():external_count(0), ptr(nullptr) {}
        int external_count;
        node* ptr;
    };
    struct node
    {
        std::atomic<T*> data;
        std::atomic<node_counter> count;
        std::atomic<counted_node_ptr> next;

        node(int external_count = 2)
        {
            node_counter new_count;
            new_count.internal_count = 0;
            new_count.external_counters = external_count;
            count.store(new_count);

            counted_node_ptr node_ptr;
            node_ptr.ptr = nullptr;
            node_ptr.external_count = 0;

            next.store(node_ptr);
        }


        void release_ref()
        {
            std::cout << "call release ref " << std::endl;
            node_counter old_counter = count.load(std::memory_order_relaxed);
            node_counter new_counter;
            do
            {
                new_counter = old_counter;
                --new_counter.internal_count;
            }
            while (!count.compare_exchange_strong(
                    old_counter, new_counter,
                    std::memory_order_acquire, std::memory_order_relaxed));
            if (!new_counter.internal_count &&
                !new_counter.external_counters)
            {
                delete this;
                std::cout << "release_ref delete success" << std::endl;
                destruct_count.fetch_add(1);
            }
        }
    };

    std::atomic<counted_node_ptr> head;
    //⇽--- 1
    std::atomic<counted_node_ptr> tail;

    // ⇽---  1
    void set_new_tail(counted_node_ptr& old_tail,
                      counted_node_ptr const& new_tail)
    {
        node* const current_tail_ptr = old_tail.ptr;
        // ⇽---  2  此处仅有一个线程能设置tail为new_tail，失败的会更新old_tail为tail的新值
        //  为防止失败的线程重试导致tail被再次更新所以添加了后面的&&判断
        while (!tail.compare_exchange_weak(old_tail, new_tail) &&
               old_tail.ptr == current_tail_ptr);
        // ⇽---  3
        if (old_tail.ptr == current_tail_ptr)
            //⇽---  4
            free_external_counter(old_tail);
        else
            //⇽---  5
            current_tail_ptr->release_ref();
    }

    static void free_external_counter(counted_node_ptr& old_node_ptr)
    {
        std::cout << "call  free_external_counter " << std::endl;
        node* const ptr = old_node_ptr.ptr;
        int const count_increase = old_node_ptr.external_count - 2;
        node_counter old_counter =
                ptr->count.load(std::memory_order_relaxed);
        node_counter new_counter;
        do
        {
            new_counter = old_counter;
            //⇽---  1
            --new_counter.external_counters;
            //⇽---  2
            new_counter.internal_count += count_increase;
        }
            //⇽---  3
        while (!ptr->count.compare_exchange_strong(
                old_counter, new_counter,
                std::memory_order_acquire, std::memory_order_relaxed));
        if (!new_counter.internal_count &&
            !new_counter.external_counters)
        {
            //⇽---  4
            destruct_count.fetch_add(1);
            std::cout << "free_external_counter delete success" << std::endl;
            delete ptr;
        }

    }


    static void increase_external_count(
            std::atomic<counted_node_ptr>& counter,
            counted_node_ptr& old_counter)
    {
        counted_node_ptr new_counter;
        do
        {
            new_counter = old_counter;
            ++new_counter.external_count;
        } while (!counter.compare_exchange_strong(
                old_counter, new_counter,
                std::memory_order_acquire, std::memory_order_relaxed));
        old_counter.external_count = new_counter.external_count;
    }

public:
    lock_free_queue() {

        counted_node_ptr new_next;
        new_next.ptr = new node();
        new_next.external_count = 1;
        tail.store(new_next);
        head.store(new_next);
        std::cout << "new_next.ptr is " << new_next.ptr << std::endl;
    }

    ~lock_free_queue() {
        while (pop());
        auto head_counted_node = head.load();
        delete head_counted_node.ptr;
    }

    void push(T new_value)
    {
        std::unique_ptr<T> new_data(new T(new_value));
        counted_node_ptr new_next;
        new_next.ptr = new node;
        new_next.external_count = 1;
        counted_node_ptr old_tail = tail.load();
        for (;;)
        {
            increase_external_count(tail, old_tail);
            T* old_data = nullptr;
            //⇽---  6
            if (old_tail.ptr->data.compare_exchange_strong(
                    old_data, new_data.get()))
            {
                counted_node_ptr old_next;
                counted_node_ptr now_next = old_tail.ptr->next.load();
                //⇽---  7
                if (!old_tail.ptr->next.compare_exchange_strong(
                        old_next, new_next))
                {
                    //⇽---  8
                    delete new_next.ptr;
                    new_next = old_next;   // ⇽---  9
                }
                set_new_tail(old_tail, new_next);
                new_data.release();
                break;
            }
            else    // ⇽---  10
            {
                counted_node_ptr old_next ;
                // ⇽--- 11
                if (old_tail.ptr->next.compare_exchange_strong(
                        old_next, new_next))
                {
                    // ⇽--- 12
                    old_next = new_next;
                    // ⇽---  13
                    new_next.ptr = new node;
                }
                //  ⇽---  14
                set_new_tail(old_tail, old_next);
            }
        }

        construct_count++;
    }


    std::unique_ptr<T> pop()
    {
        counted_node_ptr old_head = head.load(std::memory_order_relaxed);
        for (;;)
        {
            increase_external_count(head, old_head);
            node* const ptr = old_head.ptr;
            if (ptr == tail.load().ptr)
            {
                //头尾相等说明队列为空，要减少内部引用计数
                ptr->release_ref();
                return std::unique_ptr<T>();
            }
            //  ⇽---  2
            counted_node_ptr next = ptr->next.load();
            if (head.compare_exchange_strong(old_head, next))
            {
                T* const res = ptr->data.exchange(nullptr);
                free_external_counter(old_head);
                return std::unique_ptr<T>(res);
            }
            ptr->release_ref();
        }
    }

    static std::atomic<int> destruct_count;
    static std::atomic<int> construct_count;
};

template<typename T>
std::atomic<int> lock_free_queue<T>::destruct_count = 0;

template<typename T>
std::atomic<int> lock_free_queue<T>::construct_count = 0;


int  main(){
//    test_thread_safe_stack();
//    test_thread_safe_queue();
//    TestLockFreeStack();
    
    return 0;
}