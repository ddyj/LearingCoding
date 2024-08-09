#include "util.h"



/*
108. 将有序数组转换为二叉搜索树
给你一个整数数组 nums ，其中元素已经按 升序 排列，请你将其转换为一棵 高度平衡 二叉搜索树。
高度平衡 二叉树是一棵满足「每个节点的左右两个子树的高度差的绝对值不超过 1 」的二叉树。
*/





/*
二叉搜索树（Binary Search Tree，BST）是一种特殊的二叉树数据结构，它具有以下特点：

对于每个节点，其左子树上的所有节点的值都小于它的值。
对于每个节点，其右子树上的所有节点的值都大于它的值。
左子树和右子树也都是二叉搜索树。

二叉搜索数的中序遍历是一个升序排列
本题相当于用升序序列恢复二叉搜索树

因为要求是平衡树 所以二叉树的根结点一定是在数组的中间位置


我们可以通过拆分子区间的方式对此题遍历，每次选择子区间的中间值放入 
*/
class Solution {
public:
    TreeNode* create_Tree(vector<int>nums,int start,int end){
        if(start > end){return nullptr;}
        int mid = (start + end)/2;
        TreeNode* root = new TreeNode(nums[mid]);
        root->left = create_Tree(nums,start,mid-1);
        root->right = create_Tree(nums,mid+1,end);
        return root;
    }
    TreeNode* sortedArrayToBST(vector<int>& nums) {
        return create_Tree(nums,0,nums.size()-1);
    }
};



//相较于上题 难点是如何确定子链表区间的中间值是多少
/*
    方法1 : 遍历链表转换为数组
    方法2 : 计算出链表的长度n  
            令left = 0 、right = n
            每个子区间的中值为 mid = l + (r-l)/2 
            偏移量为(r-l)/2

*/


//进阶：109. 有序链表转换二叉搜索树
/*
给定一个单链表的头节点  head ，其中的元素 按升序排序 ，将其转换为高度平衡的二叉搜索树。
本题中，一个高度平衡二叉树是指一个二叉树每个节点 的左右两个子树的高度差不超过 1。

输入: head = [-10,-3,0,5,9]
输出: [0,-3,9,-10,null,5]
解释: 一个可能的答案是[0，-3,9，-10,null,5]，它表示所示的高度平衡的二叉搜索树。
*/

class Solution {
public:
    ListNode* getMid(ListNode* head,int t){
        ListNode* phead = head;
        while(t>0){
            phead = phead->next;
            t--;
        }
        return phead;
    }
    TreeNode* create_Tree(ListNode* head,int start,int end){
        if(start > end){return nullptr;}
        int t = (end - start)/2;
        ListNode* cur = getMid(head,t);
        TreeNode* root = new TreeNode(cur->val);

        root->left = create_Tree(head,start,start+t-1);
        //注意这里传入的值
        root->right = create_Tree(cur->next,start+t+1,end);
        return root;
    }
    TreeNode* sortedListToBST(ListNode* head) {
        if(head==nullptr){return nullptr;}
        ListNode* phead = head;
        int list_len = 0;
        while(phead!=nullptr){phead=phead->next;list_len++;}
        return create_Tree(head,0,list_len-1);
    }
};