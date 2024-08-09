
#include "util.h"
//128. 最长连续序列

/**
 给定一个未排序的整数数组 nums ，找出数字连续的最长序列（不要求序列元素在原数组中连续）的长度。

请你设计并实现时间复杂度为 O(n) 的算法解决此问题。

示例 1：

输入：nums = [100,4,200,1,3,2]
输出：4
解释：最长数字连续序列是 [1, 2, 3, 4]。它的长度为 4。
 * 
 */
//建立一张哈希表 对nums元素去重
//再对哈希表遍历 由于我们是要寻找最长的连续数字 我们需要从没有前继的内容中（没有cur_num -1 ）的值开始
//所以先判断是否没有前继 然后再去查找表中有几个后续 并维护最大值
class Solution {
public:
    int longestConsecutive(vector<int>& nums) {
        if(nums.size() == 0){return 0;}
        int res = 0;
        unordered_set<int>hash_table;
        //建表 + 去重复
        for(int id=0;id<nums.size();id++){
            hash_table.insert(nums[id]);
        }
        
        for(auto &p : hash_table){
            //查找顺序序列的起点 即没有x-1的数
            if(hash_table.count(p-1)==0){
                int cur_num = p+1;
                while(hash_table.count(cur_num)>0){
                    cur_num++;
                }
                res = max(res,cur_num - p);
            }
        }
        return res;
    }
};