#include "util.h"



//计算二叉树的最大最小深度


//最大深度
class Solution {
public:
    void getMax_dep(TreeNode* root,int& maxdep,int cur_dep){
        if(root==nullptr){return;}
        maxdep = max(maxdep,cur_dep);
        getMax_dep(root->left,maxdep,cur_dep+1);
        getMax_dep(root->right,maxdep,cur_dep+1);
    }
    int maxDepth(TreeNode* root) {
        int maxdep = 0;
        getMax_dep(root,maxdep,1);
        return maxdep;
    }

    //2023-10-22更新
    int getMax_dep(TreeNode* root){
        if(root==nullptr){return 0;}
        
        int left_dep =  1+ getMax_dep(root->left);
        int right_dep = 1+ getMax_dep(root->right);

        return max(left_dep,right_dep);
    }
    int maxDepth(TreeNode* root) {
        int res = getMax_dep(root);
        return res;
    }
};




//TODO： 543. 二叉树的直径
//详情见cpp40

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







//最小深度
//计算左右子树的长度 并返回最小值
class Solution {
public:
    int minDepth(TreeNode* root) {
        if(root==nullptr){return 0;}
        //由于根结点不算做子叶 当前子树的根结点的左或右子树没有结点时 返回左右子数中最大的长度(为null一侧为0)
        if(root->left==nullptr || root->right == nullptr){
            return 1+max(minDepth(root->left),minDepth(root->right));
        }
        //当是正常二叉树的时候，返回左子树长度和右子树长度的最小值
        return 1+min(minDepth(root->left),minDepth(root->right));
    }
};