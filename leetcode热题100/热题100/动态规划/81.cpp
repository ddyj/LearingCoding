

#include "util.h"
/*
70.爬楼梯
假设你正在爬楼梯。需要 n 阶你才能到达楼顶。

每次你可以爬 1 或 2 个台阶。你有多少种不同的方法可以爬到楼顶呢？

输入：n = 2
输出：2
解释：有两种方法可以爬到楼顶。
1. 1 阶 + 1 阶
2. 2 阶
*/


class Solution {
public:
    int climbStairs(int n) {
        vector<int>dp(n+1,0);
        if(n==1){return 1;}
        if(n==2){return 2;}
        dp[1] = 1;
        dp[2] = 2;
        
        for(int id=3;id<=n;id++){
            dp[id] = dp[id-2] + dp[id-1];
        }

        return dp[n];
    }
};