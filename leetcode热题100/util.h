#ifndef __UTIL_H__
#define __UTIL_H__



#include<iostream>
#include<vector>
#include<unordered_map>
#include<unordered_set>
#include<string>
#include<algorithm>

#include <queue>
#include <dqeue>
#include <stack>
#include <priority_queue>
using namespace std;


struct ListNode {
      int val;
      ListNode *next;
      ListNode(int x) : val(x), next(NULL) {}
};

struct TreeNode {
      int val;
      TreeNode *left;
      TreeNode *right;
      TreeNode() : val(0), left(nullptr), right(nullptr) {}
      TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
      TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

#endif