#include "util.h"

/*
543. 二叉树的直径
给你一棵二叉树的根节点，返回该树的 直径 。
二叉树的 直径 是指树中任意两个节点之间最长路径的 长度 。这条路径可能经过也可能不经过根节点 root 。
两节点之间路径的 长度 由它们之间边数表示。

输入：root = [1,2,3,4,5]
输出：3
解释：3 ，取路径 [4,2,1,3] 或 [5,2,1,3] 的长度。
*/


//基于二叉树的最大深度



//对于一个结点 这个结点的最长路径应该是当前结点向左子树的深度 + 向右子树的深度 - 1(左右深度都会从结点开始计算，相当于多加了一个根结点的数量) 
//而一颗数的直径应当是所有根结点的最长路径的最大值

class Solution {
public:
    int cal_dep(TreeNode* root,int &max_path){
        if(root==nullptr){return 0;}
        int left_dep = 1 + cal_dep(root->left,max_path);
        int right_dep = 1 + cal_dep(root->right,max_path); 
        max_path = max(max_path,left_dep+right_dep-1);
        return max(left_dep,right_dep);
    }

    int diameterOfBinaryTree(TreeNode* root) {
        int max_path = 0;
        cal_dep(root,max_path);
        //深度是结点的个数 这里的路径长度是边的个数 需要减一
        return max_path-1;
    }
};