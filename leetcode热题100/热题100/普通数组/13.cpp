
#include "util.h"


/*
给你一个整数数组 nums ，请你找出一个具有最大和的连续子数组（子数组最少包含一个元素），返回其最大和。
子数组 是数组中的一个连续部分。

示例 1：

输入：nums = [-2,1,-3,4,-1,2,1,-5,4]
输出：6
解释：连续子数组 [4,-1,2,1] 的和最大，为 6 。
 
*/

class Solution {
public:
    int maxSubArray(vector<int>& nums) {
        //无后效性 动态规划  注意动态规划的递推式是如何重新划分区间的
        if(1){
            vector<int>dp(nums.size());
            dp[0] = nums[0];
            for(int id=1;id<nums.size();id++){
                dp[id] = max(dp[id-1]+nums[id],nums[id]);
            }
            sort(dp.begin(),dp.end());
            return dp[nums.size()-1]; 
        }
        
        if(1){
            //动态规划修改 这里我们不需要用vector存储 由于题目只需要最大值 所以是一个0维的动态规划
            int maxSubArray(vector<int>& nums) {
                int max_dp = nums[0];
                int res = max_dp;
                for(int id = 1;id<nums.size();id++){
                    max_dp = max(max_dp+nums[id],nums[id]);
                    res = max(res,max_dp);
                }
                return res;
            }
        }

        //贪心
        //当前和如果是负数 当前“连续和”为负数的时候立刻放弃，
        //从下一个元素重新计算“连续和”，因为负数加上下一个元素 “连续和”只会越来越小。
        if(0){
            int cur_sum = 0;
            int max_sum = INT_MIN;
            for(int id=0;id<nums.size();id++){
                cur_sum +=nums[id];
                //不断检索最大的cur_sum
                if(cur_sum > max_sum){
                    max_sum = cur_sum;
                }
                if(cur_sum < 0){
                    cur_sum = 0;
                }
            }
            return max_sum;  
        }

    }
};


////无后效性 动态规划  
//同类型题目：

/**
152. 乘积最大子数组
给你一个整数数组 nums ，请你找出数组中乘积最大的非空连续子数组（该子数组中至少包含一个数字），并返回该子数组所对应的乘积。
测试用例的答案是一个 32-位 整数。
子数组 是数组的连续子序列。
示例 1:

输入: nums = [2,3,-2,4]
输出: 6
解释: 子数组 [2,3] 有最大乘积 6。

* 
*/

// dp 子序列乘积最大的值
// 由于这个题中存在负数的可能 需要同时维护两个dp 一个是最大值 一个是最小值
// 当出现负值的时候 最大和最小值互换
// dp = (dp * nums ,nums)
class Solution {
public:
    int maxProduct(vector<int>& nums) {
        if(nums.size() == 1){return nums[0];}
        if(nums.size() == 2 ){return max(max(nums[0]*nums[1],nums[1]),nums[0]);}
        else{
            int dp_max = nums[0];
            int dp_min = nums[0];
            int res = dp_max;
            for(int id=1;id<nums.size();id++){
                if(nums[id] > 0){
                   dp_max = max(nums[id]*dp_max ,nums[id]);
                   dp_min = min(nums[id]*dp_min,nums[id]); 
                   res = max(dp_max,res);
                }
                else{
                    int temp = dp_max;
                    dp_max = dp_min;
                    dp_min = temp;

                    dp_max = max(nums[id]*dp_max ,nums[id]);
                    dp_min = min(nums[id]*dp_min,nums[id]); 
                    res = max(dp_max,res);
                }       
            }
            return res;
        }
        
    }
};