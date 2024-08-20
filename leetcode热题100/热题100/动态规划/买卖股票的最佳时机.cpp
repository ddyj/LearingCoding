#include "util.h"

/*
给你一个整数数组 prices ，其中 prices[i] 表示某支股票第 i 天的价格。

在每一天，你可以决定是否购买和/或出售股票。你在任何时候 最多 只能持有 一股 股票。你也可以先购买，然后在 同一天 出售。

返回 你能获得的 最大 利润 。


输入：prices = [7,1,5,3,6,4]
输出：7
*/

class Solution {
public:

    //贪心
    int maxProfit01(vector<int>& prices) {

        int res = 0;
        int len = prices.size();
        for(int i = 1;i<len;i++){
            if(prices[i] - prices[i-1] >0){
                res += prices[i] - prices[i-1];
            }
        }

        return res;
    }

    //动态规划
    int maxProfit(vector<int>& prices) {
        int len = prices.size();

        //dp[i][j]表是第i天持有状态位i的股票可以获得的最大金额 
        //0代表出售  如果没有买入状态1 那么什么也不用做
        vector<vector<int>>dp(len,vector<int>(2,0));
        dp[0][0] = 0;
        dp[0][1] = -prices[0];

        for(int i=1;i<len;i++){
            dp[i][0] = max(dp[i-1][1] + prices[i] , dp[i-1][0]);
            dp[i][1] = max(dp[i-1][1],dp[i-1][0]-prices[i]);
        }



        return max(dp[len-1][0],dp[len-1][1]);
    }
};