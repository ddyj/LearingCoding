#include  "util.h"


/*
你这个学期必须选修 numCourses 门课程，
记为 0 到 numCourses - 1 。

在选修某些课程之前需要一些先修课程。 
先修课程按数组 prerequisites 给出，其中 prerequisites[i] = [ai, bi] ，表示如果要学习课程 ai 则 必须 先学习课程  bi 。

例如，先修课程对 [0, 1] 表示：
想要学习课程 0 ，你需要先完成课程 1 。
请你判断是否可能完成所有课程的学习？
如果可以，返回 true ；否则，返回 false 。

 

示例 1：

输入：numCourses = 2, prerequisites = [[1,0]]
输出：true
解释：总共有 2 门课程。学习课程 1 之前，你需要完成课程 0 。这是可能的。
示例 2：

输入：numCourses = 2, prerequisites = [[1,0],[0,1]]
输出：false
解释：总共有 2 门课程。学习课程 1 之前，你需要先完成​课程 0 ；并且学习课程 0 之前，你还应先完成课程 1 。这是不可能的。
 

*/


class Solution {
public:
    //拓扑排序应用
    bool canFinish(int numCourses, vector<vector<int>>& prerequisites) {
        //计算每一个顶点的入度
        vector<int>inDegree(numCourses,0);
        //统计当前课的后续课程 当切断弧的时候 需要根据这个表来更新入度
        unordered_map<int,vector<int>>hash_table;
        for(auto c : prerequisites){
            inDegree[c[0]]++;
            hash_table[c[1]].push_back(c[0]);
        }
        //BFS 每次对入度为0的结点入队
        queue<int>bfsque;
        for(int i=0;i<numCourses;i++){
            if(inDegree[i] == 0){
                bfsque.push(i);
            }
        }

        int count = 0;
        while(!bfsque.empty()){
            int select_id = bfsque.front();
            bfsque.pop();
            count++;
            //更新与当前结点有关的点的入度
            int len = hash_table[select_id].size();
            for(int i=0;i<len;i++){
                if(inDegree[hash_table[select_id][i]] > 0){
                    inDegree[hash_table[select_id][i]] --;
                    if(inDegree[hash_table[select_id][i]] == 0){
                        bfsque.push(hash_table[select_id][i]);
                    }
                }
            }
        }
        return count == numCourses;
    }
};


/*
现在你总共有 numCourses 门课需要选，记为 0 到 numCourses - 1。
给你一个数组 prerequisites ，其中 prerequisites[i] = [ai, bi] ，
表示在选修课程 ai 前 必须 先选修 bi 。

例如，想要学习课程 0 ，你需要先完成课程 1 ，我们用一个匹配来表示：[0,1] 。
返回你为了学完所有课程所安排的学习顺序。可能会有多个正确的顺序，
你只要返回 任意一种 就可以了。如果不可能完成所有课程，返回 一个空数组 。

输入：numCourses = 2, prerequisites = [[1,0]]
输出：[0,1]
解释：总共有 2 门课程。要学习课程 1，你需要先完成课程 0。因此，正确的课程顺序为 [0,1] 。
*/
class Solution {
public:
    vector<int> findOrder(int numCourses, vector<vector<int>>& prerequisites) {
        vector<int>res;
        vector<int>indgress(numCourses,0);
        unordered_map<int,vector<int>>next;
        for(int i=0;i<prerequisites.size();i++){
            indgress[prerequisites[i][0]]++;
            next[prerequisites[i][1]].emplace_back(prerequisites[i][0]);
        }
        queue<int>bfsque;
        for(int i=0;i<numCourses;i++){
            if(indgress[i]==0){
                bfsque.push(i);
            }
        }
        int count = 0;
        while(!bfsque.empty()){
            int id = bfsque.front();
            bfsque.pop();
            count++;
            res.emplace_back(id);
            int len = next[id].size();
            for(int i=0;i<len;i++){
                if(indgress[next[id][i]] > 0){
                    indgress[next[id][i]]--;
                    if(indgress[next[id][i]] == 0){
                        bfsque.push(next[id][i]);
                    }
                }
            }
        }
        if(count != numCourses){
            res.clear();
        }
        return res;
    }
};