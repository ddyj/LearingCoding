//1 两数之和

/*
给定一个整数数组 nums 和一个整数目标值 target，请你在该数组中找出 和为目标值 target  的那 两个 整数，并返回它们的数组下标。

你可以假设每种输入只会对应一个答案。但是，数组中同一个元素在答案里不能重复出现。

你可以按任意顺序返回答案。 

示例 1：

输入：nums = [2,7,11,15], target = 9
输出：[0,1]
解释：因为 nums[0] + nums[1] == 9 ，返回 [0, 1] 。
*/

//定一议一  当遍历为id时 查找vec中大小为 target - num[id] 的值
//哈希 建立哈希表 查找哈希表中大小为 target - num[id] 的值

#include "util.h"

class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        vector<int>res;
        //法1:
        for(int id = 0 ;id < nums.size() ;id++){
            auto it = find(nums.begin()+id+1,nums.end(),target-nums[id]);
            if(it!=nums.end()){
                res.emplace_back(id);
                res.emplace_back(it - nums.begin());
                return res;
            }
        }
        //法2
        unordered_map<int,int>hash_table;
        for(int id = 0;id<nums.size();id++){
            if(!hash_table.empty()&&hash_table.count(target - nums[id])!=0){
                res.emplace_back(hash_table[target -nums[id]]);
                res.emplace_back(id);
                return res;
            }
            else{
                hash_table.insert(make_pair(nums[id],id));
            }
        }
        return res;
    }
};