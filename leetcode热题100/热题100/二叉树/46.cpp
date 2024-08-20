#include "util.h"

/*
114. 二叉树展开为链表
给你二叉树的根结点 root ，请你将它展开为一个单链表：

展开后的单链表应该同样使用 TreeNode ，其中 right 子指针指向链表中下一个结点，而左子指针始终为 null 。
展开后的单链表应该与二叉树 先序遍历 顺序相

输入：root = [1,2,5,3,4,null,6]
输出：[1,null,2,null,3,null,4,null,5,null,6]
*/


class Solution {
public:
    void frontTravel(TreeNode* root,vector<TreeNode*>&nodeList){
        if(root==nullptr){return;}
        nodeList.emplace_back(root);
        frontTravel(root->left,nodeList);
        frontTravel(root->right,nodeList);
    }
    void flatten(TreeNode* root) {
        if(root == nullptr){return;}
        vector<TreeNode*>nodeList;
        frontTravel(root,nodeList);
        for(int id=0;id<nodeList.size()-1;id++){
            nodeList[id]->left = nullptr;
            nodeList[id]->right = nodeList[id+1];
        }
        nodeList[nodeList.size()-1]->left =nullptr;
        nodeList[nodeList.size()-1]->right =nullptr;
        root = nodeList[0];
    }
};