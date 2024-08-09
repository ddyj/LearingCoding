
#include "util.h"
/*
T66 153. 寻找旋转排序数组中的最小值
已知一个长度为 n 的数组，预先按照升序排列，经由 1 到 n 次 旋转 后，得到输入数组。例如，原数组 nums = [0,1,2,4,5,6,7] 在变化后可能得到：
若旋转 4 次，则可以得到 [4,5,6,7,0,1,2]
若旋转 7 次，则可以得到 [0,1,2,4,5,6,7]
注意，数组 [a[0], a[1], a[2], ..., a[n-1]] 旋转一次 的结果为数组 [a[n-1], a[0], a[1], a[2], ..., a[n-2]] 。

给你一个元素值 互不相同 的数组 nums ，它原来是一个升序排列的数组，并按上述情形进行了多次旋转。请你找出并返回数组中的 最小元素 。

你必须设计一个时间复杂度为 O(log n) 的算法解决此问题。
示例 1：

输入：nums = [3,4,5,1,2]
输出：1
解释：原数组为 [1,2,3,4,5] ，旋转 3 次得到输入数组。
*/

/*
图解 
https://leetcode.cn/problems/find-minimum-in-rotated-sorted-array/solutions/698479/xun-zhao-xuan-zhuan-pai-xu-shu-zu-zhong-5irwp/?envType=study-plan-v2&envId=top-100-liked
*/

//!查找旋转数组的最小值 找右侧不找左侧
class Solution {
public:
    int findMin(vector<int>& nums) {
        //二分法 找到间隔为1的左侧L和右侧R 使得nums[R] < nums[L]
        int left = 0;
        int right = nums.size()-1;
        while(left<right){
            int mid = left + (right-left)/2;
            if(nums[mid] < nums[right]){
                right = mid;
            }
            else{
                left = mid+1;
            }
        }
        return nums[left];
    }
};


/*
T67 33. 搜索旋转排序数组
整数数组 nums 按升序排列，数组中的值 互不相同 。

在传递给函数之前，nums 在预先未知的某个下标 k（0 <= k < nums.length）上进行了 旋转，使数组变为 [nums[k], nums[k+1], ..., nums[n-1], nums[0], nums[1], ..., nums[k-1]]（下标 从 0 开始 计数）。例如， [0,1,2,4,5,6,7] 在下标 3 处经旋转后可能变为 [4,5,6,7,0,1,2] 。

给你 旋转后 的数组 nums 和一个整数 target ，
如果 nums 中存在这个目标值 target ，则返回它的下标，否则返回 -1 。

你必须设计一个时间复杂度为 O(log n) 的算法解决此问题。

示例 1：

输入：nums = [4,5,6,7,0,1,2], target = 0
输出：4
*/

/*
    先确定数组中的最小值 将数组分为左右两个升序数组
    确定target所属的数组范围 对这个数组做二分处理
*/

class Solution {
public:
    int findMin(vector<int>& nums){
        int left = 0;
        int right = nums.size()-1;
        while(left<right){
            int mid = left + (right-left)/2;
            if(nums[mid] < nums[right]){
                right = mid;
            }
            else{
                left = mid+1;
            }
        }
        return left;
    }
    int searchsub(const vector<int>nums,int left,int right,int target){
        while(left<=right){
            int mid = left + (right-left)/2;
            if(nums[mid] > target){right = mid-1;}
            else if(nums[mid] == target){return mid;}
            else{left = mid+1;}
        }
        return -1;
    }
    int search(vector<int>& nums, int target) {
        int pos = findMin(nums);
        if(pos == 0){return searchsub(nums,0,nums.size()-1,target);}
        else if(nums[pos-1] >= target && nums[0] <= target){
            return searchsub(nums,0,pos-1,target);
        }
        else if(nums[pos] <= target && nums[nums.size()-1]>=target){
            return searchsub(nums,pos,nums.size()-1,target);
        }
        else if(target < nums[pos]){return -1;}
                   
        return -1;
    }
};