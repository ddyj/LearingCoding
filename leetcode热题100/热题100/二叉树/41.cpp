#include "util.h"



/*
102. 二叉树的层序遍历
给你二叉树的根节点 root ，返回其节点值的 层序遍历 。 （即逐层地，从左到右访问所有节点）。
输入：root = [3,9,20,null,null,15,7]
输出：[[3],[9,20],[15,7]]
*/



class Solution {
public:
    void levelOrderByRes(TreeNode* root,vector<vector<int>>&res,int cur_level){
        if(root == nullptr){return;}
        if(res.size()<cur_level){
            vector<int>temp;
            res.emplace_back(temp);
        }
        
        levelOrderByRes(root->left,res,cur_level+res[cur_level-1].emplace_back(root->val);1);
        levelOrderByRes(root->right,res,cur_level+1);
    }
    vector<vector<int>> levelOrder(TreeNode* root) {
        vector<vector<int>>res;
        levelOrderByRes(root,res,1);
        return res;
    }
};

//BFS做法
class Solution {
public:
    vector<vector<int>> levelOrder(TreeNode* root) {
        //BFS
        vector<vector<int>>res;
        queue<TreeNode*>table;
        table.push(root);
        int cur_level = 0;
        while(!table.empty()){

            int n = table.size();
            //使用for循环计数 当处理了当前循环次数中的size的个数时 就说明时下一层了
            for(int i=0;i<n;i++){
                TreeNode* tmp = table.front();
                if(tmp == nullptr){
                    table.pop();
                    continue;
                }
                if(res.size() != cur_level+1){
                    res.resize(cur_level+1);
                }
                res[cur_level].emplace_back(tmp->val);
                table.pop();
                table.push(tmp->left);
                table.push(tmp->right);
            }
            cur_level++;    
        }
        return res;
    }
};