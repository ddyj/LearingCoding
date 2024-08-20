

#include "util.h"

/*
给你一个整数数组 nums，返回 数组 answer ，其中 answer[i] 等于 nums 中除 nums[i] 之外其余各元素的乘积 。
题目数据 保证 数组 nums之中任意元素的全部前缀元素和后缀的乘积都在  32 位 整数范围内。
请 不要使用除法，且在 O(n) 时间复杂度内完成此题。


示例 1:

输入: nums = [1,2,3,4]
输出: [24,12,8,6]
*/


//*!!!* 总结 这类题想需要我们手动写出表达式 当无法顺序表达的时候 可以考虑逆序排布、拆分两侧排布的方式解决
//      本体中就是分成两侧排布
//n=nums.size()-1
//ans[i] = a[0] * a[1]*…… *a[i-1] * a[i+1] *……a[n]
//由于明确不能使用除法 可以用两个数组分别记录a[0] * a[1]*…… *a[i-1]和a[i+1] *……a[n]的值 最后ans的值是数组对应位置的乘积
//left[i] = a[0] * a[1]*…… *a[i-1] right[i] = a[i+1] *……a[n]
//left[0] = 1 right[n] = 1
class Solution {
public:

    //法一 分别拆分左右
    
    vector<int> productExceptSelf(vector<int>& nums) {
        vector<int>left_num(nums.size());
        vector<int>right_num(nums.size());
        vector<int>res(nums.size());
        left_num[0] = 1;
        right_num[nums.size()-1] = 1;
        for(int id = 0;id <nums.size()-1 ;id++){
            left_num[id+1] = left_num[id] * nums[id];
        }
        for(int id = nums.size()-1;id >0 ;id--){
            right_num[id-1] =right_num[id] * nums[id];
        }
        for(int id=0;id<nums.size();id++){
            res[id] = left_num[id] * right_num[id];
        }
        return res;
    }
    
    //法二 进阶 相比于法一 我们这里可以不维护right_num 在记录完left_num后 考虑递推公式：
    /*  
    n=nums.size()-1
    ans[n] = left_num[n] * 1
    ans[n-1] = left_num[n-1] * a[n]
    ans[n-2] = left_num[n-2] * a[n] * a[n-1]

    我们只需要用一个累乘的变量代替right_num即可
    
    */
    vector<int> productExceptSelf_Greater(vector<int>& nums) {
        vector<int>left_num(nums.size());
        vector<int>res(nums.size());
        left_num[0] = 1;
        int pre_muti = 1;
        for(int id = 0;id <nums.size()-1 ;id++){
            left_num[id+1] = left_num[id] * nums[id];
        }
        for(int id = nums.size()-1;id >= 0 ;id--){
            res[id] = left_num[id] * pre_muti;
            pre_muti *= nums[id]; 
        }
        return res;
    }
};