

/*
46. 全排列
给定一个不含重复数字的数组 nums ，返回其 所有可能的全排列 。
你可以 按任意顺序 返回答案。

示例 1：

输入：nums = [1,2,3]
输出：[[1,2,3],[1,3,2],[2,1,3],[2,3,1],[3,1,2],[3,2,1]]
*/


/*
回溯问题可以转换为树的问题  -- 处理这类问题的时候一定要画树图 从树图分析dfs的回环和舍取细节

如      
                      1  2  3
第一轮dfs          1     2      3
第二轮dfs      12   13  21 23   31 32
第三轮dfs   123    123  213 231 312  321


在for循环中调用dfs 注意存储的path是不带应用的

这里需要考虑 如何让下一轮dfs存储的数组是上一轮选剩下的  -- 我们选择删除 选一个删一个
            在同级的dfs中 需要放置同级间数据污染 对id>0的情况 要把上一轮循环的值从path中取出来
*/


#include "util.h"

class Solution {
public:
    void dfs(vector<int>nums,vector<vector<int>>&res,vector<int>path){
        int len = nums.size();
        for(int id=0;id<len;id++){
            vector<int>temp = nums;
            //vector<int>temp_path = path;
            //如果当前id大于0 要id将上一个循环存入的值取出来 
            if(id>0){
                path.pop_back();
            }
            path.emplace_back(nums[id]);
            temp.erase(std::remove(temp.begin(), temp.end(), temp[id]), temp.end());
            if(temp.size()==0){res.emplace_back(path);return;}
            else{dfs(temp,res,path);}
        }
    }
    vector<vector<int>> permute(vector<int>& nums) {
        vector<vector<int>>res;
        vector<int>path;
        dfs(nums,res,path);
        return res;
    }
};



//进阶
/*
47. 全排列 II
给定一个可包含重复数字的序列 nums ，按任意顺序 返回所有不重复的全排列
输入：nums = [1,1,2]
输出：
[[1,1,2],
 [1,2,1],
 [2,1,1]]
*/
/*
由于 temp.erase(std::remove(temp.begin(), temp.end(), temp[id]), temp.end());是删除所有和temp[id]相等的值
这里有重复元素的话就不能这样做，也就是说每一轮dfs我们传递的都是完整数组nums
我们用于一个vis来记录这个元素在上一轮中有没有使用 1 没用 0 用了
去重逻辑 首先如果当前值vis是0 也就是当前值用过了 就跳过
        同时如果当前id大于0 如果满足nums[id]== nums[id-1] && vis[id-1]==1
        即当前值等于上一个值且上一个值是没有用到当前循环里的
注意 更新了放置同级污染的位置 不必在判断id>0 每轮dfs执行完毕就执行即可
*/


class Solution {
public:
    bool is_allzero(vector<int>res){
        for(int a:res){
            if(a==1){return false;}
        }
        return true;
    }
    void dfs(vector<int>nums,vector<vector<int>>&res,vector<int>path,vector<int>vis){
        if(is_allzero(vis)){res.emplace_back(path);return;}
        for(int id=0;id<nums.size();id++){
            if(vis[id]==0 ||(id>0&&nums[id]==nums[id-1]&&vis[id-1]==1) ){
                continue;
            }
            //if(id>0){path.pop_back();}
            path.emplace_back(nums[id]);
            vis[id] = 0;
            dfs(nums,res,path,vis);
            vis[id] = 1;
            path.pop_back();
            
        }
    }
    vector<vector<int>> permuteUnique(vector<int>& nums) {
        vector<vector<int>>res;
        vector<int>path;
        vector<int>vis(nums.size(),1);
        sort(nums.begin(), nums.end());
        dfs(nums,res,path,vis);
        return res;
    }
};



//T56

/*

78. 子集
给你一个整数数组 nums ，数组中的元素 互不相同 。返回该数组所有可能的子集（幂集）。
解集 不能 包含重复的子集。你可以按 任意顺序 返回解集。

示例 1：

输入：nums = [1,2,3]
输出：[[],[1],[2],[1,2],[3],[1,3],[2,3],[1,2,3]]
*/

class Solution {
public:
    void dfs(vector<int>nums,vector<vector<int>>&res,vector<int>path,int cur_len){
        for(int id=cur_len;id<nums.size();id++){
            path.emplace_back(nums[id]);
            res.emplace_back(path);
            dfs(nums,res,path,id+1);
            path.pop_back();
        }
    }
    vector<vector<int>> subsets(vector<int>& nums) {
        vector<vector<int>>res;
        vector<int>path;
        res.emplace_back(path);
        dfs(nums,res,path,0);
        return res;
    }
};