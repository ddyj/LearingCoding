#ifndef __AUTHOR_THREADSAFE_TABLE_H__
#define __AUTHOR_THREADSAFE_TABLE_H__


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
        //在链表中查找
        bucket_iterator find_entry_for(const Key & key){
            return std::find_if(data.begin(),data.end(),[&]
                    (bucket_value const& item){
                return item.first == key;
            });
        }
    public:
        //查找key值 返回对应的value
        Value value_for(Key const& key,Value const& default_value){
            std::shared_lock<std::shared_mutex>lock(s_mtx);
            bucket_iterator const found_iterator = find_entry_for(key);
            return (found_iterator == data.end()) ? default_value : found_iterator->second;
        }
        //添加key和value
        void add_or_update_mapping(Key const& key, Value const& value){
            std::unique_lock<std::shared_mutex>lock(s_mtx);
            bucket_iterator const found_iterator = find_entry_for(key);
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
                    std::unique_lock<std::shared_mutex>(buckets[i]->s_mtx));
        }
        std::map<Key,Value>res;
        for(unsigned int i = 0;i< buckets.size();++i){
            typename bucket_type::bucket_iterator it = buckets[i]->data.begin();
            for(;it!=buckets[i]->data.end();++it){
                res.insert(*it);
            }
        }
        return res;
    }

};

#endif