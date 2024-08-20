

/*
105. 从前序与中序遍历序列构造二叉树
给定两个整数数组 preorder 和 inorder ，其中 preorder 是二叉树的先序遍历， 
inorder 是同一棵树的中序遍历，请构造二叉树并返回其根节点。

输入: preorder = [3,9,20,15,7], inorder = [9,3,15,20,7]
输出: [3,9,20,null,null,15,7]
*/


#include "util.h"


/*

//分治 + 递归

1.前序的第一个值一定是根节点
2.确定根节点后在中序遍历的结果找到根节点 在根节点的左侧的都是左子树 右侧的是右子树 由此可以确定左右子树的结点数
3.在前序遍历中 可以分为 根 左 右 当确定结点数后 将数组分为左右两个子树 
4.对两个子树我们采取相同的操作
*/

class Solution {
private:
    unordered_map<int,int>hash_table;
public:
    void create_hash(vector<int>&inorder){
        for(int id=0;id<inorder.size();id++){
            hash_table.insert(make_pair(inorder[id],id));
        }
    }
    int getPose(int target){
        return hash_table[target];
    }
    TreeNode* myBuildTree(vector<int>& preorder, vector<int>& inorder,
                    int preorder_left,int preorder_right,int inorder_left,int inorder_right){
        if(preorder_left > preorder_right){return nullptr;}
        if(inorder_left > inorder_right){return nullptr;}
        //当前树的根结点
        TreeNode* root = new TreeNode(preorder[preorder_left]);
        //由于树中无重复 元素 这里用哈希查找  id是根结点在中序遍历中的id
        int id = getPose(preorder[preorder_left]);
        //计算左右树长度
        int left_len = id - inorder_left;
        int right_len = inorder_right - id;
        
    
        root->left = myBuildTree(preorder,inorder,
                                    preorder_left+1,preorder_left+left_len,
                                    inorder_left,id-1);

        root->right = myBuildTree(preorder,inorder,
                                    preorder_right-right_len+1,preorder_right,
                                    id+1,inorder_right);
        return root;
    }
    TreeNode* buildTree(vector<int>& preorder, vector<int>& inorder) {
        create_hash(inorder);
        return myBuildTree(preorder,inorder,0,preorder.size()-1,0,preorder.size()-1); 
    }
};