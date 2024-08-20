
#include "util.h"

/*
给你一个整数数组 nums 和一个整数 k ，请你统计并返回 该数组中和为 k 的连续子数组的个数 。
子数组是数组中元素的连续非空序列。


示例 1：
输入：nums = [1,1,1], k = 2
输出：2

*/

// ** trick 前缀和
//如果我们用p[i]表示(0,i)的所有元素的和
//那么实际上我们要求的字串(i,j)是满足p[j] - p[i] == k的

//转化为求解两数之和的问题的变式 我们遍历数组 获取每个位置的p[i]在哈希表中存储p[i]的值 
//等待后续值在哈希表中找到满足p[j] - k 的情况
//而p[j] - k的出现的次数 就是对应的字串数

class Solution {
public:
    int subarraySum(vector<int>& nums, int k) {
        int res = 0;
        int pre = 0;
        unordered_map<int,int>presum;
        //这里presum[0] = 1 
        //因为如果初始哈希表为空 当出现pre = k的结果 即 pre-k = 0 的时候满足题设但是没有执行进res处理的if
        //所以在初始化哈希表的时候就需要对0的位置置0
        presum[0] = 1;
        for(int id=0;id<nums.size();id++){
            pre += nums[id];
            if(presum.count(pre-k)!=0){
                res+=presum[pre - k];
            }
            presum[pre]++;
        }
        return res;
    }
};



//TODO：
/*
给你一个整数数组 nums 和一个整数 k ，编写一个函数来判断该数组是否含有同时满足下述条件的连续子数组：

子数组大小 至少为 2 ，且
子数组元素总和为 k 的倍数。
如果存在，返回 true ；否则，返回 false 。

如果存在一个整数 n ，令整数 x 符合 x = n * k ，则称 x 是 k 的一个倍数。0 始终视为 k 的一个倍数。


示例 1：

输入：nums = [23,2,4,6,7], k = 6
输出：true
解释：[2,4] 是一个大小为 2 的子数组，并且和为 6 。
*/

// pre[j] - pre[i] = n*k
// pre[j] = n*k + res1 pre[i] = n2*k+res2
// sum(i,j) = nk <=> pre[j]%k == pre[i]%k 

class Solution {
public:
    bool checkSubarraySum(vector<int>& nums, int k) {
      unordered_map<int,int>pre_sumHash;
      //考虑到取0-n的情况  让哈希表中有这个值就行 id赋多少无所谓
      pre_sumHash[0] = -1;
      int pre=0;
      for(int id=0;id<nums.size();id++){
        pre = (pre + nums[id])%k;
        if(pre_sumHash.count(pre)!=0){
            if(id - pre_sumHash[pre] >= 2){
               return true;
            }
        }
        //不更新id 只插入id 哈希表中存储的余数的可能 如果一直更新 就是p[j]-p[j-1]
        else{
          pre_sumHash[pre] = id;
        }
      }
      return false;
    }
};