
#include "util.h"

/*


152. 乘积最大子数组

给你一个整数数组 nums ，请你找出数组中乘积最大的非空连续子数组（该子数组中至少包含一个数字），并返回该子数组所对应的乘积。

测试用例的答案是一个 32-位 整数。

子数组 是数组的连续子序列。
输入: nums = [2,3,-2,4]
输出: 6
解释: 子数组 [2,3] 有最大乘积 6。
 
*/

class Solution {
public:
    void swap(int &a,int &b){
        int temp = a;
        a = b;
        b = temp;
    }
    int maxProduct(vector<int>& nums) {
        int max_ans = nums[0];
        int min_ans = nums[0];
        int res = max_ans;
        if(nums.size() == 1){return max_ans;}
        for(int id=1;id<nums.size();id++){
            if(nums[id]<0){
                swap(max_ans,min_ans);
            } 
            max_ans = max(max_ans*nums[id],nums[id]);
            res = max(res,max_ans);
            min_ans = min(min_ans*nums[id],nums[id]);
        }


        return res;

    }
};


