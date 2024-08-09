/*

给你一个字符串数组 words ，找出并返回 length(words[i]) * length(words[j]) 的最大值，并且这两个单词不含有公共字母。
如果不存在这样的两个单词，返回 0 。
输入：words = ["abcw","baz","foo","bar","xtfn","abcdef"]
输出：16 
解释：这两个单词为 "abcw", "xtfn"。
*/


#include "util.h"

class Solution {
private:
    unordered_map<int,int>hash_table;
public:
    //Q：如何计算两个单词长度一致
    //A：使用位运算 int长度是2^31 有31个位符号判断 这里借鉴 我们对a-z按位排 a-2^0 z-2^26
    //   如果两个数字取交为0 那么两个数字一定不相交
    //   这里使用对二进制数1左移的方式快速计算进位
    int maxProduct(vector<string>& words) {
        int len = words.size();
        for(int i=0;i<len;i++){
            int t=0;
            for(char c:words[i]){
                int u = c-'a';
                t |= 1<<u;
            }
            hash_table[i] = t;
        }
        int res = 0;
        for(int i=0;i<len-1;i++){
            for(int j=i+1;j<len;j++){
                if((hash_table[i] & hash_table[j])==0){
                    res = max(res,int(words[i].length()*words[j].length()));
                }
            }
        }
        return res;
    }
};