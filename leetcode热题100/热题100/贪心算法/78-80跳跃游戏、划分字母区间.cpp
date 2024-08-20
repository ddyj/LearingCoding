#include "util.h"


/*
55 跳跃游戏
给你一个非负整数数组 nums ，
你最初位于数组的 第一个下标 。
数组中的每个元素代表你在该位置可以跳跃的最大长度。

判断你是否能够到达最后一个下标，
如果可以，返回 true ；否则，返回 false 。
*/
class Solution {
public:
    bool canJump(vector<int>& nums) {
        int len = nums.size();
        int maxPose = 0;
        for(int i =0;i<len ;i++){
            maxPose = max(maxPose,nums[i]+i);
            if(i >= maxPose && i!=len-1){
                return false;
            }
        }
        return true;
    }
};


/*
45 跳跃游戏2
给定一个长度为 n 的 0 索引整数数组 nums。初始位置为 nums[0]。

每个元素 nums[i] 表示从索引 i 向前跳转的最大长度。
换句话说，如果你在 nums[i] 处，你可以跳转到任意 nums[i + j] 处:

0 <= j <= nums[i] 
i + j < n
返回到达 nums[n - 1] 的最小跳跃次数。
生成的测试用例可以到达 nums[n - 1]。
*/
class Solution {
public:
    int jump(vector<int>& nums) {
        int len = nums.size();
        int cur_pose = 0;  //当前位置
        int max_pose = 0;  //从i-cur_pose的最远距离
        int cnts = 0;
        for(int i=0;i<len;i++){
            max_pose = max(max_pose,i+nums[i]);
            //当i访问到当前位置的时候 说明需要跳动一下 每次跳到最远距离
            if(i==cur_pose && i!=len-1){
                cnts++;
                cur_pose = max_pose;
            }
        }
        return cnts;
    }
};




/*
763 划分字母区间

给你一个字符串 s 。
我们要把这个字符串划分为尽可能多的片段，
同一字母最多出现在一个片段中。

注意，划分结果需要满足：
将所有划分结果按顺序连接，得到的字符串仍然是 s 。

返回一个表示每个字符串片段的长度的列表。

 

*/
class Solution {
public:
    vector<int> partitionLabels(string s) {
        vector<int>res;
        int len = s.length();
        unordered_map<char,int>far_pose;
        //记录每一个字符的最远位置
        for(int i=0;i<len;i++){
            far_pose[s[i]] = i;  
        }
        int cur_start = 0; //记录起点
        int str_maxPose = 0; //记录当前起点到i区域的字符串中 每一个字母 最远的位置 的最大值
        //按照最远距离切割 本身就会切出最小值
        for(int i=0;i<len;i++){
            str_maxPose = max(str_maxPose,far_pose[s[i]]);
            //当i的位置和最大位置一致时（再往下就是新字母） 
            //说明前面需要断开了
            if(i == str_maxPose){
                res.emplace_back(str_maxPose - cur_start + 1);
                cur_start = i + 1;
                str_maxPose = far_pose[s[cur_start]];
            }
        }
        return res;
    }
};