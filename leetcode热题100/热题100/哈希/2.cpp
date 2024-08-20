#include "util.h"

// 49. 字母异位词分组


/*
给你一个字符串数组，请你将 字母异位词 组合在一起。可以按任意顺序返回结果列表。

字母异位词 是由重新排列源单词的所有字母得到的一个新单词。

 

示例 1:

输入: strs = ["eat", "tea", "tan", "ate", "nat", "bat"]
输出: [["bat"],["nat","tan"],["ate","eat","tea"]]
*/

//哈希
// trick 对于一个string 排序后会按照ascII码大小进行重排 
// 也就是说 如果string是互为字母异位词 的 那么sort后的结果一定是一致的

// 这里要注意 对于哈希表的判断我们是根据判断sort后的str 但是对于res的存储我们是存储的strs的原始str

class Solution {
public:
    vector<vector<string>> groupAnagrams(vector<string>& strs) {
        unordered_map<string,int>hash_table;
        vector<vector<string>>res;
        for(int id = 0 ; id < strs.size() ;id++){
            string str = strs[id];
            sort(str.begin(),str.end());
            if(!hash_table.empty() && hash_table.count(str)!=0){
                res[hash_table[str]].emplace_back(strs[id]);
            }
            else{
                hash_table.insert(make_pair(str,hash_table.size()));
                res.emplace_back(vector<string>(1,strs[id]));
            }
        }
        return res;
    }
};