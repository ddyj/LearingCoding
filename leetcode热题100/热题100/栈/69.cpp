#include "util.h"

/*
20. 有效的括号
给定一个只包括 '('，')'，'{'，'}'，'['，']' 的字符串 s ，判断字符串是否有效。

有效字符串需满足：

左括号必须用相同类型的右括号闭合。
左括号必须以正确的顺序闭合。
每个右括号都有一个对应的相同类型的左括号。

输入：s = "()"
输出：true
*/

class Solution {
public:
    bool isValid(string s) {
        stack<char>table;
        for(int id=0;id<s.size();id++){
            if(s[id] == '}' || s[id] == ']' || s[id]==')'){
                if(!table.empty() && table.top() == '{'){
                    if(s[id] == '}'){table.pop();}
                    else{return false;}
                }
                else if(!table.empty() && table.top() == '['){
                    if(s[id] == ']'){table.pop();}
                    else{return false;}
                }
                else if(!table.empty() && table.top() == '('){
                    if(s[id] == ')'){table.pop();}
                    else{return false;}
                }
                else{
                    return false;
                }
            }
            else{
                table.push(s[id]);
            }
        }
        return table.empty();
    }
};