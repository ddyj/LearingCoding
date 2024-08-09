#ifndef __MEMORYPOOL_H__
#define __MEMORYPOOL_H__


#include <iostream>
#include <memory>
template<size_t obj_size, size_t obj_num>
class MemoryPool {
private:
	//空闲节点
	struct FreeNode {
		FreeNode* next;
		char data[obj_size];
	};
	//内存管理链表结点
	struct MemoryNode {
		MemoryNode* next;
		FreeNode dataArr[obj_num];
	};
	//空闲节点链表
	FreeNode* freeNodeHeader;
	//已近内存管理链表
	MemoryNode* memoryListNode;
	MemoryPool() {
		freeNodeHeader = nullptr;
		memoryListNode = nullptr;
	}
	
	static std::shared_ptr<MemoryPool<obj_size, obj_num>>s_ptr;
public:
	~MemoryPool(){
		MemoryNode* ptr = nullptr;
		while (memoryListNode != nullptr) {
			ptr = memoryListNode->next;
			delete memoryListNode;
			memoryListNode = ptr;
		}
	}

	void* m_malloc() {
		//先看有没有空闲结点
		if (freeNodeHeader == nullptr) {
			//创建一个新的内存结点
			MemoryNode* newBlock = new MemoryNode();
			newBlock->next = nullptr;
			//让当前空闲节点指向新创建空间的第一个元素
			freeNodeHeader = &newBlock->dataArr[0];

			//连接新建立的20块freenode
			for (int i = 1; i < obj_num; ++i) {
				newBlock->dataArr[i - 1].next = &(newBlock->dataArr[i]);
			}
			newBlock->dataArr[obj_num - 1].next = nullptr;

			//第一次分配内存
			if (memoryListNode == nullptr) {
				memoryListNode = newBlock;
			}
			//将其添加到已近分配内存链表中
			else {
				newBlock->next = memoryListNode;
				memoryListNode = newBlock;
			}
		}
		//有空闲结点 就直接使用空闲节点
		void* freenode = static_cast<void*>(freeNodeHeader);
		freeNodeHeader = freeNodeHeader->next;
		return freenode;
	}
	//头插法 将free掉的FreeNode重新插入到头部
	void m_free(void *p) {
		FreeNode* pNode = static_cast<FreeNode*>(p);
		pNode->next = freeNodeHeader;
		freeNodeHeader = pNode;
	}

	static std::shared_ptr<MemoryPool<obj_size, obj_num>>get_instance() {
		s_ptr = std::shared_ptr<MemoryPool<obj_size, obj_num>>(new MemoryPool<obj_size, obj_num>());
		return s_ptr;
	}
};

template<size_t obj_size, size_t obj_num>
std::shared_ptr<MemoryPool<obj_size, obj_num>> MemoryPool<obj_size, obj_num>::s_ptr = nullptr;




#endif