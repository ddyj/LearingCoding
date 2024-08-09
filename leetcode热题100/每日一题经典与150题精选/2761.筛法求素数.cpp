#include"util.h"


/*
    给你一个整数 n 。如果两个整数 x 和 y 满足下述条件，则认为二者形成一个质数对：

1 <= x <= y <= n
x + y == n
x 和 y 都是质数
请你以二维有序列表的形式返回符合题目要求的所有 [xi, yi] ，列表需要按 xi 的 非递减顺序 排序。如果不存在符合要求的质数对，则返回一个空数组。

注意：质数是大于 1 的自然数，并且只有两个因子，即它本身和 1 。

*/


/*
筛法：https://oi-wiki.org/math/number-theory/sieve/#%E8%BF%87%E7%A8%8B
其思想是 如果i是素数 那么从i往上含有i的因子的数一定是合数

*/

class Solution {
private:
    bool is_Prime[1000001];

public:
    void init_Prine(int n){
        is_Prime[0] = false;
        is_Prime[1] = false;
        for(int i=2;i<=n;i++){is_Prime[i]=true;}
        for(int i=2;i<=n;i++){
            if(is_Prime[i]){
                if((long long)i*i>n){continue;}
                //为什么从i*i开始
                //因为2*i -- (i-1)*i 在前面已经遍历过了
                for(int j=i*i;j<=n;j+=i){
                    is_Prime[j] = false;
                }
            }

        }
    }

    vector<vector<int>> findPrimePairs(int n) {
        vector<vector<int>>res;
        init_Prine(n);

        for (int i = 2; i <= n / 2; i++) {
            if (is_Prime[i] && is_Prime[n - i]) {
                res.push_back({i, n - i});
            }
        }




        return res;
    }
};