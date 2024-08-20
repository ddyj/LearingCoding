/*
21. 合并两个有序链表
将两个升序链表合并为一个新的 升序 链表并返回。新链表是通过拼接给定的两个链表的所有节点组成的。 

输入：l1 = [1,2,4], l2 = [1,3,4]
输出：[1,1,2,3,4,4]
*/


/*
穿针法解释 
从两个链表中元素出发  res依次指向两个list中的小数
如果list1中数小 则res->next = list1 同时 list1 = list1->next 交叉式的将两个链表串起来
        1  2  4                   2 4
res               -->   res - 1 
        1  2  3                  1 2 3
*/



#include "util.h"

//穿针引线
class Solution {
public:
    ListNode* mergeTwoLists(ListNode* list1, ListNode* list2) {
        ListNode *res = new ListNode(-1);
        ListNode *phead = res;
        while(list1!=nullptr && list2!=nullptr){
            if(list1->val < list2->val){
                phead->next = list1;
                list1 = list1->next;
                phead = phead->next;
            }
            else{
                phead->next = list2;
                list2 = list2->next;
                phead = phead->next;
            }
        }
        if(list1==nullptr){phead->next = list2;}
        else if(list2==nullptr){phead->next = list1;}
        return res->next;
    }
};



//穿针法进阶
/*
T34 
23. 合并 K 个升序链表
给你一个链表数组，每个链表都已经按升序排列。
请你将所有链表合并到一个升序链表中，返回合并后的链表。
输入：lists = [[1,4,5],[1,3,4],[2,6]]
输出：[1,1,2,3,4,4,5,6]
解释：链表数组如下：
[
  1->4->5,
  1->3->4,
  2->6
]
将它们合并到一个有序链表中得到。
1->1->2->3->4->4->5->6
*/


class Solution {
public:
    bool is_allNull(vector<ListNode*>& lists){
        for(ListNode* t :lists){
            if(t!=nullptr){return true;}
        }
        return false;
    }
    ListNode* mergeKLists(vector<ListNode*>& lists) {
        ListNode* res = new ListNode(-1);
        ListNode* phead = res;
        while(is_allNull(lists)){
            ListNode* minList = lists[0];
            int min_id = 0;
            for(int id=1;id<lists.size();id++){
                if(minList==nullptr){minList = lists[id];min_id = id;}
                if(lists[id]==nullptr){continue;}
                else{
                    if(minList->val > lists[id]->val){
                        minList = lists[id];
                        min_id = id;
                    }
                }
            }
            if(min_id==lists.size()-1 && minList==nullptr ){break;}
            phead->next = lists[min_id];
            phead = phead->next;
            lists[min_id] = lists[min_id]==nullptr ? nullptr : lists[min_id]->next; 
        }
        return res->next;
    }
};

//改进 使用归并排序的方式
class Solution {
public:
    ListNode* merge2Lists(ListNode*res,ListNode*add_list){
        ListNode* ans = new ListNode(-1);
        ListNode* head = ans;
        while(res!=nullptr && add_list!=nullptr){
            if(res->val < add_list->val){
                ans->next = res;
                ans = ans->next;
                res = res->next;
            }
            else{
                ans->next = add_list;
                ans = ans->next;
                add_list = add_list->next;
            }
        }
        if(res!=nullptr){
            ans->next = res;
        }
        if(add_list!=nullptr){
            ans->next = add_list;
        }
        return head->next;
    }

    ListNode* mergeSort(vector<ListNode*>&lists,int start_pose,int end_pose){
        if(end_pose <= start_pose ){return lists[start_pose];}
        int mid = start_pose + (end_pose - start_pose)/2;
        return merge2Lists(mergeSort(lists,start_pose,mid),mergeSort(lists,mid+1,end_pose));
    }

    ListNode* mergeKLists(vector<ListNode*>& lists) {
        if(lists.size() == 0){return nullptr;}
        if(lists.size() == 1){return lists[0];}
        return mergeSort(lists,0,lists.size()-1);
    }
};



//对任意数组实现归并排序
class Solution {
public:
       void merege2arr(vector<int>&nums,int start,int mid,int end){
            vector<int>temp(nums.size());
            int s_a = start;
            int e_a = mid;
            int s_b = mid+1;
            int e_b = end;
            int count = start;
            int find_a = s_a;
            int find_b = s_b;
            while(find_a <= e_a && find_b <= e_b){
                if(nums[find_a] < nums[find_b]){
                    temp[count++] = nums[find_a++];
                }
                else{
                    temp[count++] = nums[find_b++];
                }
            }
            while(find_a <= e_a){
                temp[count++] = nums[find_a++];
            }
            while(find_b <= e_b){
                temp[count++] = nums[find_b++];
            }
            
            for(int i = start ; i <= end; i++){
                nums[i] = temp[i];
            } 
        }
        void reverse_sort(vector<int>&nums,int start,int end){
            if(end <= start){return;}
            int mid = start + (end - start)/2;
            reverse_sort(nums,start,mid);
            reverse_sort(nums,mid+1,end);      
            // 如果数组的这个子区间本身有序，无需合并
            if(nums[mid] <= nums[mid+1]){return;}
            merege2arr(nums,start,mid,end);
        }
        vector<int> sortArray(vector<int>& nums) {
            reverse_sort(nums,0,nums.size()-1);
            return nums;
        }
};