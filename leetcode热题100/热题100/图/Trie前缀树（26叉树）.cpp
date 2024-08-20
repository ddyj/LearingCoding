#include "util.h"


/*
Trie（发音类似 "try"）或者说 前缀树 是一种树形数据结构，
用于高效地存储和检索字符串数据集中的键。这一数据结构有相当多的应用情景，
例如自动补完和拼写检查。

请你实现 Trie 类：

Trie() 初始化前缀树对象。
void insert(String word) 向前缀树中插入字符串 word 。
boolean search(String word) 如果字符串 word 在前缀树中，
返回 true（即，在检索之前已经插入）；否则，返回 false 。
boolean startsWith(String prefix) 如果之前已经插入的字符串 word 的前缀之一为 prefix ，
返回 true ；否则，返回 false 。

*/


/*
参考连接
https://leetcode.cn/problems/implement-trie-prefix-tree/solutions/98390/trie-tree-de-shi-xian-gua-he-chu-xue-zhe-by-huwt
*/
class Trie {
private:
    bool is_end;
    Trie* next[26];
public:
    Trie() {
        is_end = false;
        for(int i=0;i<26;i++){
            next[i] = nullptr;
        }
    }

 
    void insert(string word) {
        Trie* node = this;
        int len = word.length();
        for(int i=0;i<len;i++){
            if(node->next[word[i]-'a']!=nullptr){
                node = node->next[word[i]-'a'];
            }
            else{
                node->next[word[i]-'a'] = new Trie();
                node = node->next[word[i]-'a'];
            }
        }
        node->is_end = true;
    }
    
    bool search(string word) {
        Trie* node = this;
        int len = word.length();
        for(int i =0;i<len;i++){
            if(node->next[word[i]-'a']!=nullptr){
                node = node->next[word[i]-'a'];
            }
            else{
                return false;
            }
        }
        return node->is_end;
    }
    
    bool startsWith(string prefix) {
        Trie* node = this;
        int len = prefix.length();
        for(int i =0;i<len;i++){
            if(node->next[prefix[i]-'a']!=nullptr){
                node = node->next[prefix[i]-'a'];
            }
            else{
                return false;
            }
        }
        return true;
    }
};

/**
 * Your Trie object will be instantiated and called as such:
 * Trie* obj = new Trie();
 * obj->insert(word);
 * bool param_2 = obj->search(word);
 * bool param_3 = obj->startsWith(prefix);
 */