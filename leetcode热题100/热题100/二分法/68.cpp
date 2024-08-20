#include "util.h"

/*
4.寻找两个正序数组的中位数
给定两个大小分别为 m 和 n 的正序（从小到大）数组 nums1 和 nums2。请你找出并返回这两个正序数组的 中位数 。
算法的时间复杂度应该为 O(log (m+n)) 。

输入：nums1 = [1,3], nums2 = [2]
输出：2.00000
解释：合并数组 = [1,2,3] ，中位数 2
*/

//借鉴合并有序链表的穿针引线法 我们可以合并两个数组到中位数出现的位置


class Solution {
public:
    double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
        int final_len = nums1.size() + nums2.size();
        int mid_pos = final_len/2;

        int count = 0;
        double cur_num = 0;
        int nums1_id = 0;
        int nums2_id = 0;


        //防止报数组为空的错误
        nums1.emplace_back(INT_MAX);
        nums2.emplace_back(INT_MAX);

        while(nums1_id<nums1.size() || nums2_id<nums2.size()){
            int num1 = nums1_id > nums1.size()-1 ? INT_MAX : nums1[nums1_id];
            int num2 = nums2_id > nums2.size()-1 ? INT_MAX : nums2[nums2_id];
            double pre = cur_num;
            cur_num = min(num1,num2);
            if(cur_num == num1){nums1_id++;}
            else{nums2_id++;}
            
            if(count == mid_pos){
                if(final_len%2!=0){return cur_num;}
                return (cur_num+pre)/2.0;
            }

            count++;
        }
        return 1;
    }
};