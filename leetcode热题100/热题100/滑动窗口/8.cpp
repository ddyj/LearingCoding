
#include "util.h"

// 3. 无重复字符的最长子串

/*
给定一个字符串 s ，请你找出其中不含有重复字符的 最长子串 的长度。

 

示例 1:

输入: s = "abcabcbb"
输出: 3 
解释: 因为无重复字符的最长子串是 "abc"，所以其长度为 3。
*/

class Solution {
public:
    int get_max(const int &a,const int &b){
        return a>b ? a:b;
    }
    int lengthOfLongestSubstring(string s) {
        int left = 0;
        int right = 0;
        int max_len = 0;
        if(s.size() == 0){return 0;}
        //用hash代替窗口 这样可以快速判断这个字符在不在窗口中
        unordered_set<char>hash_table;
        for(;right<s.size();right++){
            while(hash_table.count(s[right])!=0){
                hash_table.erase(s[left]);
                left++;
            }
            hash_table.insert(s[right]);
            max_len = get_max(max_len,hash_table.size());
        }
        return max_len;
    }
};