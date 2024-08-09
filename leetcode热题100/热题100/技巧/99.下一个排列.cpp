#include "util.h"

class Solution {
public:
    /*
        1.从后向前找第一个递增序列AB
        2.从末尾到B找出第一个大于A的值 C
        3.交换AC 交换之后让A位置之后的元素变为升序(可以证明A位置到结尾时升序的)
    */
    void nextPermutation(vector<int>& nums) {
        int i = nums.size()-2;
        while(i>=0 && nums[i] >= nums[i+1]){
            i--;
        }
        if(i >= 0){
            int j = nums.size() -1 ;
            while(j>=0 && nums[i] >= nums[j]){
                j--;
            }
            swap(nums[i],nums[j]);
        }
        reverse(nums.begin() + i + 1, nums.end());
    }
};