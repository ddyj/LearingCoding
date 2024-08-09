#include"util.h"


//15. 三数之和

/*
给你一个整数数组 nums ，判断是否存在三元组 [nums[i], nums[j], nums[k]] 满足 i != j、i != k 且 j != k ，同时还满足 nums[i] + nums[j] + nums[k] == 0 。请

你返回所有和为 0 且不重复的三元组。

注意：答案中不可以包含重复的三元组。

 

 

示例 1：

输入：nums = [-1,0,1,2,-1,-4]
输出：[[-1,-1,2],[-1,0,1]]
解释：
nums[0] + nums[1] + nums[2] = (-1) + 0 + 1 = 0 。
nums[1] + nums[2] + nums[4] = 0 + 1 + (-1) = 0 。
nums[0] + nums[3] + nums[4] = (-1) + 2 + (-1) = 0 。
不同的三元组是 [-1,0,1] 和 [-1,-1,2] 。
注意，输出的顺序和三元组的顺序并不重要。
*/

//双指针 定一议二模式 确定一个id 从id+1 到 size-1做双指针遍历
//难点是如何去重 这部分参看注释

class Solution {
public:
    vector<vector<int>> threeSum(vector<int>& nums) {
        vector<vector<int>>res;
        sort(nums.begin(),nums.end());
        for(int id=0;id<nums.size();id++){
            //去重 如果当前值和上一个值一样 就不需要计算这个数字
            if(id>=1&&nums[id]==nums[id-1]){
                continue;
            }
            //双指针计算当前id往右所有合适的解
            int left = id+1;
            int right = nums.size()-1;
            vector<int>temp(3,0);
            while(left<right){
                if(nums[id] + nums[left] + nums[right] == 0){
                    temp[0] = nums[id];
                    temp[1] = nums[left];
                    temp[2] = nums[right];
                    res.emplace_back(temp);

                    //更新left和right也有技巧 
                    //为防止重复 需要更新的left和right指向的值和当前不一样
                    int cur_temp = nums[left];
                    while(left<right&&nums[left]==cur_temp){left++;}
                    cur_temp = nums[right];
                    while(left<right&&nums[right]==cur_temp){right--;}
                }
                else if(nums[id] + nums[left] + nums[right] > 0){right--;}
                else if(nums[id] + nums[left] + nums[right] < 0){left++;}
            }
        }
        return res;
    }
};