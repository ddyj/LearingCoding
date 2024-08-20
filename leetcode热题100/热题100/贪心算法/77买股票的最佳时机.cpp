#include "util.h"


/*
参考网址：
https://leetcode.cn/circle/discuss/qiAgHn/
*买卖股票的最佳时机 0维的dp -- 只用维护最大最小两个值就可以
*买卖股票的最佳时机II 贪心 -- 由于每一天可以买入卖出 那么每天赚一点和最后总赚的价格是一样的 详细分析见这个题的注释

*/


/*
121.买股票的最佳时机

给定一个数组 prices ，它的第 i 个元素 prices[i] 表示一支给定股票第 i 天的价格。

你只能选择 某一天 买入这只股票，并选择在 未来的某一个不同的日子 卖出该股票。
设计一个算法来计算你所能获取的最大利润。

返回你可以从这笔交易中获取的最大利润。如果你不能获取任何利润，返回 0 。
*/
class Solution {
public:
    int maxProfit(vector<int>& prices) {
        int minn = prices[0];
        int res = 0;
        for(int id=1;id<prices.size();id++){
            if(prices[id] < minn){
                minn = prices[id];
            }
            else{
                res = max(res,prices[id]-minn);
            }
        }
        return res;
    }
};

/*
122 买卖股票的最佳时机2
给你一个整数数组 prices ，其中 prices[i] 表示某支股票第 i 天的价格。

在每一天，你可以决定是否购买和/或出售股票。
你在任何时候 最多 只能持有 一股 股票。你也可以先购买，然后在 同一天 出售。

返回 你能获得的 最大 利润 。
*/

//贪心 只要有利就卖出 
/*
 只要今天的股价大于昨天的 就赚钱
 最极端的情况 1234567
 每一天买入第二天卖出 每一天转1块 和最后一天都卖出转6块是一样的
*/

class Solution {
public:
    int maxProfit(vector<int>& prices) {
        int len = prices.size();
        if(len<=1){return 0;}
        int res = 0;
        for(int id=1;id<len;id++){
            int diff = prices[id] - prices[id-1];
            if(diff > 0){res+=diff;}
        }
        return res;
    }
};