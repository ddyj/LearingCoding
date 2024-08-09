#include"util.h"

//堆排序
class Solution {
public:
    struct cmp{
        bool operator()(ListNode* p1,ListNode* p2){
             return p1->val > p2->val;
        }
    };

    ListNode* sortList(ListNode* head) {
        priority_queue<ListNode*,vector<ListNode*>,cmp>tree;
        while(head!=nullptr){
            tree.push(head);
            head= head->next;
        }
        ListNode* res = new ListNode(-1);
        ListNode* ans = res;
        while(!tree.empty()){
            res->next = tree.top();
            tree.pop();
            res = res->next;
        }
        res->next = nullptr;
        return ans->next;
    }
};

//手写版堆排序
class Solution {
public:
    void my_swap(vector<ListNode*> &record,int a,int b){
        if(a == b){return ;}
        ListNode * tmp = record[a];
        record[a] = record[b];
        record[b] = tmp;
    }
    void heapfiy(vector<ListNode*> &record,int pose,int len){
        if(len==0){return;}
        int lpose = pose*2+1;
        int rpose = pose*2+2;
        int min_pose = pose;
        if(lpose<len && record[lpose]->val < record[min_pose]->val){
            min_pose = lpose;
        }
        if(rpose<len && record[rpose]->val < record[min_pose]->val){
            min_pose = rpose;
        }
        if(min_pose!=pose){
            my_swap(record,pose,min_pose);
            heapfiy(record,min_pose,len);
        }
    }
    ListNode* heapsort(vector<ListNode*> &record,int len){
        for(int i= (len-1-1)/2 ; i>=0 ;i--){
            heapfiy(record,i,len);
        }
        ListNode* res = new ListNode(-1);
        ListNode* phead = res;
        for(int i=len-1;i>=0;i--){
            phead ->next = record[0];
            phead = phead->next;
            my_swap(record,0,i);
            heapfiy(record,0,i);  
        }
        phead->next = nullptr;
        return res->next;
    }

    ListNode* sortList(ListNode* head) {
        if(head==nullptr){return head;}
        vector<ListNode*>record;
        while(head!=nullptr){
            record.emplace_back(head);
            head=head->next;
        }
        int len = record.size();
  
        ListNode *res = heapsort(record,len);
        return res;
    }
};

//快排
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */

//快排
class Solution {
public:
    ListNode* quicksort(ListNode* head){
        //在快排中这里是判断left >= right 
        //这里head是left 而随着拆分的进行 最底层的结果就是head为空，或者head->next 为空
        //子链长度为0或1
        if(head == nullptr || head->next == nullptr){
            return head;
        }
        ListNode* lefthead = partition(head);
        ListNode* righthead = head->next;
        //这里必须断链 将其拆分为两个表
        head->next = nullptr;
        ListNode* left =  quicksort(lefthead);
        ListNode* right = quicksort(righthead);

        return connectTwoList(left,right);
    }
    //快排核心 找出中心元素 
    ListNode* partition(ListNode* head){
        ListNode* p = head;
        int pivot = p->val;
        p = p->next;
        //这里和数组快排不一样，由于我们没有办法对单向链表反向操作，right--操作
        //所以这里新定义左、右端点 将小于pivt的结点接到左边 大于的接到右边
        ListNode *leftDummyNode = new ListNode(-1);
        ListNode *rightDummyNode = nullptr;
        ListNode *leftTail = leftDummyNode;
       

        while(p!=nullptr){
            while(p!=nullptr && p->val < pivot){
                leftDummyNode->next = p;
                leftDummyNode = leftDummyNode->next;
                p = p->next;
            }
            //把等号赋给右侧序列 这样避免了如果当前序列中节点值都相等，没有分出左右导致的死循环
            while(p!=nullptr && p->val >= pivot){
                ListNode* nextnode = p->next;
                p->next = rightDummyNode;
                rightDummyNode = p;
                p = nextnode;
            }
        }
        
        leftDummyNode->next = head;
        leftDummyNode->next->next = rightDummyNode;
        return leftTail->next;
    }


    ListNode* connectTwoList(ListNode* left,ListNode*right){
        ListNode* pl = left;
        while(pl->next!=nullptr){
            pl = pl->next;
        }
        pl->next = right;
        return left;
    }

    ListNode* sortList(ListNode* head) {
        return quicksort(head);
    }
};