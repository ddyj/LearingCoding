#include "MemoryPool.h"
class myData {
private:
	int a;
	static int count;
public:
	myData() : a(count) {
		++count;
		print();
	};
	void print() {
		std::cout << this << ": ";
		std::cout << "the " << a << "th object" << std::endl;
	}
};
int myData::count = 0;

template<typename T, size_t PoolSize>
class AbstractDataPool {
private:
	std::shared_ptr<MemoryPool<sizeof(T), PoolSize>> memory_pool;
	queue<T*>_Data;
public:
	AbstractDataPool(){
		memory_pool = MemoryPool<sizeof(T), PoolSize>::get_instance();
	};
	
	void createData(int num) {
		for (int i = 0; i < num; ++i) {
			void* mem = memory_pool->m_malloc();
			T* p1 = new(mem) T();
			_Data.push(p1);
		}
	}

	void deleteData(int num) {
		for (int i = 0; i < num; ++i) {
			auto it = _Data.front();
			_Data.pop();
			memory_pool->m_free(it);
		}
	}
};

void test_memory_pool() {
	AbstractDataPool<myData,5> demo;
	demo.createData(5);
	demo.deleteData(5);
	demo.createData(5);
}


int main() {
	test_memory_pool();
	return 0;
}


/*
output:
000001E3014E6228: the 0th object
000001E3014E6238: the 1th object
000001E3014E6248: the 2th object
000001E3014E6258: the 3th object
000001E3014E6268: the 4th object

000001E3014E6268: the 5th object
000001E3014E6258: the 6th object
000001E3014E6248: the 7th object
000001E3014E6238: the 8th object
000001E3014E6228: the 9th object
*/