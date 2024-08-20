#include "util.h"

/*
24. 两两交换链表中的节点
给你一个链表，两两交换其中相邻的节点，并返回交换后链表的头节点。
你必须在不修改节点内部的值的情况下完成本题（即，只能进行节点交换）。
输入：head = [1,2,3,4]
输出：[2,1,4,3]
*/

/*
题根： 交换链表中的两个结点
       方法 加一个前哨点辅助
例：    1  -  2 
    pre    -   1    -   2   - 3  -4          pre   -   2   - 1  - 3 - 4         pre  2  1  3  4
                                     -- >                               -- >
    pre        s        f     t              pre       f     s    t                     pre s f
                          temp = fast->next
                          pre->next = fast
                          fast->next = slow
                          slow = temp  

而在本体中 核心是不断移动pre slow fast这个三指针窗口 
由于是移动两两结点 下一个结点的front应该在当前的slow的位置

图解：
https://leetcode.cn/problems/swap-nodes-in-pairs/solutions/444474/liang-liang-jiao-huan-lian-biao-zhong-de-jie-di-91/?envType=study-plan-v2&envId=top-100-liked
*/

class Solution {
public:
    ListNode* swapPairs(ListNode* head) {
        if(head==nullptr){return head;}
        ListNode* front = new ListNode(-1);
        front->next = head;

        ListNode* TEMP = front;
        ListNode* fast = head->next;
        ListNode* slow = head;
   
        while(fast!=nullptr&&slow!=nullptr){
            ListNode* tmp = fast->next;
            front->next = fast;
            fast->next = slow;
            slow->next = tmp;

            front = slow;
            slow = slow->next;
            if(slow==nullptr){break;}
            fast = slow->next;
        }
        return TEMP->next;
    }
}