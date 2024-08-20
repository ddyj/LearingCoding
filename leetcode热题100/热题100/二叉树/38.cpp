
#include "util.h"


/*
226 翻转二叉树
给你一棵二叉树的根节点 root ，翻转这棵二叉树，并返回其根节点。
输入：root = [4,2,7,1,3,6,9]
输出：[4,7,2,9,6,3,1]
*/

//经典的递归思想 要翻转一颗二叉树 就是先互换左右子树 然后再互换左右子树中的左右子树 ……
class Solution {
public:
    TreeNode* invertTree(TreeNode* root) {
        if(root==nullptr){return nullptr;}
        //对左右子树的交换
        TreeNode* temp = root->left;
        root->left = root->right;
        root->right = temp;
        //对左右子树的结点进行交换
        invertTree(root->left);
        invertTree(root->right);

        return root;
    }
};