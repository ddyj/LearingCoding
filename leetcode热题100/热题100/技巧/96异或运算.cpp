#include "util.h"


/*
136 只出现一次的数字
给你一个 非空 整数数组 nums ，除了某个元素只出现一次以外，其余每个元素均出现两次。找出那个只出现了一次的元素。

你必须设计并实现线性时间复杂度的算法来解决此问题，且该算法只使用常量额外空间。
*/

class Solution {
private:
    unordered_set<int> hash_table;
public:
    //法一 哈希
    int singleNumber_hash(vector<int>& nums) {
        for(int id=0;id<nums.size();id++){
            if(hash_table.count(nums[id]) == 1 ){
                hash_table.erase(nums[id]);
            }
            else{
                hash_table.insert(nums[id]);
            }
        }
        return *hash_table.begin();
    }
    //从算理的角度入手
    /*
    知识点：正常我们交换两个元素 是借助一个temp作为辅助元素交换
           我们也可以不借助temp交换 是通过异或实现的
           异或 两个相同的数异或结果是0 相关不同的数异或结果是1 切异或满足结合律
           对于a、b 
           a = a^b;
           b = a^b;  // 此时 b = a^b^b = a; 
           a = b^a;  // 此时 a = a^b^b^a^b = b;
    */
    int singleNumber(vector<int>& nums){
        int res = nums[0];
        for(int id=1;id<nums.size();id++){
            res ^=nums[id];
        }
        return res;
    }
};