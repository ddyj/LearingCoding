#include"util.h"


/*
169 多数元素
给定一个大小为 n 的数组 nums ，返回其中的多数元素。
多数元素是指在数组中出现次数 大于 ⌊ n/2 ⌋ 的元素。

你可以假设数组是非空的，并且给定的数组总是存在多数元素。

*/

class Solution {
private:
    unordered_map<int,int>hash_table;
public:
    //法一 哈希
    int majorityElement_hash(vector<int>& nums) {
        int len = nums.size();
        for(int id=0;id<len;id++){
            hash_table[nums[id]]++;
            if(hash_table[nums[id]] > len/2){return nums[id];}
        }
        return 0;
    }
    //法二 投票法
    //从第一个数开始 如果遇到和这个数相同的数 就加一 不相同就减一 减到0换一个数
    //最后票数最多的数 一定会存储下来
    int majorityElement(vector<int>& nums) {
        int len = nums.size();
        int count = 1;
        int number = nums[0];
        for(int id=1;id<len;id++){
            if(count==0){
                number = nums[id];
            }
            count = number == nums[id] ? count+1 : count-1;
        }
        return number;
    }
};