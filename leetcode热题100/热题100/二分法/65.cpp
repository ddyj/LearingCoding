#include "util.h"

/*

34. 在排序数组中查找元素的第一个和最后一个位置
给你一个按照非递减顺序排列的整数数组 nums，和一个目标值 target。
请你找出给定目标值在数组中的开始位置和结束位置。

如果数组中不存在目标值 target，返回 [-1, -1]。

你必须设计并实现时间复杂度为 O(log n) 的算法解决此问题。
*/


class Solution {
public:
    int my_lower_bound(vector<int>&nums,int target){
        int left = 0;
        int right = nums.size()-1;
        while(left<=right){
            int mid = left + (right-left)/2;
            if(nums[mid] < target ){left = mid+1;}
            else{right = mid-1;}
        }
        return left;
    }
    int find_upper(vector<int>&nums,int left_pose){
        int temp = nums[left_pose];
        while(left_pose < nums.size()){
            if(nums[left_pose] > temp){return left_pose-1;}
            left_pose++;
        }
        return nums.size()-1;
    }

    vector<int> searchRange(vector<int>& nums, int target) {
        vector<int>res(2,-1);

        int left_pose = my_lower_bound(nums,target);
        
        if(nums.size()==0 || left_pose==nums.size() || nums[left_pose]!=target){
            return res;
        }
        //找到大于当前元素的值
        int right_pose = find_upper(nums,left_pose);
        res[0] = left_pose;
        res[1] = right_pose;
        return res;
    }
};