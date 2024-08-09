#include "util.h"


/*


5. 最长回文子串
给你一个字符串 s，找到 s 中最长的回文子串。

如果字符串的反序与原始字符串相同，则该字符串称为回文字符串

输入：s = "babad"
输出："bab"
解释："aba" 同样是符合题意的答案。
*/


class Solution {
public:
    string longestPalindrome(string s) {
        int len = s.length();
        if(len<2){return s;}
        vector<vector<bool>>dp(len,vector<bool>(len,false));
        for(int id=0;id<len;id++){
            dp[id][id] = true;
        }
        int left = 0;
        int max_len = 0;
        /*
            Q 为什么i是从最右下角遍历的
            A 如果从最左侧遍历 对于aaaa的场景 左上角会会因为dp[1][2]没有初值 导致变为false出错（将下面输出代码恢复可以看到 所有结果的右上角都是0）
                              如果从有右下角 开始此时dp[1][2]先赋值 在传递到dp[0][3]
            总结 我们dp在遍历的时候 一定要分析dp传播的方向 要从已知递推未知
        */
        for(int i=len-1;i>=0;i--){
            for(int j=i;j<len;j++){
                if(s[i]==s[j]){
                    if(j-i <= 2){
                        dp[i][j] = true;
                    }
                    else{
                        dp[i][j] = dp[i+1][j-1];
                    }
                }
                if(dp[i][j] && j-i+1>max_len){
                    max_len = j-i+1;
                    left = i;
                }   
            }
        }

        // for(int i=0;i<len;i++){
        //     for(int j=0;j<len;j++){
        //         std::cout<<dp[i][j];
        //     }
        //     std::cout<<std::endl;
        // }
        // std::cout<<left << " "<<max_len << std::endl;

        return s.substr(left,max_len);
    }
};