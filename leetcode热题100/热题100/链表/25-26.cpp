/*

25 141. 环形链表
如果链表中有某个节点，可以通过连续跟踪 next 指针再次到达，则链表中存在环。 为了表示给定链表中的环，评测系统内部使用整数 pos 来表示链表尾连接到链表中的位置（索引从 0 开始）。
注意：pos 不作为参数进行传递 。仅仅是为了标识链表的实际情况。
如果链表中存在环 ，则返回 true 。 否则，返回 false 。

输入：head = [3,2,0,-4], pos = 1
输出：true
解释：链表中有一个环，其尾部连接到第二个节点。

26 142. 环形链表 II
给定一个链表的头节点  head ，返回链表开始入环的第一个节点。 如果链表无环，则返回 null。
如果链表中有某个节点，可以通过连续跟踪 next 指针再次到达，则链表中存在环。 为了表示给定链表中的环，评测系统内部使用整数 pos 来表示链表尾连接到链表中的位置（索引从 0 开始）。
如果 pos 是 -1，则在该链表中没有环。注意：pos 不作为参数进行传递，仅仅是为了标识链表的实际情况。
不允许修改 链表。
*/


//这里题解是一个快慢指针追及问题 模型太过金典 应熟练记忆
//原理网址 https://leetcode.cn/problems/linked-list-cycle-ii/solutions/12616/linked-list-cycle-ii-kuai-man-zhi-zhen-shuang-zhi-/?envType=study-plan-v2&envId=top-100-liked

#include "util.h"

class Solution {
public:
    //141
    bool hasCycle(ListNode *head) {
        ListNode* slow =head;
        ListNode* fast =head;
        while(fast!=nullptr && fast->next!=nullptr){
            slow = slow->next;
            fast = fast->next->next;
            if(fast==slow){return true;}
        }
        return false;
    }
    //142
    ListNode *detectCycle(ListNode *head) {
        ListNode* slow =head;
        ListNode* fast =head;
        while(1){
            if(fast==nullptr || fast->next==nullptr){return nullptr;}
            slow = slow->next;
            fast = fast->next->next;  
            if(fast==slow){break;}
        }
        fast = head;
        while(slow!=fast){
            fast = fast->next;
            slow = slow->next;
        }
        return fast;
    }
};