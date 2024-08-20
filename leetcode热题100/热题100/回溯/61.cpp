
#include "util.h"

/*
131. 分割回文串
给你一个字符串 s，请你将 s 分割成一些子串，使每个子串都是 回文串 。返回 s 所有可能的分割方案。
回文串 是正着读和反着读都一样的字符串。
输入：s = "aab"
输出：[["a","a","b"],["aa","b"]]
*/

class Solution {
public:
    bool is_palindrome(const string &str){
        string re_str = str;
        reverse(re_str.begin(),re_str.end());
        return re_str == str;
    }
    void dfs(string s,vector<vector<string>>&res,vector<string>path,int start_id,int cut_id){
        //终止条件  由于判断了每一个子字符串是否回文 所以当能执行到cut_id == s.size() path一定是满足题设的
        if(cut_id == s.size()){
            if(cut_id==start_id){
                res.emplace_back(path);
                return;
            }
            else{
                string sub = s.substr(start_id,cut_id-start_id);

                if(!is_palindrome(sub)){return;}
                else{
                    path.emplace_back(sub);
                    res.emplace_back(path);
                    return;
                }   
            }
        }


        //在当前id下不切
        dfs(s,res,path,start_id,cut_id+1);
        //在当前id下切一刀 如果分割的字符是回文的 就加入path 如果不是就直接return
        string sub = s.substr(start_id,cut_id-start_id);
        if(!is_palindrome(sub)){return;}
        else{
            path.emplace_back(sub);
            dfs(s,res,path,cut_id,cut_id+1);
        }
    }


    vector<vector<string>> partition(string s) {
        vector<vector<string>>res;
        vector<string>path;  //存储当前分割字符串的结果
        int start_id = 0;  //没有被切割的字符的起点
        int cut_id =1; //当前切割点的位置
        dfs(s,res,path,start_id,cut_id);

        return res;
    }
};