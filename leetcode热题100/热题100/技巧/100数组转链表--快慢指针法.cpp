

#include "util.h"

/*
287 寻找重复数
给定一个包含 n + 1 个整数的数组 nums ，其数字都在 [1, n] 范围内（包括 1 和 n），可知至少存在一个重复的整数。

假设 nums 只有 一个重复的整数 ，返回 这个重复的数 。

你设计的解决方案必须 不修改 数组 nums 且只用常量级 O(1) 的额外空间。

*/

class Solution {
private:
    unordered_map<int,int>hash_table;
public:
    //法一 建立哈希表
    int findDuplicate_hash(vector<int>& nums) {
        for(int c:nums){
            if(hash_table[c]!=0){return c;}
            hash_table[c]++;
        }
        return 1;
    }
    //法二 快慢指针法 按照nums[id]的方式 那么一定会形成一个链表 重复元素就是入换结点的位置
    int findDuplicate(vector<int>& nums){
        int slow = nums[0];
        int fast = nums[nums[0]];
        
        while(fast!=slow && fast < nums.size()){
            slow = nums[slow];
            fast = nums[nums[fast]];
        }
      
        fast = 0;
        while(fast!=slow){
            fast = nums[fast];
            slow = nums[slow];
        }
        return slow;
    }
};