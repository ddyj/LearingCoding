#include"util.h"

/*
有些数的素因子只有 3，5，7，
请设计一个算法找出第 k 个数。
注意，不是必须有这些素因子，而是必须不包含其他的素因子。
例如，前几个数按顺序应该是 1，3，5，7，9，15，21。

示例 1:

输入: k = 5

输出: 9

*/

class Solution {
private:
    unordered_set<int>hash_table;   //放置入堆元素重复
    priority_queue<long,vector<long>,greater<long>>heap; // 最小堆   优先队列的实现本质上就是一个堆
public:
    //丑数2 三指针法
    /*
        关于三指针法的理解 
        反过来说也是一样的，一个丑数 x3 / x5 / x7 就会得到某一个更大的丑数。

        如果把丑数数列叫做 ugly[i]，那么考虑一下三个数列：
        1. ugly[0]*3,ugly[1]*3,ugly[2]*3,ugly[3]*3,ugly[4]*3,ugly[5]*3……
        2. ugly[0]*5,ugly[1]*5,ugly[2]*5,ugly[3]*5,ugly[4]*5,ugly[5]*5……
        3. ugly[0]*7,ugly[1]*7,ugly[2]*7,ugly[3]*7,ugly[4]*7,ugly[5]*7……

        我们最终输出的数列是将 1 2 3合并成一个数列 保证他是有小变大的过程
        -- 穿针引线法  由于有三个数组 这里就用三个线头等着被串
        -- 由于会出现重复项目 如3*5 和 5*3 之类 所以这里用if 不用else if


    */
    int getKthMagicNumber(int k) {
        vector<int>dp(k,0);
        dp[0] = 1;
        int p1 = 0;
        int p2 = 0;
        int p3 = 0;

        for(int i=1;i<k;i++){
            dp[i] = min(dp[p1]*3,min(dp[p2]*5,dp[p3]*7));
            if(dp[i]==dp[p1]*3){p1++;}
            if(dp[i]==dp[p2]*5){p2++;}
            if(dp[i]==dp[p3]*7){p3++;}
        }
        return dp[k-1];
    }

    //入堆做法  每次出堆顶元素--min 入堆 min*3 *5 *7  为防止重复 这里就用一个哈希去重
    int getKthMagicNumber(int k) {
        heap.push(1L);
        hash_table.insert(1L);
        int count = 0;
        int res = 0;
        vector<int>mutil = {3,5,7};
        while(count < k){
            long cur = heap.top();
            res = static_cast<int>(cur);
            heap.pop();

            for(int a : mutil){
                long next = cur * a;
                if(hash_table.count(next)==0){
                    heap.push(next);
                    hash_table.insert(next);
                }
            }
         

            //hash_table.erase(tmp);
            count++;

        }
        return res;
    }
};