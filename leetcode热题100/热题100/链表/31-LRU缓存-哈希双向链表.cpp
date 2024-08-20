#include "util.h"


//参考文档：https://leetcode.cn/problems/lru-cache/solutions/12711/lru-ce-lue-xiang-jie-he-shi-xian-by-labuladong

class LRUCache {
    //哈希链表
private:
    //对于双向链表 尾部节点表示使用频率最高的val 头部节点表示使用频率最低的node
    class doubleNode{
        public:
            int _key;
            int _val;
            doubleNode* _pre;
            doubleNode* _next;
            doubleNode():_key(0),_val(0){
                _pre = nullptr;
                _next = nullptr;
            }
            doubleNode(int key,int val):_key(key),_val(val){
                _pre = nullptr;
                _next = nullptr;
            }

            void changeVal(int val){
                _val = val;
            }
    }; 
    class doubleList{
        public:
            doubleNode* head = new doubleNode();
            doubleNode* tail = new doubleNode();
            int _size;
            doubleList(){
                head->_next = tail;
                tail->_pre = head;
                _size = 0;
            }
            inline int getsize(){return _size;} 
            void addNodeInBack(doubleNode * addNode){
                tail->_pre->_next = addNode;
                addNode->_pre = tail->_pre;
                addNode->_next = tail;
                tail->_pre = addNode;
                _size++;
            }
            int deleteNodeInfront(){
                if(_size == 0){return -1;}
                int delete_key = head->_next->_key;
                if(head->_next->_next!=nullptr)
                    head->_next->_next->_pre = head;
                else
                    head->_next->_pre = head;
                head->_next = head->_next->_next;
                _size--;
                return delete_key;
            }
            void changedNode2Back(doubleNode* node1){
                node1->_pre->_next = node1->_next;
                node1->_next->_pre = node1->_pre;

                tail->_pre->_next = node1;
                node1->_pre = tail->_pre;
                node1->_next = tail;
                tail->_pre = node1; 
            }
            int getBackNodeVal(){
                return tail->_pre->_val;
            }
    };

    doubleList dList;
    unordered_map<int,doubleNode*>hash_table;
    int _capacity;
public:
    LRUCache(int capacity) {
        _capacity = capacity;
    }
    
    int get(int key) {
        if(dList.getsize()==0){
            return -1;
        }
        if(hash_table.find(key) == hash_table.end()){
            return -1;
        }

        dList.changedNode2Back(hash_table[key]);
        return dList.getBackNodeVal();
    }
    
    void put(int key, int value) {
        if(hash_table.find(key) == hash_table.end()){
            if(dList.getsize() < _capacity){
                doubleNode* addNode = new doubleNode(key,value);
                dList.addNodeInBack(addNode);
                hash_table[key] = addNode;
            }
            else{
                int d_key = dList.deleteNodeInfront();
                if(d_key < 0){return;}
                hash_table.erase(d_key);

                doubleNode* addNode = new doubleNode(key,value);
                dList.addNodeInBack(addNode);
                hash_table[key] = addNode;
            }
        }
        else{
            hash_table[key]->changeVal(value);
            dList.changedNode2Back(hash_table[key]);
        }
    }
};

/**
 * Your LRUCache object will be instantiated and called as such:
 * LRUCache* obj = new LRUCache(capacity);
 * int param_1 = obj->get(key);
 * obj->put(key,value);
 */



class LRUCache {
private:
    struct doubleNode{
        int key;
        int data;
        doubleNode* pre;
        doubleNode* next;
        doubleNode(int _key =-1,int _data=-1):
            key(_key),data(_data),pre(nullptr),next(nullptr){};
    };
    struct doubleList{
        doubleNode* head = new doubleNode();
        doubleNode* tail = new doubleNode();
        int cur_nums;
        doubleList(){
            head->next = tail;
            tail->pre = head;
            cur_nums = 0;
        };
        //向末尾插入元素
        doubleNode* AddNodeToEnd(int& key,int& val){
            doubleNode* add_node = new doubleNode(key,val);
            tail->pre->next = add_node;
            add_node->pre = tail->pre;
            add_node->next = tail;
            tail->pre = add_node;
            ++cur_nums;
            return add_node;
        }
        //修改某个位置的元素并将其移动到队尾
        void ChangeValAndMove(doubleNode* ChangeNode,int& val){
            ChangeNode->data = val;
            ChangeNode->pre->next = ChangeNode->next;
            ChangeNode->next->pre = ChangeNode->pre;

            tail->pre->next = ChangeNode;
            ChangeNode->pre = tail->pre;
            ChangeNode->next = tail;
            tail->pre = ChangeNode;
        }
        //删除队头的结点
        int deleteNode(){
            if(cur_nums == 0){return -1;}
            doubleNode* old_node = head->next;
            int delete_key = old_node->key;
            if(delete_key == -1){return delete_key;}
            old_node->next->pre = head;
            head->next = old_node->next;
            delete old_node;
            --cur_nums;
            return delete_key;
        }
        //访问最新结点，并将访问后的结点放置到队尾
        int GetValAndMove(doubleNode* ChangeNode){
            int val = ChangeNode->data;
            ChangeValAndMove(ChangeNode,val);
            return val;
        }
        inline int GetSize(){return cur_nums;}
    };
    std::unordered_map<int,doubleNode*>hash_node;
    doubleList lru_list;
    int _capacity;
public:
    LRUCache(int capacity) {
        _capacity = capacity;
    }
    int get(int key) {
        if(lru_list.GetSize() == 0){return -1;}
        if(hash_node.find(key) == hash_node.end()){return -1;}
        return lru_list.GetValAndMove(hash_node[key]);
    }
    void put(int key, int value) {
        if(hash_node.find(key) != hash_node.end()){
            lru_list.ChangeValAndMove(hash_node[key],value);
        }
        else{
            while(lru_list.GetSize() >= _capacity){
                int delete_key = lru_list.deleteNode();
                if(delete_key < 0){return;}
                hash_node.erase(delete_key);
            }
            hash_node[key] = lru_list.AddNodeToEnd(key,value);
        }
    }
};