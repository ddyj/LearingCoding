/*** 
 * @Descripttion: 
 * 单向链表+迭代器
 * @version: 
 * @Author: lhw
 * @Date: 2023-08-08 21:23:48
 * @FilePath: /C++/MyDesign/5_MyIteratorSList.cpp
 * @LastEditors: lhw
 * @LastEditTime: 2023-08-09 16:13:58
 */
#include <iostream>
using namespace std;

// list容器的节点
template<typename T>
struct myNode
{
public:
    using value_type = T;
    value_type val;
    myNode* next;
    myNode():next(nullptr){}
    myNode(const value_type& data):val(data), next(nullptr){}
    myNode(const value_type& data, myNode<T>* _next):val(data), next(_next){}
};

// 节点迭代器
template<typename T>
class IteratorList{
public:
    using value_type = T;
    using link_type = myNode<T>;
    IteratorList(link_type* mPtr=0):node(mPtr){}
    IteratorList& operator++(){
        node = node->next;
        return *this;
    }
    IteratorList operator++(int){
        IteratorList temp = *this;
        node = node->next;
        return temp;
    }
    value_type& operator *(){
        return node->val;
    }
    link_type* operator->(){
        return node;
    }
    IteratorList begin(){
        return IteratorList(node);
    }
    IteratorList end(){
        return IteratorList(nullptr);
    }
    bool operator ==(const IteratorList& x){
        return node == x.node;
    }
    bool operator !=(const IteratorList& x){
        return node != x.node;
    }
private:
    link_type* node;
};

// 单向链表
// 最后节点插入在head节点后面
template<typename T>
class myList{
private:
    using list_node = myNode<T>;
    list_node head;        // 静态类型，头结点
    size_t _size;
public:
    using iterator = IteratorList<T> ;
    using value_type = T;
    
    myList():_size(0){
        head.next=nullptr;
    }
    ~myList(){
        while(head.next!=nullptr){
            list_node* node = head.next;
            head.next = node->next;
            delete node;
        }
        _size=0;
    }
    void push_front(const value_type& value){
        head.next = new list_node(value, head.next);
        ++_size;
    }
    void pop_front(){
        list_node* node = head.next;
        head.next = node->next;
        delete node;
        --_size;
    }
    size_t size() const{
        return _size;
    }
    void show(std::ostream& out = std::cout) const{
        list_node* node = head.next;
        for(;node!=nullptr;node= node->next)
            out << node->val<<" ";
        out << std::endl;
    }
    value_type& front(){        // 第一个元素
        return head.next->val;
    }
    iterator begin(){
        return iterator(head.next);
    }
    iterator end() {
        return iterator(nullptr);
    }

};



// g++ 5_MyIterator.cpp -o 5_MyIterator
int main(){
    myList<int> mlist;
    mlist.push_front(1);
    mlist.push_front(2);
    mlist.push_front(3);
    mlist.push_front(4);
    mlist.show();
    std::cout << mlist.size() << std::endl;
    myList<int>::iterator iter;
    for(iter=mlist.begin();iter != mlist.end(); iter++){
        std::cout << *iter << " ";
    }
    std::cout << std::endl;
}
