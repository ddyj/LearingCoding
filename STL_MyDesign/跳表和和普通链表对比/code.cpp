#include <iostream>
#include <cstdlib>
#include <ctime>
#include <climits>


//单链表的实现
class Node {
public:
    int value;
    Node* next;
    Node(int val) : value(val), next(nullptr) {}
};

class LinkedList {
public:
    LinkedList() : head(nullptr) {}

    void insert(int value) {
        Node* newNode = new Node(value);
        newNode->next = head;
        head = newNode;
    }

    Node* search(int value) {
        Node* current = head;
        while (current != nullptr) {
            if (current->value == value) {
                return current;
            }
            current = current->next;
        }
        return nullptr;
    }

private:
    Node* head;
};

class SkipListNode {
public:
    int value;
    //指向不同层次的下一个节点的指针
    SkipListNode** forward;
    SkipListNode(int level, int value) : value(value) {
        forward = new SkipListNode* [level + 1];
        std::fill(forward, forward + level + 1, nullptr);
    }
    ~SkipListNode() {
        delete[] forward;
    }
};

class SkipList {
public:
    SkipList(int maxLevel) : maxLevel(maxLevel), level(0) {
        head = new SkipListNode(maxLevel, INT_MIN);
    }

    ~SkipList() {
        delete head;
    }

    void insert(int value) {
        SkipListNode* current = head;
        SkipListNode** update = new SkipListNode* [maxLevel + 1];
        std::fill(update, update + maxLevel + 1, nullptr);

        for (int i = level; i >= 0; --i) {
            while (current->forward[i] != nullptr && current->forward[i]->value < value) {
                current = current->forward[i];
            }
            //此时有两种情况：1. current->forward[i] == nullptr 此时当前节点就是插入节点
            // 2. current->forward[i]->value >= value 
            //     而current->value<value的 那插入位置就是current的后面一个节点

            // 这里update记录了不同层间的插入节点的上一位
            update[i] = current;
        }
        // 移动到当前层的下一个节点，以检查是否需要插入新节点。
        current = current->forward[0];
        // 如过是nullptr 或者 值不等于value（实际上是>=) 如果等于就不插入了
        if (current == nullptr || current->value != value) {
            // 生成一个随机层数
            int randLevel = randomLevel();
            // 如果新节点的层数大于当前跳表的层数，则需要更新跳表的层数。
            if (randLevel > level) {
                for (int i = level + 1; i <= randLevel; ++i) {
                    update[i] = head;
                }
                level = randLevel;
            }
            // 要插入节点
            SkipListNode* newNode = new SkipListNode(randLevel, value);
            for (int i = 0; i <= randLevel; ++i) {
                newNode->forward[i] = update[i]->forward[i];
                update[i]->forward[i] = newNode;
            }
        }
        delete[] update;
    }

    bool search(int value) {
        SkipListNode* current = head;
        for (int i = level; i >= 0; --i) {
            while (current->forward[i] != nullptr && current->forward[i]->value < value) {
                current = current->forward[i];
            }
            if (current->forward[i]!=nullptr && current->forward[i]->value == value) {
                return true;
            }
        }

        current = current->forward[0];
        return current != nullptr && current->value == value;
    }

private:
    int randomLevel() {
        int level = 0;
        while (std::rand() % 2 && level < maxLevel) {
            ++level;
        }
        return level;
    }

    int maxLevel;
    int level;
    SkipListNode* head;
};

void jumpListvsList() {
    LinkedList list;
    for (int i = 0; i < 1000000; ++i) {
        list.insert(i);
    }

    clock_t start = clock();
    list.search(999999);
    for (int i = 999999; i >= 900000; --i) {
        list.search(i);
    }
    clock_t end = clock();

    std::cout << "LinkedList search time: " << (static_cast<double>(end) - static_cast<double>(start)) / (CLOCKS_PER_SEC/1000) << " millseconds\n";


    SkipList skipList(16);
    for (int i = 0; i < 1000000; ++i) {
        skipList.insert(i);
    }

    clock_t s_start = clock();
    for (int i = 999999; i >= 900000; --i) {
        skipList.search(i);
    }
    clock_t s_end = clock();

    std::cout << "SkipList search time: " << (static_cast<double>(s_end) - static_cast<double>(s_start)) / (CLOCKS_PER_SEC / 1000) << " millseconds\n";

    /*
        LinkedList search time: 33624 millseconds
        SkipList search time: 35 millseconds
    */
}