#include "util.h"

/*
2. 两数相加
给你两个 非空 的链表，表示两个非负的整数。它们每位数字都是按照 逆序 的方式存储的，
并且每个节点只能存储 一位 数字。

请你将两个数相加，并以相同形式返回一个表示和的链表。

你可以假设除了数字 0 之外，这两个数都不会以 0 开头

输入：l1 = [2,4,3], l2 = [5,6,4]
输出：[7,0,8]
解释：342 + 465 = 807.
*/

class Solution {
public:
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        int add = 0; //进位判断
        ListNode* res = new ListNode(-1); //先驱
        ListNode* phead = res;
        while(l1!=nullptr || l2!=nullptr){
            int l1_num = l1==nullptr ? 0 :l1->val; 
            int l2_num = l2==nullptr ? 0 :l2->val;
            int cur_num = l1_num + l2_num + add;
            if(cur_num >= 10 ){
                cur_num = cur_num -10;
                add = 1; 
            }
            else{
                add = 0;
            } 
            phead->next = new ListNode(cur_num);
            phead = phead->next;

            l1 = l1==nullptr ? l1 : l1->next;
            l2 = l2==nullptr ? l2 : l2->next;
        }
        if(add!=0){
            phead->next = new ListNode(1);
        }
        
        return res->next;
    }
};