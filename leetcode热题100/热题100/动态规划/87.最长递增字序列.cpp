

#include "util.h"
/*
给你一个整数数组 nums ，找到其中最长严格递增子序列的长度。

子序列 是由数组派生而来的序列，删除（或不删除）数组中的元素而不改变其余元素的顺序。
例如，[3,6,2,7] 是数组 [0,3,1,6,2,2,7] 的子序列。

 
输入：nums = [10,9,2,5,3,7,101,18]
输出：4
解释：最长递增子序列是 [2,3,7,101]，因此长度为 4 
*/


/*
    用dp[i]表示nums[i]结尾的最长子序列的长度 -- 这里不一定是从0开始的 是取决于j的遍历的
    例如 如果dp[i]在遍历结束后 取dp[j]+1 则表示的是从j-i的最长的字序列
    
    递推原理：
    如果一个较大的数接在较小的数后面，就会形成一个更长的子序列。
    dp[i] = max(dp[j]+1) j from(0,i)
*/
class Solution {
public:
    int lengthOfLIS(vector<int>& nums) {
        vector<int>dp(nums.size(),1);
        if(nums.size()==1){return dp[0];}
        for(int id=1;id<nums.size();id++){
            for(int j=0;j<id;j++){
                if(nums[j]<nums[id]){dp[id] =max(dp[j]+1 , dp[id]) ;}
            }
        }
        int res = dp[0];
        for(int id=1;id < dp.size();id++){
            res = max(res,dp[id]);
        }

        return res;
    }
};