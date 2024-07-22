//
// Created by dai on 2024/7/8.
//


//dyj  双向链表
//
// Created by dai on 2024/7/8.
//

#include <iostream>

template<typename T>
class myNode{
public:
    using value_type = T;
    value_type _data;
    myNode* next;
    myNode* prev;
    myNode():next(nullptr),prev(nullptr) {};
    myNode(const value_type& val):_data(val),next(nullptr),prev(nullptr){};
    myNode(const value_type& val,myNode* next,myNode* pre):_data(val),next(next),prev(pre) {};

    bool operator==(const myNode& lhs){
        return this == &lhs;
    }
    bool operator!=(const myNode& lhs){
        return !(*this == &lhs);
    }
    friend std::ostream& operator<<(std::ostream& os, const myNode<T>& node);
};
template<typename T>
std::ostream& operator<<(std::ostream& os,const myNode<T>& node){
    os << node._data;
    return os;
}
template<typename T>
class iteratorList{
public:
    using value_type = T;
    using link_type = myNode<value_type>;
private:
    link_type* node;
public:
    iteratorList(link_type* ptr = nullptr) : node(ptr){};
    //++iterator
    iteratorList& operator++(){
        node = node->next;
        return *this;
    }
    //iterator++
    iteratorList& operator++(int){
        iteratorList temp = *this;
        node=node->next;
        return temp;
    }
    //--iterator
    iteratorList& operator--(){
        node = node->prev;
        return *this;
    }
    iteratorList&operator--(int){
        iteratorList temp = *this;
        node = node->prev;
        return temp;
    }
    value_type&operator*(){
        return node->_data;
    }
    link_type* operator->(){
        return node;
    }
    iteratorList begin(){
        return iteratorList(node);
    }
    iteratorList end(){
        return iteratorList(nullptr);
    }
    bool operator==(const iteratorList&x){
        return node == x.node;
    }
    bool operator!=(const iteratorList&x){
        return node!=x.node;
    }
};


//双项链表
template <typename T>
class mylist{
private:
    using list_node = myNode<T>;
    list_node head;
    list_node tail;
    size_t _size;
public:
     using iterator = iteratorList<T>;
     using value_type = T;

     mylist():_size(0){
         head.next = &tail;
         tail.prev = &head;
     }
     ~mylist(){
         while(head.next!=&tail){
             list_node* node = head.next;
             head.next = node->next;
             delete node;
             node = nullptr;
             --_size;
         }
         _size = 0;
     }
     void push_front(const value_type& val){
         list_node *add_node = new list_node(val);
         add_node->next = head.next;
         head.next->prev = add_node;
         head.next = add_node;
         add_node->prev = &head;
         ++_size;
     }
     void push_back(const value_type& val){
         list_node *add_node = new list_node(val);
         add_node->prev = tail.prev;
         add_node->next = &tail;
         tail.prev->next = add_node;
         tail.prev = add_node;
         ++_size;
     }
     void pop_back(){
         if(_size == 0){return ;}
         list_node *node = tail.prev;
         tail.prev = node->prev;
         node->prev->next = &tail;
         delete node;
         node = nullptr;
         --_size;
     }
     void pop_front(){
        if(_size == 0){return ;}
        list_node *node = head.next;
        head.next = node->next;
        node->next->prev = &head;
        delete node;
        node = nullptr;
        --_size;
     }
     size_t size()const{
         return _size;
     }
     void show()const{
         list_node *node = head.next;
         for(;node!= &tail;node=node->next){
             std::cout << node->_data<<" ";
         }
         std::cout << std::endl;
     }
     value_type &front(){
         return head.next->val;
     }
     value_type &back(){
         return tail.prev->val;
     }
    iterator begin(){
        return iterator(head.next);
    }
    iterator end(){
        return iterator(&tail);
    }
};

void test_myIterator(){
    mylist<int>mlist;
    for(int i=0;i<10;++i){
        if(i%2 == 0){
            mlist.push_back(i);
        }
        else{
            mlist.push_front(i);
        }
    }
    mlist.show();
    mylist<int>::iterator iter;
    for(iter=mlist.begin();iter!=mlist.end();++iter){
        std::cout << *iter << " ";
    }
    std::cout << std::endl;
    for(int i=0;i<5;++i){
        if(i%2 == 0){
            mlist.pop_back();
        }
        else{
            mlist.pop_front();
        }
    }
    std::cout <<"after pop ... " <<std::endl;
    mlist.show();
    for(iter=mlist.begin();iter!=mlist.end();++iter){
        std::cout << *iter << " ";
    }
    std::cout << std::endl;
}

int main(){
    test_myIterator();
    return 0;
}