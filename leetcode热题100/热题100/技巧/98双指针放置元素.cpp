#include "util.h"



/*
75 颜色分类
给定一个包含红色、白色和蓝色、共 n 个元素的数组 nums ，
原地对它们进行排序，使得相同颜色的元素相邻，并按照红色、白色、蓝色顺序排列。
我们使用整数 0、 1 和 2 分别表示红色、白色和蓝色。
必须在不使用库内置的 sort 函数的情况下解决这个问题

输入：nums = [2,0,2,1,1,0]
输出：[0,0,1,1,2,2]
*/


//一个遍历id 一个左指针left 一个右指针0
//如果nums[id] == 0  交换 left++;
//如果nums[id] == 2 nums[right] 和当前nums[id]交换 right--
//如果是1 id++

class Solution {
public:
    void sortColors(vector<int>& nums) {
        int left = 0;
        int cur_id = 0;
        int right = nums.size()-1;
        if(nums.size() == 1){return;}
        while(cur_id <= right){
            if(nums[cur_id] == 1 ){cur_id++;}
            //从左侧换过来的 只能是1
            else if(nums[cur_id] == 0){
                nums[cur_id] = nums[left];
                nums[left] = 0;
                left++;
                cur_id++;
            }
            //可能会把后头的1、0换过来 所以不对cur_id++,下一轮重新判断
            else if(nums[cur_id] == 2){
                nums[cur_id] = nums[right];
                nums[right] = 2;
                right--;
            }
        }
    }
};