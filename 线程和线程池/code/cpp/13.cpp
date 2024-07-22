#include<iostream>

#include<thread>
#include <algorithm>

#include<mutex>
#include<memory>
#include <condition_variable>
#include<exception>
#include <iterator>
#include <map>
#include <list>
#include <vector>
#include <shared_mutex>
#include <set>


template<typename Key,typename Value,typename Hash = std::hash<Key>>
class threadsafe_lookup_table{
private:
    class bucket_type{
        friend  class threadsafe_lookup_table;
    private:
        //存储元素的类型为pair，由key和value构成
        typedef std::pair<Key,Value>bucket_value;
        //构建链表存储元素
        typedef std::list<bucket_value>bucket_data;
        //链表的迭代器
        typedef typename bucket_data::iterator bucket_iterator;
        //链表数据
        bucket_data data;

        //这里使用共享锁
        mutable std::shared_mutex s_mtx;
        //在链表中查找对应元素
        bucket_iterator find_entry_for(const Key & key){
            return std::find_if(data.begin(),data.end(),[&]
                    (bucket_value const& item){
                return item.first == key;
            });
        }
    public:
        //查找key值 返回对应的value 如果没找到 就返回默认值
        //这里可以学习一下这个思路 当需要更新值的时候添加一个 默认值作为是否查找到的判断
        Value value_for(Key const& key,Value const& default_value){
            std::shared_lock<std::shared_mutex>lock(s_mtx);
            const bucket_iterator  found_iterator = find_entry_for(key);
            return (found_iterator == data.end()) ? default_value : found_iterator->second;
        }
        //添加/更新key和value
        void add_or_update_mapping(Key const& key, Value const& value){
            std::unique_lock<std::shared_mutex>lock(s_mtx);
            const bucket_iterator found_iterator = find_entry_for(key);
            if(found_iterator == data.end()){
                data.emplace_back(key,value);
            }
            else{
                found_iterator->second = value;
            }
        };
        //删除key和value
        void remove_mapping(Key const& key) {
            std::unique_lock<std::shared_mutex> lock(s_mtx);
            bucket_iterator const found_iterator = find_entry_for(key);
            if (found_iterator != data.end()) {
                data.erase(found_iterator);
            }
        }
    };
    std::vector<std::unique_ptr<bucket_type>>buckets;
    Hash hasher;
    //计算哈希值 并和容器size取模 放置到对应的位置上
    bucket_type& get_bucket(Key const& key)const{
        std::size_t const bucket_index = hasher(key) % buckets.size();
        return *buckets[bucket_index];
    }

public:
    threadsafe_lookup_table(unsigned num_buckets = 20,Hash const& hasher_ = Hash()):
        buckets(num_buckets),hasher(hasher_) {
        for(unsigned i = 0; i < num_buckets;++i){
            buckets[i].reset(new bucket_type);
        }
    }
    threadsafe_lookup_table(threadsafe_lookup_table const& other) = delete ;
    threadsafe_lookup_table&operator=(const threadsafe_lookup_table&) = delete;

    Value value_for(Key const& key,Value const& default_value = Value()){
        return get_bucket(key).value_for(key,default_value);
    }

    void add_or_update_mapping(Key const& key,Value const& value){
        get_bucket(key).add_or_update_mapping(key,value);
    }
    void remove_mapping(Key const& key){
        get_bucket(key).remove_mapping(key);
    }

    std::map<Key,Value>get_map(){
        std::vector<std::unique_lock<std::shared_mutex>> locks;
        for(unsigned int i = 0; i < buckets.size();++i){
            locks.push_back(
                    std::unique_lock<std::shared_mutex>(buckets[i]->s_mtx,std::defer_lock));
        }
        std::map<Key,Value>res;
        for(unsigned int i = 0;i< buckets.size();++i){
            locks[i].lock();
            typename bucket_type::bucket_iterator it = buckets[i]->data.begin();
            for(;it!=buckets[i]->data.end();++it){
                res.insert(*it);
            }
            locks[i].unlock();
        }
        return res;
    }
};
class MyClass
{
public:
    MyClass(int i):_data(i){}
    friend std::ostream& operator << (std::ostream& os, const MyClass& mc){
        os << mc._data;
        return os;
    }
private:
    int _data;
};
void test_hash_thread(){
    std::set<int> removeSet;
    threadsafe_lookup_table<int, std::shared_ptr<MyClass>> table;
    std::thread t1([&]() {
        for(int i = 0; i < 100; i++)
        {
            auto class_ptr =  std::make_shared<MyClass>(i);
            table.add_or_update_mapping(i, class_ptr);
        }
    });
    std::thread t2([&]() {
        for (int i = 0; i < 100; )
        {
            auto find_res = table.value_for(i, nullptr);
            if(find_res)
            {
                table.remove_mapping(i);
                removeSet.insert(i);
                i++;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    });
    std::thread t3([&]() {
        for (int i = 100; i < 200; i++)
        {
            auto class_ptr = std::make_shared<MyClass>(i);
            table.add_or_update_mapping(i, class_ptr);
        }
    });
    t1.join();
    t2.join();
    t3.join();
    for(auto & i : removeSet)
    {
        std::cout << "remove data is " << i << std::endl;
    }
    auto copy_map =  table.get_map();
    for(auto & i : copy_map)
    {
        std::cout << "copy data is " << *(i.second) << std::endl;
    }
}
int main(){

    test_hash_thread();
    
    return 0;
}