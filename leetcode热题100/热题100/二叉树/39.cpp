#include "util.h"

/*
101. 对称二叉树
给你一个二叉树的根节点 root ， 检查它是否轴对称。
*/

/*
实际上就是对比左子树和右子树是否对称
也就是对比左子树的左子树和右子树的右子树
        左子树的右子树和右子树的左子树是否对称 ……
*/
class Solution {
public:
    bool is_symmetry(TreeNode* left_root,TreeNode* right_root){
        if(left_root==nullptr && right_root == nullptr){return true;}
        else if(left_root==nullptr || right_root==nullptr){return false;}
        if(left_root->val!=right_root->val){return false;}
        return is_symmetry(left_root->left,right_root->right) && is_symmetry(left_root->right,right_root->left);
    }
    bool isSymmetric(TreeNode* root) {
        if(root==nullptr){return true;}
        return is_symmetry(root->left,root->right);
    }
};