#include "util.h"


/*
98. 验证二叉搜索树
给你一个二叉树的根节点 root ，判断其是否是一个有效的二叉搜索树。

有效 二叉搜索树定义如下：

节点的左子树只包含 小于 当前节点的数。
节点的右子树只包含 大于 当前节点的数。
所有左子树和右子树自身必须也是二叉搜索树。
*/


//二叉搜索树的中序遍历一定是升序的
class Solution {
public:
    void midTravel(TreeNode* root,long long &cur,bool& flag){
        if(root==nullptr){return;}
        if(!flag){return;}
        midTravel(root->left,cur,flag);
        if(cur<root->val){cur = root->val;}
        else{flag = false;}
        midTravel(root->right,cur,flag);
    }
    bool isValidBST(TreeNode* root) {
        if(root->left==nullptr && root->right==nullptr){return true;}
        long long min_value = -9223372036854775808LL;
        bool res = true;
        midTravel(root,min_value,res);
        return res;
    }
};


/*
230. 二叉搜索树中第K小的元素
给定一个二叉搜索树的根节点 root ，和一个整数 k ，
请你设计一个算法查找其中第 k 个最小元素（从 1 开始计数）
输入：root = [3,1,4,null,2], k = 1
输出：1
*/

//由于二叉搜索树是升序的 那么第k小(从1开始)应该是中序遍历找到的第k个元素
class Solution {
public:
    void midTravel(TreeNode*root,int k,int& cur_id,int& res){
        if(cur_id > k || root==nullptr){return;}
        midTravel(root->left,k,cur_id,res);
        cur_id++;
        if(cur_id==k){res=root->val;}
        midTravel(root->right,k,cur_id,res);
    }
    int kthSmallest(TreeNode* root, int k) {
        int res = 0;
        int cur_id = 0;
        midTravel(root,k,cur_id,res);
        return res;
    }
};