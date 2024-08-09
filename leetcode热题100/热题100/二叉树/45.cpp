#include "util.h"



/*
199. 二叉树的右视图
给定一个二叉树的 根节点 root，想象自己站在它的右侧，按照从顶部到底部的顺序，返回从右侧所能看到的节点值。
输入: [1,2,3,null,5,null,4]
输出: [1,3,4]
*/



//方案1 层排列后拿每一层的最右边的元素
class Solution {
public:
    void Level_Sort(TreeNode*root,vector<vector<int>>&sort_level,int cur_level){
        if(root==nullptr){return;}
        if(sort_level.size()<cur_level){vector<int>temp;sort_level.emplace_back(temp);}
        sort_level[cur_level-1].emplace_back(root->val);
        Level_Sort(root->left,sort_level,cur_level+1);
        Level_Sort(root->right,sort_level,cur_level+1);
    }
    vector<int> rightSideView(TreeNode* root) {
        vector<vector<int>>sort_level;
        vector<int>res;
        int cur_level = 1;
        Level_Sort(root,sort_level,cur_level);

        for(int id=0;id<sort_level.size();id++){
            int len = sort_level[id].size()-1;
            res.emplace_back(sort_level[id][len]);
        }

        return res;
    }
};