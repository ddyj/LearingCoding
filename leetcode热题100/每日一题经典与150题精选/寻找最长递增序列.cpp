#include "util.h"


/*
给你一个整数数组 nums ，找到其中最长严格递增子序列的长度。

子序列 是由数组派生而来的序列，删除（或不删除）数组中的元素而不改变其余元素的顺序。

例如，[3,6,2,7] 是数组 [0,3,1,6,2,2,7] 的子序列。

 

输入：nums = [10,9,2,5,3,7,101,18]
输出：4
解释：最长递增子序列是 [2,3,7,101]，因此长度为 4 。
*/



class Solution {
public:
    //往回dp dp[i]以第i个数字结尾的的序列的 最长递增长度为i
    //dp[i] 等于 max(dp[j]) + 1   0 <= j < i
    int lengthOfLIS01(vector<int>& nums) {
        int len = nums.size();
        vector<int>dp(len,0);  //dp[i]以第i个数字结尾的的序列的 最长递增长度为i
        //dp[0] = 1; 
        int res = dp[0];
        for(int i=0;i<len;i++){
            int tmp = INT_MIN;
            for(int j=0;j<i;j++){
                if(nums[j] < nums[i]){tmp = max(dp[j],tmp);}
            }
            dp[i] =tmp==INT_MIN ? 1 : tmp + 1;
            res = max(res,dp[i]);
        }
        return res;
    }
    //二分法
    /*
    已经得到的上升子序列的结尾的数越小，
    那么遍历的时候后面接上一个数，会有更大的可能构成一个长度更长的上升子序列。
    既然结尾越小越好，我们可以记录 在长度固定的情况下，结尾最小的那个元素的数值.



    维护一个tails 数组 tails[i]表示以长度为i+1的所有上升子序列的最下的结尾值
    最终tails的最大下标+1 就是数组的最大长度

    维护方式 
        对于新遍历元素nums[i]  如果nums[i] > tails[end] 将nums[i]直接放入
        如果nums[i] < tails[end]  就用二分 将 第一个大于当前元素的值替换为nums[i]
    */
    int lengthOfLIS(vector<int>&nums){
        vector<int>tails;
        int len = nums.size();
        tails.emplace_back(nums[0]);
        int end = 0; //tails 的末尾下标 用于二分
        for(int i=1;i<len;i++){
            if(nums[i]>tails[end]){
                tails.emplace_back(nums[i]);
                end++;
            }
            else{
                int left = 0;
                int right = end;
                while(left <= right){
                    int mid = left + (right-left)/2 ;
                    if(tails[mid] < nums[i]){
                        left = mid+1;
                    }
                    else{
                        right = mid-1;
                    }
                }
                tails[left] = nums[i];
            }
        }
        return end+1;
    }
};





/*
我们定义 arr 是 山形数组 当且仅当它满足：

arr.length >= 3
存在某个下标 i （从 0 开始） 满足 0 < i < arr.length - 1 且：
arr[0] < arr[1] < ... < arr[i - 1] < arr[i]
arr[i] > arr[i + 1] > ... > arr[arr.length - 1] 
给你整数数组 nums​ ，请你返回将 nums 变成 山形状数组 的​ 最少 删除次数。


输入：nums = [1,3,1]
输出：0
解释：数组本身就是山形数组，所以我们不需要删除任何元素。
*/

/*
思路 我们遍历每一个位置 向左向右寻找最长递增子序列 
reslen = max(leftlen + rightlen -1 ,reslen); 
*/




class Solution {
public:
    void my_reverse(vector<int>&nums,int start,int end){
        while(start < end){
            int tmp = nums[start];
            nums[start] = nums[end];
            nums[end] = tmp;
            start++;
            end--;
        }
    }


    int callengthOfLIS(vector<int>nums,int start,int end,int type){
        if(start == end){return 0;}
        if(type){
            my_reverse(nums,start,end-1);
        }
        vector<int>tails;
        tails.emplace_back(nums[start]);
        
        int t_end = 0;

        for(int i=start+1;i<end;i++){
            if(nums[i] > tails[t_end]){
                tails.emplace_back(nums[i]);
                t_end ++;
            }
            else{
                int left = 0;
                int right = t_end-1;
    
                while(left <= right){
                    int mid = left + (right-left)/2;
                    if(tails[mid] < nums[i]){
                        left = mid+1;
                    }
                    else{
                        right = mid-1;
                    }
                }
                tails[left] = nums[i];
            }
        }

        return t_end > 0 ? t_end + 1 : 0;
    }



    //对每一个元素遍历 求0-i的最长递增子序列长度 + i-end的最长递减自序列长度 的最大值
    int minimumMountainRemovals(vector<int>& nums) {
        int len = nums.size();
        int reslen = 0;
        for(int i=1;i<len-1;i++){
            int leftlen = callengthOfLIS(nums,0,i+1,0);
            int rightlen = callengthOfLIS(nums,i,len,1);
            if(leftlen!=0 && rightlen!=0){
                reslen = max(leftlen + rightlen -1 ,reslen); 
            }
        }
        return len - reslen;
    }
};