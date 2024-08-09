#include "util.h"
/*
1143 最长公共子序列

给定两个字符串 text1 和 text2，返回这两个字符串的最长 公共子序列 的长度。如果不存在 公共子序列 ，返回 0 。

一个字符串的 子序列 是指这样一个新的字符串：它是由原字符串在不改变字符的相对顺序的情况下删除某些字符（也可以不删除任何字符）后组成的新字符串。

例如，"ace" 是 "abcde" 的子序列，但 "aec" 不是 "abcde" 的子序列。
两个字符串的 公共子序列 是这两个字符串所共同拥有的子序列。


输入：text1 = "abcde", text2 = "ace" 
输出：3  
解释：最长公共子序列是 "ace" ，它的长度为 3 。
*/


class Solution {
private:
    unordered_map<char,int>hash_str1;
    unordered_map<char,int>hash_str2;
public:
    int longestCommonSubsequence(string text1, string text2) {
        int len1 = text1.length();
        int len2 = text2.length();
        for(int id=0;id<len1;id++){hash_str1[text1[id]] = id;}
        for(int id=0;id<len2;id++){hash_str2[text2[id]] = id;}
        //dp[i][j]  str1在0-i的区域内 和 str2在0-j的区域的公共子序列长度
        vector<vector<int>>dp(len1,vector<int>(len2,0)); 
        //边界条件  dp[0][] =  str1[0]是否在0-j的区域内存在？  dp[][0]同理
        dp[0][0] = text1[0] == text2[0] ? 1 : 0;
        for(int id=1;id<len2;id++){
            dp[0][id] = text1[0] == text2[id] ? 1 : dp[0][id-1];
        }
        for(int id=1;id<len1;id++){
            dp[id][0] = text1[id] == text2[0] ? 1 : dp[id-1][0];
        }
        //递推 dp[i][j] = dp[i-1][j-1] + 1(str1[i] == str2[j])
        //     dp[i][j] = max(dp[i-1][j],dp[i][j-1])   
        for(int id=1;id<len1;id++){
            for(int jd=1;jd<len2;jd++){
                if(text1[id] == text2[jd]){
                    dp[id][jd] = dp[id-1][jd-1] + 1;
                }
                else{
                    dp[id][jd] = max(dp[id-1][jd],dp[id][jd-1]);
                }
            }
        }
        
        return dp[len1-1][len2-1];
    }
};