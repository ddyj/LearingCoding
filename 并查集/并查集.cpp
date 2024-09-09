//并查集  -- 一个元素要么是根结点 要么是子叶 层高是2
#include<unordered_map>
using std::unordered_map;
using std::cerr;
using std::cout;
using std::endl;
class unionSet {
private:
	unordered_map<int, int>union_parent_hash_table;
	int find(int x) {
		//未查询到 直接接入当根结点
		if (union_parent_hash_table.find(x) == union_parent_hash_table.end()) {
			union_parent_hash_table[x] = x;
		}
		//还有上级 而且自己不是根节点 就合并父节点到根节点上 -- 合并
		if (union_parent_hash_table[x] != x) {
			union_parent_hash_table[x] = find(union_parent_hash_table[x]);
		}
		return union_parent_hash_table[x];
	}
public:
	void insert(int x, int y) {
		int rootX = find(x);
		int rootY = find(y);
		//如果两者相同 表示属于同一个根节点
		//否则 就让parent[y] = x 这是输入设置的所属关系
		if (rootX != rootY) {
			union_parent_hash_table[rootY] = rootX;
		}
	}
	//查询
	bool select(int x,int y) {
		int rootX = find(x);
		int rootY = find(y);
		//如果两者相同 表示属于同一个根节点
		return rootX == rootY;
	}
};


void test_union_Set() {
	/*
				1                   12
            2       3             13  
 		4      5   6  7         15   16
	  8 9 10            11
	*/
	unionSet uSet;
	uSet.insert(1, 2);
	uSet.insert(1, 3);
	uSet.insert(2, 4);
	uSet.insert(2, 5);
	uSet.insert(3, 6);
	uSet.insert(3, 7);
	uSet.insert(4, 8);
	uSet.insert(4, 9);
	uSet.insert(4, 10);
	uSet.insert(7, 11);
	uSet.insert(12, 13);
	uSet.insert(13, 15);
	uSet.insert(13, 16);


	cout << uSet.select(1, 7) << endl;  //true
	cout << uSet.select(1, 13) << endl;  //false
	cout << uSet.select(10, 7) << endl;  //true
	cout << uSet.select(4, 11) << endl;  //true
	cout << uSet.select(4, 15) << endl;  //false
	cout << uSet.select(5, 12) << endl;  //false


}