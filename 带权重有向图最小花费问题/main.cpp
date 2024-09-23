#include<climits>
struct Edge {
	int from, to, cost;
};
/*
	迪杰斯特拉算法 不支持负权重 因为其本身是从起点出发逐次延伸步长查询当前步长下最小的距离
	例如一步可达情况下 0-1的cost是10 这里可以直接认为0-1的最小距离是10  但是如果有负权重，就并不适用了

	时间复杂度为O(ElogV),因为是按照结点构成的路径执行的 所以是logV
*/
void dijkstra(int start, int n, const vector<Edge>& edges) {
	vector<int>dist(n, INT_MAX);
	dist[start] = 0;


	//记录当前结点出发时此时的最小cost
	priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
	pq.push({ 0,start });
	while (!pq.empty()) {
		auto[d, u] = pq.top();
		pq.pop();
		if (d > dist[u]) continue;
		for (const Edge& edge : edges) {
			if (edge.from != u) { continue; }
			int v = edge.to;
			int cost = edge.cost;
			if (dist[u] + cost < dist[v]) {
				dist[v] = dist[u] + cost;
				pq.push({ dist[u] + cost,v });
			}
		}
	}

	// 输出从 start 到各节点的最短路径
	for (int i = 0; i < n; ++i) {
		if (dist[i] == INT_MAX)
			cout << "Node " << i << ": unreachable\n";
		else
			cout << "Node " << i << ": " << dist[i] << "\n";
	}
}
/*
	贝尔曼福特解法 对每一个边进行松弛操作
	判断d[i] + weight[i][j] < d[j]
	则更新d[j] 为d[i] + weight[i][j]
	直到遍历的过程中不存在更新的操作
	
	如果存在负环 无解 所以要判断
	因为每一轮都是要遍历所有边的时间复杂度为0(EV) 
*/
bool bellmanFord(int start, int n, const vector<Edge>& edges) {
	vector<int> dist(n, INT_MAX);
	dist[start] = 0;

	// 进行 n-1 次松弛操作
	for (int i = 0; i < n - 1; ++i) {
		for (const Edge& edge : edges) {
			if (dist[edge.from] != INT_MAX && dist[edge.from] + edge.cost < dist[edge.to]) {
				dist[edge.to] = dist[edge.from] + edge.cost;
			}
		}
	}

	// 检测负环，如果还能进行松弛，说明存在负环
	for (const Edge& edge : edges) {
		if (dist[edge.from] != INT_MAX && dist[edge.from] + edge.cost < dist[edge.to]) {
			cout << "Graph contains a negative-weight cycle.\n";
			return false;
		}
	}

	// 输出最短路径
	for (int i = 0; i < n; ++i) {
		if (dist[i] == INT_MAX)
			cout << "Node " << i << ": unreachable\n";
		else
			cout << "Node " << i << ": " << dist[i] << "\n";
	}

	return true;
}
/*
	弗洛伊德算法 计算矩阵图中任意两个点之间的最短距离
	弗洛伊德算法的核心思想是将i-j 和i-k-j之间的距离进行比较
*/

void floyd(int n,const vector<Edge>& edges) {
	// 创建一个距离矩阵，初始化为输入的邻接矩阵
	vector<vector<int>> dist(n,vector<int>(n,INT_MAX));
	for (const Edge& edge : edges) {
		dist[edge.from][edge.to] = edge.cost;
	}
	// 核心算法，三层循环遍历所有节点，更新最短路径
	for (int k = 0; k < n; ++k) {
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < n; ++j) {
				// 如果通过节点 k 可以缩短从 i 到 j 的路径，更新 dist[i][j]
				if (dist[i][k] != INT_MAX && dist[k][j] != INT_MAX && dist[i][k] + dist[k][j] < dist[i][j]) {
					dist[i][j] = dist[i][k] + dist[k][j];
				}
			}
		}
	}
	// 输出最短路径
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			if (dist[i][j] == INT_MAX)
				cout << "INF" << "\t";  // 无路径情况
			else
				cout << dist[i][j] << "\t";
		}
		cout << endl;
	}
}