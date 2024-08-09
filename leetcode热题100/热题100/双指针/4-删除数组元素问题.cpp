#include "util.h"

//283. 移动零 

/*
给定一个数组 nums，编写一个函数将所有 0 移动到数组的末尾，同时保持非零元素的相对顺序。

请注意 ，必须在不复制数组的情况下原地对数组进行操作。

 

示例 1:

输入: nums = [0,1,0,3,12]
输出: [1,3,12,0,0]
*/

//注意这种填入数字的双指针 一个用于遍历 一个用于记录新数组内容
//图解网址：
//https://leetcode.cn/problems/move-zeroes/solutions/90229/dong-hua-yan-shi-283yi-dong-ling-by-wang_ni_ma/?envType=study-plan-v2&envId=top-100-liked
class Solution {
public:
    void moveZeroes(vector<int>& nums) {
        int oripos = 0;
        int newpose =0;
        //遍历旧的 将不为0的存储到新起点的数组中去
        for(; oripos <nums.size() ; oripos++){
            if(nums[oripos]!=0){
                nums[newpose] = nums[oripos];
                newpose++;
            }
        }
        //遍历新的 将0赋值给新数组
        for(;newpose<nums.size();newpose++){
            nums[newpose] = 0;
        }
    }
};

// TODO: 同样类型的题目

// 27. 移除元素
/*
给你一个数组 nums 和一个值 val，你需要 原地 移除所有数值等于 val 的元素，并返回移除后数组的新长度。

不要使用额外的数组空间，你必须仅使用 O(1) 额外空间并 原地 修改输入数组。

元素的顺序可以改变。你不需要考虑数组中超出新长度后面的元素。
示例 1：
输入：nums = [3,2,2,3], val = 3
输出：2, nums = [2,2]
解释：函数应该返回新的长度 2, 并且 nums 中的前两个元素均为 2。你不需要考虑数组中超出新长度后面的元素。例如，函数返回的新长度为 2 ，而 nums = [2,2,3,3] 或 nums = [2,2,0,0]，也会被视作正确答案。
*/
class Solution {
public:
    int removeElement(vector<int>& nums, int val) {
        int oripos = 0;
        int newpos = 0;
        for(;oripos<nums.size();oripos++){
            //如果当前元素不等于目标值 则新数组存储目标值
            if(nums[oripos]!=val){
                nums[newpos] = nums[oripos];
                newpos++;
            }
        }
        return newpos;
    }
};

//80 删除有序数组的重复项
/*
给你一个有序数组 nums ，请你 原地 删除重复出现的元素，
使得出现次数超过两次的元素只出现两次 ，返回删除后数组的新长度。

不要使用额外的数组空间，你必须在 原地 修改输入数组 并在使用 O(1) 额外空间的条件下完成。

进阶 如果是超过k次的元素只出现k次 那么改怎么处理


输入：nums = [1,1,1,2,2,3]
输出：5, nums = [1,1,2,2,3]
解释：函数应返回新长度 length = 5, 
并且原数组的前五个元素被修改为 1, 1, 2, 2, 3。 不需要考虑数组中超出新长度后面的元素。

参考：
https://leetcode.cn/problems/remove-duplicates-from-sorted-array-ii/solutions/702970/gong-shui-san-xie-guan-yu-shan-chu-you-x-glnq/?envType=study-plan-v2&envId=top-interview-150
*/

class Solution {
public:
    int removeDuplicates(vector<int>& nums,int k) {
        int len = nums.size();
        if(len == 0){return 0;}

        int newpose = 0;
        for(int c : nums){
            //1 首先保留前k-1位因为前k-1位是一定满足题设的
            //2 当pose>k的时候 判断当前pose往前的k位
            //                如果往前k位和当前pose不相等，说明一定没有达到k位 所以继续记录
            //                如果往前k位和当前pose相等 例如k=2 此时 1 1 1 这里就不回记录
            if(newpose < k || nums[newpose-k] !=c){
                nums[newpose] = c;
                newpose++;
            }
        }

        return newpose;

    }
};

//TODO: 进阶 技巧98  75 颜色分类