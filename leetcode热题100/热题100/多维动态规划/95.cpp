
#include "util.h"
/*
72 编辑距离
给你两个单词 word1 和 word2， 请返回将 word1 转换成 word2 所使用的最少操作数  。

你可以对一个单词进行如下三种操作：

插入一个字符
删除一个字符
替换一个字符
输入：word1 = "horse", word2 = "ros"
输出：3
解释：
horse -> rorse (将 'h' 替换为 'r')
rorse -> rose (删除 'r')
rose -> ros (删除 'e')

*/


/*  ！！！
对“dp[i-1][j-1] 表示替换操作，dp[i-1][j] 表示删除操作，dp[i][j-1] 表示插入操作。”的补充理解：

以 word1 为 "horse"，word2 为 "ros"，且 dp[5][3] 为例，即要将 word1的前 5 个字符转换为 word2的前 3 个字符，也就是将 horse 转换为 ros，因此有：

(1) dp[i-1][j-1]，即先将 word1 的前 4 个字符 hors 转换为 word2 的前 2 个字符 ro，然后将第五个字符 word1[4]（因为下标基数以 0 开始） 由 e 替换为 s（即替换为 word2 的第三个字符，word2[2]）

(2) dp[i][j-1]，即先将 word1 的前 5 个字符 horse 转换为 word2 的前 2 个字符 ro，然后在末尾补充一个 s，即插入操作

(3) dp[i-1][j]，即先将 word1 的前 4 个字符 hors 转换为 word2 的前 3 个字符 ros，然后删除 word1 的第 5 个字符

 ---来自 评论区

*/


class Solution {
public:
    int minDistance(string word1, string word2) {
        int len1 = word1.length();
        int len2 = word2.length();
        //dp[i][j]  将word1中前i个字符转成word2中前j个字符的最小操作
        vector<vector<int>>dp(len1+1,vector<int>(len2+1,0));
        //边界 
        for(int id=0;id<=len2;id++){
            dp[0][id] = id;
        }
        for(int id=0;id<=len1;id++){
            dp[id][0] = id;
        }
        //递推 dp[i-1][j-1] 表示替换操作，dp[i-1][j] 表示删除操作，dp[i][j-1] 表示插入操作。
        /*  
                      min(dp[i-1][j]+1,dp[i][j-1]+1,dp[i-1][j-1]+1 )   word1[i] != word2[j]
           dp[i][j]=   
                      dp[i-1][j-1]          word1[i] == word2[j]
        */
        for(int i=1;i<=len1;i++){
            for(int j=1;j<=len2;j++){
                if(word1[i-1]==word2[j-1]){
                    dp[i][j]=dp[i-1][j-1];
                }
                else{
                    dp[i][j] = min(min(dp[i-1][j],dp[i][j-1]),dp[i-1][j-1]) +1;
                }
            }
        }

        return dp[len1][len2];
    }
};