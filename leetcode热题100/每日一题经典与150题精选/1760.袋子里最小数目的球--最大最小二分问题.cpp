/*


给你一个整数数组 nums ，其中 nums[i] 表示第 i 个袋子里球的数目。
同时给你一个整数 maxOperations 。
你可以进行如下操作至多 maxOperations 次：

选择任意一个袋子，并将袋子里的球分到 2 个新的袋子中，每个袋子里都有 正整数 个球。
比方说，一个袋子里有 5 个球，你可以把它们分到两个新袋子里，分别有 1 个和 4 个球，
或者分别有 2 个和 3 个球。
你的开销是单个袋子里球数目的 最大值 ，你想要 最小化 开销。

请你返回进行上述操作后的最小开销。

输入：nums = [9], maxOperations = 2
输出：3
解释：
- 将装有 9 个球的袋子分成装有 6 个和 3 个球的袋子。[9] -> [6,3] 。
- 将装有 6 个球的袋子分成装有 3 个和 3 个球的袋子。[6,3] -> [3,3,3] 。
装有最多球的袋子里装有 3 个球，所以开销为 3 并返回 3 。

*/

#include "util.h"

/*
最大值最小化，那二分答案有很大概率是可以做的。
答案越小，所需要的操作次数肯定是越多的，因此就有了单调性，可以用二分答案来做。
如果我们要将最大值降低至target，可以计算一下需要多少操作数（每个数大于target的数都要尽可能均分），分为以下两种情况：

** 重点 如何建立操作次数和最大最小值之间的关系？
   对于一个大于当前目标值的数nums[i] 我们对齐进行拆分后最大值小于等于mid值

    如果   mid < num[i] <= 2*mid  需要拆分一次
           2*mid < num[i] <= 3*mid  拆分两次

            (num-1) / mid
            -- 减一 将右侧括号去掉

若操作数大于maxOperations，说明target太小了，需要往右搜索；
否则需要往左搜索。
*/

class Solution {
public:
    int cal_Operations(vector<int>&nums,int mid){
        int res = 0;
        for(int num : nums){
            if(num > mid){
                res += ((num-1)/mid);
            }
        }
        return res;
    }

    int minimumSize(vector<int>& nums, int maxOperations) {
        int len = nums.size();
        int left = 1;
        int right = *max_element(nums.begin(),nums.end());
        int res = INT_MAX;
        while(left<=right){
            //std::cout<<left<<" "<<right<<std::endl;
            int mid = left + (right-left)/2;
            //std::cout<<cal_Operations(nums,mid)<<std::endl;
            if(cal_Operations(nums,mid) > maxOperations){
                left = mid+1;
            }
            else if(cal_Operations(nums,mid) <= maxOperations){
                right = mid-1;
                res = min(res,mid);
            }
        }
        return res;
    }
};


//TODO: 同类型题
//1552. 两球之间的磁力

/*
    若设此时最大化的最小磁力为mid  在position中查找能否有满足题设的位置数量
    如果数量大于m 说明mid偏小 放大  反之缩小
    
    让 相邻两个球之间距离的最小值 最大
    也就是说 即使 间距为 33 4 其结果为3 不是4
    ** 如何计算mid下的position中的数量
        当确定最大化最小磁力为mid 也就说明在position中相邻的距离间隔最大是mid

*/


class Solution {
public:

    int cal_maxpose(vector<int>&position,int target){
        int count = 1;
        int idx = 0;
        for(int i=1;i<position.size();i++){
            if(position[i] - position[idx] >= target){
                count++;
                idx = i;
            }
        }
        return count;
    }

    int maxDistance(vector<int>& position, int m) {
        sort(position.begin(),position.end());
        int left = 0;
        int right = position[position.size()-1] - position[0];
        int res = INT_MIN;
        while(left<=right){
            int mid = left + (right-left)/2;
            if(cal_maxpose(position,mid) >= m){
                left = mid+1;
                res = max(res,mid);
            }
            else{
                right = mid-1;
            }
        }
        return res;
    }
};



/*
    这两个题目中 res的位置是有讲究的
    1760中，目的是输出最大值的最小值 (最小化开销) mid表示的是最大值 所以用min
            在二分法中 右侧区间缩小会出现减小mid的情况 直到循环结束条件满足 所以把等于号放到right一侧 
    1552中，目的是输出最小值的最大值(最大最小磁力) mid表示最小磁力 所以用max
            在二分法中  左侧区间改变会放大mid 直到循环结束条件满足  所以把等号放到left一侧


*/