#include "util.h"


/*
189. 轮转数组
给定一个整数数组 nums，将数组中的元素向右轮转 k 个位置，其中 k 是非负数。


示例 1:

输入: nums = [1,2,3,4,5,6,7], k = 3
输出: [5,6,7,1,2,3,4]
解释:
向右轮转 1 步: [7,1,2,3,4,5,6]
向右轮转 2 步: [6,7,1,2,3,4,5]
向右轮转 3 步: [5,6,7,1,2,3,4]
*/

/*
解释 当k=n*nums.size()的时候 其实nums没有改变 所以对k可以先取模 降低运算次数
所谓翻转举例
1 2 3 4 5 6 7 --- > 7 6 5 | 4 3 2 1
按k分成左右两个 分别翻转
5 6 7 | 1 2 3 4 即可得到结果
*/

class Solution {
public:
    void m_reverse(vector<int>&nums,int start,int end){
        while(start<end){
            int temp = nums[start];
            nums[start] = nums[end-1];
            nums[end-1] = temp;

            start++;
            end--;
        }
    }
    void rotate(vector<int>& nums, int k) {
        //翻转数组
        k = k%nums.size();
        m_reverse(nums,0,nums.size());
        m_reverse(nums,0,k);
        m_reverse(nums,k,nums.size());
    }
};