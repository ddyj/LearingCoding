//滑动窗口法总结 网址：
//https://leetcode.cn/problems/find-all-anagrams-in-a-string/solutions/9749/hua-dong-chuang-kou-tong-yong-si-xiang-jie-jue-zi-/?envType=study-plan-v2&envId=top-100-liked


#include "util.h"


// 438. 找到字符串中所有字母异位词

// 滑动窗口总结

// 1 建立比较数据和检测窗口的哈希表 -- hash_window（记录window） hash_word（记录p）
// !* trick 比较是否一致 可以使用一个 vaild 当hash_window中存入新数据 就和hash_Word中比较 如果相同 vaild++ 
//    如果vaild大小和hash_word的大小一致 说明窗口内一致字符
// 2 增加窗口 right++ 向hash_window中加参数 并且判读vaild值是否相同
// 3 减小窗口 当right - left > window.size的时候 就需要减小 left++ 窗口右移 同时更新vaild和hash_window
// 4 减小窗口时什么时候是满足题设的 需要存储
class Solution {
public:
    vector<int> findAnagrams(string s, string p) {
        vector<int>res;
        if(p.size() > s.size()){return res;}
        unordered_map<char,int>hash_word;
        unordered_map<char,int>hash_window;
        //建立查询表
        for(char c:p){hash_word[c]++;}
        //建立滑动窗口
        int window_length = p.size();
        int left = 0;
        int right = 0;
        int vaild = 0; //判断窗口值是否相等
        while(right<s.size()){
            char c = s[right];
            right++;
            //向右侧滑动窗口时 更新哈希表
            if(hash_word.count(c)!=0){
                hash_window[c]++;
                if(hash_window[c] == hash_word[c]){
                    vaild++;
                }
            }
            //此时需要判断是否需要缩小左侧窗口
            while(right - left >= window_length){
                //此时这个窗口满足要求 存入res
                if(vaild == hash_word.size()){
                    res.emplace_back(left);
                }
                char d = s[left];
                left++;
                //左侧出窗口后 需要更新表格 更新和插入是对称的
                if(hash_word.count(d)){
                    if(hash_window[d] == hash_word[d]){
                        vaild--;
                    }
                    hash_window[d]--;
                }
            }
        }

        return res;
    }
};



// # !TODO
// LeetCode 567 题  字符串的排列
/*
给你两个字符串 s1 和 s2 ，写一个函数来判断 s2 是否包含 s1 的排列。如果是，返回 true ；否则，返回 false 。
换句话说，s1 的排列之一是 s2 的 子串 。

示例 1：

输入：s1 = "ab" s2 = "eidbaooo"
输出：true
解释：s2 包含 s1 的排列之一 ("ba").
*/

class Solution {
public:
    bool checkInclusion(string s1, string s2) {
        if(s1.size() > s2.size() ){return false;}
        unordered_map<char,int>hash_word;
        unordered_map<char,int>hash_window;
        int window_length = s1.size();

        //建表
        for(char c : s1){
            hash_word[c]++;
        }

        //开始遍历窗口
        int right = 0;
        int left = 0;
        int vaild = 0;
        while(right < s2.size()){
            //向右侧滑动右边界
            char temp = s2[right];
            right++;
            if(hash_word.count(temp)!=0){
                hash_window[temp]++;
                if(hash_window[temp] == hash_word[temp]){
                    vaild++;
                }
            }
            //考虑是否减小窗口
            while(right - left >= window_length){
                if(vaild == hash_word.size()){return true;}
                temp = s2[left];
                left++;
                if(hash_word.count(temp)!=0){
                    if(hash_window[temp] == hash_word[temp]){
                        vaild--;
                    }
                    hash_window[temp]--;
                }

            }
        }
        return false;
    }
};


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


/*
leetcode 30 串联所有单词字串
给定一个字符串 s 和一个字符串数组 words。
 words 中所有字符串 长度相同。

s 中的 串联子串 是指一个包含  words 中所有字符串以任意顺序排列连接起来的子串。

例如，如果 words = ["ab","cd","ef"]， 
那么 "abcdef"， "abefcd"，"cdabef"， "cdefab"，"efabcd"， 和 "efcdab" 都是串联子串。 "acdbef" 不是串联子串，因为他不是任何 words 排列的连接。
返回所有串联子串在 s 中的开始索引。你可以以 任意顺序 返回答案。

输入：s = "barfoothefoobarman", words = ["foo","bar"]
输出：[0,9]
解释：因为 words.length == 2 同时 words[i].length == 3，连接的子字符串的长度必须为 6。
子串 "barfoo" 开始位置是 0。它是 words 中以 ["bar","foo"] 顺序排列的连接。
子串 "foobar" 开始位置是 9。它是 words 中以 ["foo","bar"] 顺序排列的连接。
输出顺序无关紧要。返回 [9,0] 也是可以的。
*/
class Solution {
public:
    vector<int> findSubstring(string s, vector<string>& words) {
        unordered_map<string,int>hash_windows;
        unordered_map<string,int>hash_tmp;
        int one_word_len= words[0].size();
        int words_len = words.size();
        for(int i=0;i<words_len;i++){
            hash_windows[words[i]]++;
        }

        vector<int>res;
        int len = s.size();
        //注意 窗口实际上是存在多个起点的 我们需要枚举每一个起点的情况
        for(int start=0;start<one_word_len;start++){
            int right = start;
            int left  = start;
            int cnts = 0;
            
            while(right < len){
                string tmp = s.substr(right,one_word_len);
                
                //右侧入窗口
                if(hash_windows.find(tmp)!=hash_windows.end()){
                    hash_tmp[tmp]++;
                    if(hash_tmp[tmp] == hash_windows[tmp]){
                        cnts++;
                    }
                    
                }
                
                right = right + one_word_len;
                //左侧收缩
                while(right - left > words_len * one_word_len){
                    string ltmp = s.substr(left,one_word_len);
                    if(hash_windows.find(ltmp) != hash_windows.end() ){
                        if(hash_tmp[ltmp] == hash_windows[ltmp]){
                            cnts--;
                        }
                        hash_tmp[ltmp]--; 
                    }
                    left = left + one_word_len;
                }
                
                if(right - left == words_len * one_word_len && cnts == hash_windows.size()){
                    std::cout <<start <<": "<< tmp << std::endl;
                    res.emplace_back(left);
                }
            }
            hash_tmp.clear();
        }
        
        return res;
    }
};