

#include "util.h"

/*
给你一个单链表的头节点 head ，请你判断该链表是否为回文链表。如果是，返回 true ；否则，返回 false 。
输入：head = [1,2,2,1]
输出：true
*/



class Solution {
public:
    bool isPalindrome(ListNode* head) {
        vector<int>nums;
        while(head!=nullptr){nums.emplace_back(head->val);head=head->next;}
        int left =0;
        int right = nums.size()-1;
        while(left<right){
            if(nums[left]!=nums[right]){return false;}
            left++;
            right--;
        }
        return true;
    }
};