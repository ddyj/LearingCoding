#include"util.h"


/*
最长有效括号
给你一个只包含 '(' 和 ')' 的字符串，找出最长有效（格式正确且连续）括号子串的长度。
输入：s = "(()"
输出：2
解释：最长有效括号子串是 "()"
*/

class Solution {
public:
    int longestValidParentheses(string s) {
        /*
            用栈模拟一遍，将所有无法匹配的括号的位置全部置1
            例如: "()(()"的mark为[0, 0, 1, 0, 0]
            再例如: ")()((())"的mark为[1, 0, 0, 1, 0, 0, 0, 0]
            经过这样的处理后, 此题就变成了寻找最长的连续的0的长度
        */
        if(s.size()==0){return 0;}
        vector<int>dp(s.size(),0);
        stack<pair<char,int>>table;
        for(int i=0;i<s.size();i++){
            if(s[i] == '('){
                table.push(make_pair(s[i],i));
                dp[i] = 1;
            }
            else{
                if(!table.empty() && table.top().first == '('){
                    dp[table.top().second] = 0;
                    table.pop();
                }
                else{
                    table.push(make_pair(s[i],i));
                    dp[i] = 1;
                }
            }
        }
        // while(!table.empty()){
        //     dp[table.top().second] = 1;
        //     table.pop();
        // }
        // for(int a:dp){
        //     std::cout<<a<<" ";
        // }
        std::cout<<std::endl;
        //寻找最长0的区域
        int max_len = 0;
        int cur_len = 0;
        for(int i=0;i<s.size();i++){
            if(dp[i]==0){cur_len++;max_len = max(max_len,cur_len);}
            else{max_len = max(max_len,cur_len);cur_len=0;}
        }
        return max_len;
    }
};