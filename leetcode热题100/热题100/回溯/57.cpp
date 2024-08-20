

#include "util.h"


/*
17. 电话号码的字母组合
给定一个仅包含数字 2-9 的字符串，返回所有它能表示的字母组合。答案可以按 任意顺序 返回。
给出数字到字母的映射如下（与电话按键相同）。注意 1 不对应任何字母

输入：digits = "23"
输出：["ad","ae","af","bd","be","bf","cd","ce","cf"]
*/

/*
由于输出结果是按照按键顺序执行的，所以回溯可以按照存储的string顺序遍历每一个char
*/

class Solution {
private:
    string getString(char num){
        switch(num){
            case '2' :
                return "abc";
                break;
            case '3' :
                return "def";
                break;
            case '4' :
                return "ghi";
                break;
            case '5' :
                return "jkl";
                break;
            case '6' :
                return "mno";
                break;
            case '7' :
                return "pqrs";
                break;
            case '8' :
                return "tuv";
                break;
            case '9' :
                return "wxyz";
                break;
        }
        return "";
    }
public:
    void dfs(vector<string>strlist,vector<string>&res,string path,int cur_id){
        if(cur_id == strlist.size()){res.emplace_back(path);return;}
        for(int id=0;id<strlist[cur_id].size();id++){
            path = path + strlist[cur_id][id];
            dfs(strlist,res,path,cur_id+1);
            path = path.substr(0,path.size()-1);
        }
    }
    vector<string> letterCombinations(string digits) {
        
        vector<string>strlist(digits.size());
        vector<string>res;
        if(digits.size()==0){return res;}
        string path;
        int count = 0;
        for(char num:digits){
            strlist[count] = getString(num);
            count++;
        }
        dfs(strlist,res,path,0);
        return res;
    }
};