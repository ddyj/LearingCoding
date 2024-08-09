#include "util.h"


/*
160. 相交链表
给你两个单链表的头节点 headA 和 headB ，请你找出并返回两个单链表相交的起始节点。
如果两个链表不存在相交节点，返回 null 。
输入：intersectVal = 8, listA = [4,1,8,4,5], listB = [5,6,1,8,4,5], skipA = 2, skipB = 3
输出：Intersected at '8'
解释：相交节点的值为 8 （注意，如果两个链表相交则不能为 0）。
从各自的表头开始算起，链表 A 为 [4,1,8,4,5]，链表 B 为 [5,6,1,8,4,5]。
在 A 中，相交节点前有 2 个节点；在 B 中，相交节点前有 3 个节点。
— 请注意相交节点的值不为 1，因为在链表 A 和链表 B 之中值为 1 的节点 (A 中第二个节点和 B 中第三个节点) 是不同的节点。
换句话说，它们在内存中指向两个不同的位置，而链表 A 和链表 B 中值为 8 的节点 (A 中第三个节点，B 中第四个节点) 在内存中指向相同的位置。
*/


//在两个链表运算时 双指针的应用是非常多的 所以需要我们熟练掌握双指针的设定和移动算法


//两种方式
class Solution {
public:
    //法一 : 哈希
    ListNode *getIntersectionNode_hash(ListNode *headA, ListNode *headB) {
        if(headA==nullptr||headB==nullptr){return nullptr;}
        unordered_set<ListNode*>hash_table;
        while(headA!=nullptr){
            hash_table.insert(headA);
            headA = headA->next;
        }     
        while(headB!=nullptr){
            if(hash_table.count(headB)!=0){
                return headB;
            }
            headB = headB->next;
        }
        return nullptr;
    }
    //法2 双指针追赶法 p1 = pheadA p2 = pHeadB  -- 类似的题目 快慢指针
    //遍历p1 p2 当p1==nullptr 时 p1=pheadB 当p2==nullprt 时 p2=pheadA
    //只有当p1==p2时 结果不论是不是空指针 都是我们要获取的
    //有以下情况：
    /*
    * 两个链表分别用A、B表示
    ** 如果A、B长度一致 那么p1 p2必然同时抵达 交点或两个链表的重点nullptr
    ** 如果A、B长度不一致 不妨假设A > B 此时p1 p2一定会完成一次互换起点
    ** 当p1 == p2时 如果A和B相交 其中A = a+c B=b+c p1的路程是 a+c+b p2的路程是b+c+a
    ** 或者AB不想交 A=a B=b p1 = p2 = a+b
    */ 
    ListNode *getIntersectionNode(ListNode *headA, ListNode *headB) {
        if(headA==nullptr||headB==nullptr){return nullptr;}
        ListNode* p1 = headA;
        ListNode* p2 = headB;
        while(p1 != p2){
            p1 = p1==nullptr ? headB : p1->next;
            p2 = p2==nullptr ? headA :p2->next;
        }
        return p1;
    }
};