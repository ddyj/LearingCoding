//二叉树的三序遍历


/*
前序遍历  根 - 左 - 右
中序遍历  左 - 根 - 右
后续遍历  左 - 右 - 根

*/

#include "util.h"

class Solution {
public:
    //中序遍历
    void midTraversal(TreeNode* root,vector<int>&res){
        if(root==nullptr){return ;}
        midTraversal(root->left,res);
        res.emplace_back(root->val);
        midTraversal(root->right,res);
    }


    vector<int> inorderTraversal(TreeNode* root) {
        vector<int>res;
        midTraversal(root,res);
        return res;
    }

    //前序遍历
    void frontTraversal(TreeNode*root,vector<int>&res){
        if(root==nullptr){return ;}
        res.emplace_back(root->val);
        frontTraversal(root->left,res);
        frontTraversal(root->right,res);
    }
    vector<int> preorderTraversal(TreeNode* root) {
        vector<int>res;
        frontTraversal(root,res);
        return res;
    }

    //后续遍历
    void backTraversal(TreeNode*root,vector<int>&res){
        if(root==nullptr){return ;}  
        backTraversal(root->left,res);
        backTraversal(root->right,res);
        res.emplace_back(root->val);
    }
    vector<int> postorderTraversal(TreeNode* root) {
        vector<int>res;
        backTraversal(root,res);
        return res;
    }
};