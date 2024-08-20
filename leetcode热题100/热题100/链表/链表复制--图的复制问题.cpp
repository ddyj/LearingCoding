#include "util.h"

/*
T-138 随机链表复制
给你一个长度为 n 的链表，每个节点包含一个额外增加的随机指针 random ，
该指针可以指向链表中的任何节点或空节点。

构造这个链表的 深拷贝。 深拷贝应该正好由 n 个 全新 节点组成，
其中每个新节点的值都设为其对应的原节点的值。
新节点的 next 指针和 random 指针也都应指向复制链表中的新节点，
并使原链表和复制链表中的这些指针能够表示相同的链表状态。
复制链表中的指针都不应指向原链表中的节点 。

例如，如果原链表中有 X 和 Y 两个节点，其中 X.random --> Y 。
那么在复制链表中对应的两个节点 x 和 y ，同样有 x.random --> y 。

返回复制链表的头节点。

用一个由 n 个节点组成的链表来表示输入/输出中的链表。
每个节点用一个 [val, random_index] 表示：

val：一个表示 Node.val 的整数。
random_index：随机指针指向的节点索引（范围从 0 到 n-1）；如果不指向任何节点，则为  null 。
你的代码 只 接受原链表的头节点 head 作为传入参数。

 


*/





/*
// Definition for a Node.
class Node {
public:
    int val;
    Node* next;
    Node* random;
    
    Node(int _val) {
        val = _val;
        next = NULL;
        random = NULL;
    }
};
*/

class Solution {
public:
    Node* copyRandomList(Node* head) {
        Node* res = new Node(-1);
        Node* phead = res;
        unordered_map<Node*,Node*>hash_table;
        Node* cur_node = head;
        while(cur_node!=nullptr){
            phead->next = new Node(cur_node->val);
            hash_table[cur_node] = phead->next;
            phead = phead->next;
            cur_node = cur_node->next;
        }
        cur_node = head;
        phead = res->next;

        while(cur_node!=nullptr){
            if(hash_table[cur_node->random]!=nullptr){
                phead->random = hash_table[cur_node->random];
            }
            cur_node = cur_node->next;
            phead = phead->next;
        }
        return res->next;

    }
};


/*

leetcode 133 克隆图
给你无向 连通 图中一个节点的引用，请你返回该图的 深拷贝（克隆）。

图中的每个节点都包含它的值 val（int） 和其邻居的列表（list[Node]）。

class Node {
    public int val;
    public List<Node> neighbors;
}
 

测试用例格式：

简单起见，每个节点的值都和它的索引相同。
例如，第一个节点值为 1（val = 1），第二个节点值为 2（val = 2），
以此类推。该图在测试用例中使用邻接列表表示。

邻接列表 是用于表示有限图的无序列表的集合。每个列表都描述了图中节点的邻居集。

给定节点将始终是图中的第一个节点（值为 1）。你必须将 给定节点的拷贝 作为对克隆图的引用返回
*/

/*
// Definition for a Node.
class Node {
public:
    int val;
    vector<Node*> neighbors;
    Node() {
        val = 0;
        neighbors = vector<Node*>();
    }
    Node(int _val) {
        val = _val;
        neighbors = vector<Node*>();
    }
    Node(int _val, vector<Node*> _neighbors) {
        val = _val;
        neighbors = _neighbors;
    }
};
*/

class Solution {
private:
    unordered_map<Node*,Node*>hash_table;  //记录遍历过后的结点
public:
    void dfs(Node* node){
        if(node==nullptr){return;}
        if(hash_table.find(node)!=hash_table.end()){return;}
        hash_table[node] = new Node(node->val);
        for(int i=0;i<node->neighbors.size();i++){
            dfs(node->neighbors[i]);
        }
    }


    Node* cloneGraph(Node* node) {
        
        Node* phead = node;
        dfs(phead);

        for(auto &[ori,copy]:hash_table){
            for(int i=0;i<ori->neighbors.size();i++){
                copy->neighbors.emplace_back(hash_table[ori->neighbors[i]]);
            }
        }

        return hash_table[node];
    }
};