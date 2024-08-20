#include "util.h"


/*
给你一个 无重复元素 的整数数组 candidates 和一个目标整数 target ，
找出 candidates 中可以使数字和为目标数 target 的 所有 不同组合 ，
并以列表形式返回。你可以按 任意顺序 返回这些组合。

candidates 中的 同一个 数字可以 无限制重复被选取 。
如果至少一个数字的被选数量不同，则两种组合是不同的。 

对于给定的输入，保证和为 target 的不同组合数少于 150 个。

输入：candidates = [2,3,6,7], target = 7
输出：[[2,2,3],[7]]
解释：
2 和 3 可以形成一组候选，2 + 2 + 3 = 7 。注意 2 可以使用多次。
7 也是一个候选， 7 = 7 。
仅有这两种组合。
*/


//同一个数字无限制选取的回溯 当结果是taget
//难点： 怎么去重？ 我们对数组排序，按照顺序执行dfs path中的结果也是顺序的
//这样就避免了223 232 322 这种不是顺序的组合 
//设定dfs的顺序这也是一种经典的剪枝手段

class Solution {
public:
    int getSum(vector<int>path){
        int sum = 0;
        for(int a:path){
            sum+=a;
        }
        return sum;
    }
    void dfs(vector<int>&candidates,vector<vector<int>>&res,vector<int>path,int target,int start_id){
        for(int id=start_id;id<candidates.size();id++){
            path.emplace_back(candidates[id]);
            int sum = getSum(path);
            if(id>0 && candidates[id] == candidates[id-1]){continue;}
            if(candidates[id]>target || sum>target){return;}
            if(sum==target){res.emplace_back(path);return;}
            //sum < target的时候才会执行下一轮dfs
            else{
                dfs(candidates,res,path,target,id);
            }
            path.pop_back();
        }
    }

    vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
        sort(candidates.begin(),candidates.end());
        vector<vector<int>>res;
        vector<int>path;
        dfs(candidates,res,path,target,0);
        return res;
    }
};


//正难则反
/*
除了计算加法 我们还可以计算减法 维护target - candidates[id]的值
*/
class Solution {
public:
    void dfs(vector<int>&candidates,vector<vector<int>>&res,vector<int>path,int target,int start_id){
        for(int id=start_id;id<candidates.size();id++){

            //int sum = getSum(path);
            if(target<0){return;}
            if(target==0){res.emplace_back(path);return;}
            //sum < target的时候才会执行下一轮dfs
            else{
                path.emplace_back(candidates[id]);
                dfs(candidates,res,path,target-candidates[id],id);
                path.pop_back();
            }
           
        }
    }

    vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
        sort(candidates.begin(),candidates.end());
        vector<vector<int>>res;
        vector<int>path;
        dfs(candidates,res,path,target,0);
        return res;
    }
};