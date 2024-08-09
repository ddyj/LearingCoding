


#include "util.h"

/*
198 打家劫舍
你是一个专业的小偷，计划偷窃沿街的房屋。每间房内都藏有一定的现金，
影响你偷窃的唯一制约因素就是相邻的房屋装有相互连通的防盗系统，如果两间相邻的房屋在同一晚上被小偷闯入，系统会自动报警。

给定一个代表每个房屋存放金额的非负整数数组，计算你 不触动警报装置的情况下 ，一夜之内能够偷窃到的最高金额。
输入：[1,2,3,1]
输出：4
解释：偷窃 1 号房屋 (金额 = 1) ，然后偷窃 3 号房屋 (金额 = 3)。
     偷窃到的最高金额 = 1 + 3 = 4 
*/

class Solution {
public:
    int rob(vector<int>& nums) {
        vector<int>dp(nums.size());
        dp[0] = nums[0];
        if(nums.size()==1){return dp[0];}
        dp[1] = max(nums[0],nums[1]);
        for(int id=2;id<nums.size();id++){
            dp[id] = max(dp[id-1],dp[id-2]+nums[id]);
        }
        return dp[nums.size()-1];
    }
};

//相比于198题 相当于0号和n号房是不能连在一起的 我们可以分开
//将nums分成两个序列 0-n-1 和 1-n 比较两者间的最大值



/*
213 打家劫舍II
你是一个专业的小偷，计划偷窃沿街的房屋，每间房内都藏有一定的现金。
这个地方所有的房屋都 围成一圈 ，这意味着第一个房屋和最后一个房屋是紧挨着的。
同时，相邻的房屋装有相互连通的防盗系统，如果两间相邻的房屋在同一晚上被小偷闯入，系统会自动报警 。

给定一个代表每个房屋存放金额的非负整数数组，计算你 在不触动警报装置的情况下 ，今晚能够偷窃到的最高金额
*/
class Solution {
public:
    int rob1(vector<int>& nums,int start,int end){
        vector<int>dp(end-start);
        dp[0] = nums[start];
        if(end-start == 1){return dp[0];}
        dp[1] = max(nums[start],nums[start+1]);
        for(int id=2;id<end-start;id++){
            dp[id] = max(dp[id-1],dp[id-2] + nums[id+start]);
        }
        return dp[dp.size()-1];
    }


    int rob(vector<int>& nums) {
        int len = nums.size();
        if(len==1){return nums[0];}
        if(len==2){return max(nums[0],nums[1]);}
        int max1 = rob1(nums,0,len-1);
        int max2 = rob1(nums,1,len);
        return max(max1,max2);
    }
};



/*
337 打家劫舍3
小偷又发现了一个新的可行窃的地区。这个地区只有一个入口，我们称之为 root 。

除了 root 之外，每栋房子有且只有一个“父“房子与之相连。一番侦察之后，
聪明的小偷意识到“这个地方的所有房屋的排列类似于一棵二叉树”。 如果 两个直接相连的房子在同一天晚上被打劫 ，房屋将自动报警。

给定二叉树的 root 。返回 在不触动警报的情况下 ，小偷能够盗取的最高金额 。

输入: root = [3,2,3,null,3,null,1]
输出: 7 
解释: 小偷一晚能够盗取的最高金额 3 + 3 + 1 = 7

*/

/*
对于一个根结点 会存在偷和不偷两个可能
1.偷 当前结点获得的最大钱数 = 左节点不偷 + 右节点不偷 + 当前根节点钱数
2.不偷 当前结点获得的最大钱数 = 左节点偷或不偷的最大值 + 右节点偷或不偷的最大值 的钱数
*/
class Solution {
public:
    vector<int> finderNode(TreeNode* root){
        if(root==nullptr){return {0,0};}
        vector<int>res(2,0);
        vector<int>left = finderNode(root->left);
        vector<int>right = finderNode(root->right);
        //不偷
        res[0] = max(left[0],left[1]) + max(right[0],right[1]);
         //偷
        res[1] = left[0] + right[0] + root->val;
        return res;
    }

 
    int rob(TreeNode* root) {
        vector<int> nums = finderNode(root);
        return max(nums[0],nums[1]);
    }
};