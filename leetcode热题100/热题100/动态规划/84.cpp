#include "util.h"


/*
279.完全平方数
给你一个整数 n ，返回 和为 n 的完全平方数的最少数量 。

完全平方数 是一个整数，其值等于另一个整数的平方；换句话说，其值等于一个整数自乘的积。
例如，1、4、9 和 16 都是完全平方数，而 3 和 11 不是。
输入：n = 12
输出：3 
解释：12 = 4 + 4 + 4
*/

//我们需要遍历的数是在0-sqrt(n)+1之间
//dp[i] 表示大小为i的值的完全平方数的最小数量
//dp[i] = 1 + min(dp[i-j*j]) j为选择j*j来表示 其中j属于 0-sqrt(i)


class Solution {
public:
    int numSquares(int n) {
        vector<int>dp(n+1,0);
        dp[0] = 0;
        dp[1] = 1;
        if(n<=1){
            return dp[n];
        }

        
        for(int id =2;id<=n;id++){
            int min_ = INT_MAX;
            for(int j=1;j*j<=id;j++){
                min_ = min(min_,dp[id-j*j]);
            }
            dp[id] = 1+ min_;
        }

        return dp[n];
    }
};