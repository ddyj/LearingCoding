#include "util.h"



/*
    跳跃游戏1
    给你一个非负整数数组 nums ，
    你最初位于数组的 第一个下标 。数组中的每个元素代表你在该位置可以跳跃的最大长度。
    判断你是否能够到达最后一个下标，如果可以，返回 true ；否则，返回 false 。
    输入：nums = [2,3,1,1,4]
    输出：true
    解释：可以先跳 1 步，从下标 0 到达下标 1, 然后再从下标 1 跳 3 步到达最后一个下标。
*/
class Solution {
public:
    bool canJump(vector<int>& nums) {
        int cur_max = 0;
        int len = nums.size();
        for(int i=0;i<len;i++){
            cur_max = max(cur_max,nums[i]+i);
            if(i==cur_max && i!=len-1){return false;}
        }
        return true;
    }
};

/*
    跳跃游戏2
    给定一个长度为 n 的 0 索引整数数组 nums。初始位置为 nums[0]。
    每个元素 nums[i] 表示从索引 i 向前跳转的最大长度。
    换句话说，如果你在 nums[i] 处，你可以跳转到任意 nums[i + j] 处:

    0 <= j <= nums[i] 
    i + j < n
    返回到达 nums[n - 1] 的最小跳跃次数。生成的测试用例可以到达 nums[n - 1]。
*/
class Solution {
public:
    //为了求最小步数，我们只有当不得不跳时，才跳
    //计算每一次的最大跳跃位置 只有当i移动到边界了才进行一次跳跃
    int jump(vector<int>& nums) {
        int count = 0;
        int len = nums.size();
        int cur_max = 0;
        int jump_end = 0;
        for(int i=0;i<len;i++){
            cur_max = max(cur_max,nums[i]+i);
            //注意 当i到达最后边界的时候 就不需要再跳了
            if(i == jump_end && i!=len-1){
                jump_end = cur_max;
                count++;
            }
        }
        return count;
    }
};