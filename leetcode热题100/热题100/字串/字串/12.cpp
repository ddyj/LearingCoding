#include "util.h"

//参考9.cpp

// LeetCode 76 题， 最小覆盖子串 hard

/*
给你一个字符串 s 、一个字符串 t 。返回 s 中涵盖 t 所有字符的最小子串。如果 s 中不存在涵盖 t 所有字符的子串，则返回空字符串 "" 。

 

注意：

对于 t 中重复字符，我们寻找的子字符串中该字符数量必须不少于 t 中该字符数量。
如果 s 中存在这样的子串，我们保证它是唯一的答案。

示例 1：

输入：s = "ADOBECODEBANC", t = "ABC"
输出："BANC"
解释：最小覆盖子串 "BANC" 包含来自字符串 t 的 'A'、'B' 和 'C'。
*/

class Solution {
public:
    string minWindow(string s, string t) {
        if(s.size() < t.size()){return "";}
        unordered_map<char,int>hash_word;
        unordered_map<char,int>hash_window;
        int window_length = t.size();

        //建表
        for(char c: t){hash_word[c]++;}

        //遍历滑动窗口
        int left = 0;
        int right = 0;
        int vaild = 0;

        string min_str;
        int start = 0;
        int min_len = INT_MAX;

        while(right < s.size()){
            char temp = s[right];
            right++;
            if(hash_word.count(temp)!=0){
                hash_window[temp]++;
                if(hash_window[temp] == hash_word[temp]){
                    vaild++;
                }
            }
            //这里有些不一样 我们需要确定的是最小窗口的长度 ，
            //所以这里的循环当所有hash_word中的值都出现时 才会移动(缩小窗口)
            while(vaild == hash_word.size()){
                if(right-left < min_len){
                    min_len = right-left;
                    start = left;
                }
                //缩小窗口
                temp = s[left];
                left++;
                if(hash_word.count(temp)!=0){
                    if(hash_window[temp] == hash_word[temp]){vaild--;}
                    hash_window[temp]--;
                }
            }
        }
        return min_len == INT_MAX ? "" : s.substr(start, min_len);
    }
};