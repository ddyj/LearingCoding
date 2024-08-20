
#include "util.h"

/*
我们将这三个题放在一起  这两个题目对于动态规划的递推是一致的
都是对于一个长度为i的结构 从后往前找
如单词拆分中 对于字符长度是 i 我们从i-word_len[st]为起点 word_len[st]为长度检索 如当前是dog Word = {o og}则检索 og = o / og
                                                     使得 dp[i] = 1 + dp[i-word_len]
在完全平方数中 对于当前值是i的数 我们选择所有j*j<i的j方法中的最小值(贪心) 使得 dp[i] = 1 + min_all(dp[i-j*j]) 
在找零钱中 对于当前位i的零钱 我们遍历所有的coins 使得dp[i] = 1 + min_all(dp[i-j]);
*/


/*
139 单词拆分
给你一个字符串 s 和一个字符串列表 wordDict 作为字典。请你判断是否可以利用字典中出现的单词拼接出 s 。

注意：不要求字典中出现的单词全部都使用，并且字典中的单词可以重复使用。
输入: s = "leetcode", wordDict = ["leet", "code"]
输出: true
解释: 返回 true 因为 "leetcode" 可以由 "leet" 和 "code" 拼接成。
*/
class Solution {
private:
    unordered_map<string,int>word_len;
public:
    bool wordBreak(string s, vector<string>& wordDict) {
        for(string st:wordDict){
            word_len[st] = st.size();
        }
        
        //dp[i] 字节长度为i 的字符能否被字典表示
        vector<bool>dp(s.size()+1,false);
        dp[0] = true;
        for(int cur_id = 1;cur_id<=s.size();cur_id++){
            for(string st:wordDict){
                int temp = cur_id - word_len[st];
                if( temp < 0){continue;}
                //对于长度是i的字符串 我们从后往前切子字符串
                string sub = s.substr(temp,word_len[st]);
                if(sub == st){
                    //为了放置其他错误组合的污染 我们这里用取交的方式处理
                    dp[cur_id] =dp[cur_id] | dp[temp];
                }
            }
        }
        return dp[s.size()];

    }
};


/*
322.零钱兑换
给你一个整数数组 coins ，表示不同面额的硬币；以及一个整数 amount ，表示总金额。

计算并返回可以凑成总金额所需的 最少的硬币个数 。如果没有任何一种硬币组合能组成总金额，返回 -1 。

你可以认为每种硬币的数量是无限的。
输入：coins = [1, 2, 5], amount = 11
输出：3 
解释：11 = 5 + 5 + 1
*/

//dp[amount]表示当价格是amount的时候找钱需要的硬币数
//dp[i] = 1 + min(dp[i-coins1],dp[i-coins2],...)


class Solution {
public:
    int coinChange(vector<int>& coins, int amount) {
        vector<int>dp(amount+1,-1);
        dp[0] = 0;
        for(int id=1;id<=amount;id++){
            int minn = INT_MAX;
            for(int j=0;j<coins.size();j++){
                if(id - coins[j] >=0 && dp[id-coins[j]] != -1){
                    minn = min(minn,dp[id-coins[j]]);
                }

            }
            if(minn!=INT_MAX && minn!=-1){
                 dp[id] = 1 + minn;
            }
        }
        return dp[amount];
    }
};

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