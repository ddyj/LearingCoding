
#include "util.h"


/*
19. 删除链表的倒数第 N 个结点
给你一个链表，删除链表的倒数第 n 个结点，并且返回链表的头结点。
输入：head = [1,2,3,4,5], n = 2
输出：[1,2,3,5]
*/



class Solution {
public:
    ListNode* removeNthFromEnd(ListNode* head, int n) {
        ListNode* fast = head;
        ListNode* slow = head;
        int count = 0;
        while(count<n){
            fast = fast->next;
            count++;
        }
        //如果fast直接超出链表有限范围 说明此时删除的一定是第一个元素
        if(fast==nullptr){
            head = head->next;
        }

        //当fast指向链表最后一个有效数位的时候 slow正向要删除的上一个结点
        while(fast!=nullptr && fast->next!=nullptr){
            fast = fast->next;
            slow = slow ->next;
        }
 
        if(slow->next!=nullptr){
            slow->next = slow->next->next;
        }

        
        return head;

    }
};