#include "util.h"

/*
206. 反转链表
给你单链表的头节点 head ，请你反转链表，并返回反转后的链表。
输入：head = [1,2,3,4,5]
输出：[5,4,3,2,1]
*/

//图解网址：
//https://leetcode.cn/problems/reverse-linked-list/solutions/2361282/206-fan-zhuan-lian-biao-shuang-zhi-zhen-r1jel/?envType=study-plan-v2&envId=top-100-liked


/*
    1 - 2 - 3 - 4 - nullptr
  phead tmp          front

    tmp = phead->next
    phead->next = front

    1 - nullptr - 2 - 3 - 4 
    p     f       t

    front = phead
    phead = tmp        

    1 - nullptr - 2 - 3 - 4
    f             p   t

    2 - 1 - nullptr - 3 - 4
    f                 p   t


*/






class Solution {
public:
    ListNode* reverseList(ListNode* head) {
        ListNode* phead = head;
        if(phead==nullptr){return phead;}
        ListNode* front = nullptr;
        while(phead!=nullptr){
           ListNode* next = phead->next;
           phead->next = front;
           front = phead;
           phead = next;
            
        }
        return front;
    }

    //使用栈的方式翻转链表
    ListNode* reverseList(ListNode* head) {
        ListNode * phead = head;
        ListNode * front = new ListNode(0);
        ListNode * pre = front;
        stack<ListNode*>table;

        while(phead!=nullptr){
            table.push(phead);
            phead = phead->next;
        }
        while(!table.empty()){
            //std::cout<<table.top()->val<<std::endl;
            pre->next = table.top();
            table.pop();
            pre = pre->next;
        }
        //这里需要对最后一个结点的next更新为null 否则链表将回环
        pre->next = nullptr;
        return front->next;
    }
};




/*
92 交换链表2

给你单链表的头指针 head 和两个整数 left 和 right ，其中 left <= right 。
请你反转从位置 left 到位置 right 的链表节点，返回 反转后的链表 。

输入：head = [1,2,3,4,5], left = 2, right = 4
输出：[1,4,3,2,5]
*/
class Solution {
public:

    pair<ListNode*,ListNode*> reverseList(ListNode* start,ListNode* end){
        ListNode* front = end->next;
        ListNode* pre = start;
        while(end!=front){
            ListNode* tmp = pre->next;
            pre->next = front;
            front = pre;
            pre = tmp;
        }
        return make_pair(end,start);
    }

    ListNode* reverseBetween(ListNode* head, int left, int right) {
        ListNode* start = head;
        ListNode* end = head;
        ListNode* pstart = new ListNode(0);
        pstart->next = start;
        ListNode* res = pstart;


        bool done = false;
        int left_pose = 1;
        int right_pose = 1;

        while(!done){
            if(left_pose < left && start!=nullptr && pstart!=nullptr){
                start = start->next;
                pstart = pstart->next;
                left_pose++;
            }
            if(right_pose < right && end!=nullptr){
                end = end->next;
                right_pose++;
            }
            if(right_pose>=right && left_pose >= left){done = true;}
        }

        ListNode * next_node =  end->next; 
        pair<ListNode*,ListNode*>tuble = reverseList(start,end);
        //更新起点和终点
        start = tuble.first;
        end = tuble.second;

        //将翻转后的链表接入链表中
        pstart->next = start;
        end->next = next_node;

        return res->next;
    }
};







/*
25 k个一组翻转链表
给你链表的头节点 head ，每 k 个节点一组进行翻转，请你返回修改后的链表。

k 是一个正整数，它的值小于或等于链表的长度。
如果节点总数不是 k 的整数倍，那么请将最后剩余的节点保持原有顺序。

你不能只是单纯的改变节点内部的值，而是需要实际进行节点交换。

输入：head = [1,2,3,4,5], k = 2
输出：[2,1,4,3,5]
*/


class Solution {
public:

    //法一 逐次交换
    //https://leetcode.cn/problems/reverse-nodes-in-k-group/solutions/248591/k-ge-yi-zu-fan-zhuan-lian-biao-by-leetcode-solutio/
    pair<ListNode*,ListNode*> reverseList(ListNode* start,ListNode* tail){
        ListNode* front = tail->next;
        ListNode* phead = start;

        //由于这里翻转的链表前后可能有值 这里就类似上文写法 front是206. 反转链表中的nullptr
        while(front!=tail){
            ListNode * tmp = phead->next;
            phead->next = front;
            front = phead;
            phead = tmp;
        }
        return make_pair(tail,start);
    }


    ListNode* reverseKGroup(ListNode* head, int k) {
        ListNode * pre = new ListNode(0);
        pre->next = head;
        ListNode * phead = pre;
        
        while(head!=nullptr){
            //phead相当于pstart
            ListNode * tail = phead;
            for(int i=0;i<k;i++){
                tail = tail->next;
                if(tail==nullptr){
                    return pre->next;
                }
            }
            ListNode* next_node = tail->next;

            //返回的res第一个值是翻转链表的头  第二个值是翻转链表的结尾
            pair<ListNode*,ListNode*> res = reverseList(head,tail);

            //更新头结点
            head = res.first;
            //更新尾部结点
            tail = res.second;

            //将翻转后的链表接入
            //phead始终是head的前一个结点 这里将翻转后的链表接入phead中
            phead->next = head;
            //尾部结点的指向next
            tail->next = next_node;

            //对于下一次循环来讲 phead是pstart head是start
            //更新phead
            phead = tail;
            //head始终是phead的下一位
            head = tail->next;
       
        }   
        return pre->next;
    }


    //法二 使用栈 由于栈本身就是先入后出
    ListNode* reverseKGroup(ListNode* head, int k) {
        stack<ListNode*>table;
        
        ListNode * pre = new ListNode(0);
        ListNode * phead = pre;

        while(head!=nullptr){
            ListNode * tmp = head;
            for(int i=0;tmp!=nullptr && i<k;i++){
                table.push(tmp);
                tmp = tmp->next;
            }
            if(table.size() < k){break;}

            while(!table.empty()){
                phead->next = table.top();
                table.pop();
                phead = phead->next;
            }
            head = tmp;
        }

        //补充剩余点
        phead->next = head;

        return pre->next;
    }

};