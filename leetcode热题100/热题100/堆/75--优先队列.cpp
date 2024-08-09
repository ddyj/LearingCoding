#include"util.h"

/*
347 前k个高频元素

给你一个整数数组 nums 和一个整数 k ，
请你返回其中出现频率前 k 高的元素。你可以按 任意顺序 返回答案。 

示例 1:

输入: nums = [1,1,1,2,2,3], k = 2
输出: [1,2]
*/

//这里使用纯手写的堆  实际应用中可以使用priority_queue优先队列存储

class Solution {
public:
    void myswap(vector<pair<int,int>>&heap,int pose1,int pose2){
        if(pose1 == pose2){return;}
        pair<int,int> tmp = heap[pose1];
        heap[pose1] = heap[pose2];
        heap[pose2] = tmp;
    }


    void heapify(vector<pair<int,int>>&heap,int pose,int len){
        int maxpose = pose;
        int lpose = pose*2 +1;
        int rpose = pose*2+2;
        if(lpose<len && heap[maxpose].second < heap[lpose].second){maxpose = lpose;}
        if(rpose<len && heap[maxpose].second < heap[rpose].second){maxpose = rpose;}
        if(maxpose!=pose){
            myswap(heap,maxpose,pose);
            heapify(heap,maxpose,len);
        }
    }


    void heapsort(vector<pair<int,int>>&heap,vector<int>&res,int k,int len){
        for(int i= (len-1-1)/2 ;i>=0 ;i--){
            heapify(heap,i,len);
        }




        int count = 0;
        for(int i=len-1;i>=0;i--){
            res.emplace_back(heap[0].first);
            count++;
            //std::cout<<count<<" "<<i<<std::endl;
            if(count>=k){return;}
            myswap(heap,0,i);
            heapify(heap,0,i);
        }
       
    }


    vector<int> topKFrequent(vector<int>& nums, int k) {
        vector<int>res;
        unordered_map<int,int>hash_table;
        vector<pair<int,int>>heap;
        for(int c: nums){
            hash_table[c]++;
        }
        for(auto it:hash_table){
            heap.emplace_back(make_pair(it.first,it.second));
        }
        heapsort(heap,res,k,heap.size());
        return res;
    }

//使用优先队列解法
private:
    struct my_cmp{
        bool operator()(pair<int,int>&a,pair<int,int>&b){
            return a.second < b.second;
        }
    };


public:
    vector<int> topKFrequent(vector<int>& nums, int k) {
        vector<int>res;
        unordered_map<int,int>hash_table;
        priority_queue<pair<int,int>,vector<pair<int,int>>,my_cmp>heap;
        for(int c: nums){
            hash_table[c]++;
        }
        for(auto it:hash_table){
            heap.push(make_pair(it.first,it.second));
        }
        //debug
        // while(!heap.empty()){
        //     std::cout<<heap.top().first<<" "<<heap.top().second<<std::endl;
        //     heap.pop();
        // }


        for(int i=0;i<k;i++){
            res.emplace_back(heap.top().first);
            heap.pop();
        }
        return res;
    }
};




//使用优先队列的模式：  学会如何使用优先队列


/*
    692 前k个高频词
    给定一个单词列表 words 和一个整数 k ，返回前 k 个出现次数最多的单词。
    返回的答案应该按单词出现频率由高到低排序。如果不同的单词有相同出现频率， 按字典顺序 排序。
*/



class Solution {
private:
    //注意优先队列 priority_queue的自定义比较函数是通过struct写operator实现的
    struct my_cmp{
        bool operator()(pair<string,int> &a , pair<string,int> &b)
        {
            return  a.second == b.second ?  a.first < b.first : a.second > b.second;
        }
    };

public:

    //方法一 手写堆实现
    void my_swap(vector<pair<string,int>>&table,int pose1,int pose2){
        if(pose1 == pose2){return;}
        auto tmp = table[pose1];
        table[pose1] = table[pose2];
        table[pose2] = tmp;
        return;
    }
    bool is_change(pair<string,int>a,pair<string,int>b){
        if(a.second > b.second){return true;}
        else if(a.second == b.second){
            if(a.first < b.first){return true;}
            else{return false;}
        }
        else{
            return false;
        }
        return true;
        //return a.second == b.second ? a.first < b.first : a.second > b.second;
    }
    void heapify(vector<pair<string,int>>&table,int len,int pose){
        int max_pose = pose;
        int lpose = pose*2+1;
        int rpose = pose*2+2;

        if(lpose<len && is_change(table[lpose],table[max_pose])){
            max_pose = lpose;
        }
        if(rpose<len && is_change(table[rpose],table[max_pose])){
            max_pose = rpose;
        }
        if(max_pose!=pose){
            my_swap(table,max_pose,pose);
            heapify(table,len,max_pose);
        }
    }



    void heapsort(vector<pair<string,int>>&table,int len, vector<string> &res,int k){
        for(int i = (len-1-1)/2 ; i>=0; i--){
            heapify(table,len,i);
        }

        for(int i=len-1;i>=0;i--){
            
            res.emplace_back(table[0].first);

            my_swap(table,0,i);
            heapify(table,i,0);
 
            if(res.size() >= k){
                return;
            }
        }
    }
    vector<string> topKFrequent(vector<string>& words, int k) {
        vector<string>res;
        unordered_map<string,int>hash_table;
        for(string str:words){
            hash_table[str]++;
        }

        vector<pair<string,int>>table;
        for(auto it : hash_table){
            table.emplace_back(make_pair(it.first,it.second));
        }

        int len = table.size();
        heapsort(std::ref(table),len,std::ref(res),k);
        return res;
    }

    //方法二 借助优先队列 prioirty_queue实现 注意priority_queue的自定义比较函数实现
    //参考文档 https://blog.csdn.net/AAMahone/article/details/82787184
    //        https://zhuanlan.zhihu.com/p/478887055
    vector<string> topKFrequent(vector<string>& words, int k) {
        vector<string>res(k);
        unordered_map<string,int>hash_table;
        for(string str:words){
            hash_table[str]++;
        }
        //优先队列的声明方式  vector<pair<string,int>>表示底层   my_cmp 表示自定义排序方式

        //默认的两种方式  对于pair<int,int>的形式 先比第一个 再比第二个 
        //1 升序 由小到大 最小元素在队首
        //priority_queue<int,vector<int>,greater<int>>q;
        //2 降序 由大到小 最大元素在队首
        //priority_queue<int,vector<int>,less<int>>q;


        priority_queue<pair<string,int>, vector<pair<string,int>> , my_cmp>heap;
        for(auto it : hash_table){
            heap.push(make_pair(it.first,it.second));
        }


        //debug  在这种写法下 heap是一个递增的 有小到大
        // while(!heap.empty()){
        //     std::cout<<heap.top().first<<" "<<heap.top().second<<std::endl;
        //     heap.pop();
        // }
        while(heap.size() > k){
            heap.pop();
        }
        for(int i=k-1;i>=0;i--){
            res[i] = heap.top().first;
            heap.pop();
        }

        return res;
    }
};