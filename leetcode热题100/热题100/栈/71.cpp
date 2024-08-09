#include "util.h"

/*
394 字符串解码
给定一个经过编码的字符串，返回它解码后的字符串。

编码规则为: k[encoded_string]，表示其中方括号内部的 encoded_string 正好重复 k 次。注意 k 保证为正整数。

你可以认为输入字符串总是有效的；输入字符串中没有额外的空格，且输入的方括号总是符合格式要求的。

此外，你可以认为原始数据不包含数字，所有的数字只表示重复的次数 k ，例如不会出现像 3a 或 2[4] 的输入。
输入：s = "2[abc]3[cd]ef"
输出："abcabccdcdcdef"
*/

/*
本题逻辑上复杂 把代码能完整的顺下来
*/

class Solution {
public:
    string decodeString(string s) {
        string res;
        stack<int>counts;
        stack<string>strs;
        int num = 0;
        int len = s.size();
        for(int id=0;id<len;id++){
            if(s[id]>='0' && s[id]<='9'){
                num = num*10 + (s[id] - '0');
            }
            else if( (s[id]>='a' && s[id]<='z') || (s[id]>='A' && s[id]<='Z') ){
                res +=s[id];
            }
            else if(s[id] == '['){
                //遇到前括号 认为出现新的[]区域 记录之前记录的string和count
                counts.push(num);
                strs.push(res);
                num = 0;
                res = "";
            }
            //当遇到] 开始出栈一次 计算一次[]区域的解码结果
            else if(s[id] ==']'){
                int count = counts.top();
                counts.pop();
                while(count>0){
                    strs.top() +=res;
                    count--;
                }
                res = strs.top();
                strs.pop();
            }
        }

        return res;
    }
};