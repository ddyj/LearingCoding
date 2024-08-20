
#include "util.h"


/*
在给定的 m x n 网格 grid 中，每个单元格可以有以下三个值之一：

值 0 代表空单元格；
值 1 代表新鲜橘子；
值 2 代表腐烂的橘子。
每分钟，腐烂的橘子 周围 4 个方向上相邻 的新鲜橘子都会腐烂。

返回 直到单元格中没有新鲜橘子为止所必须经过的最小分钟数。如果不可能，返回 -1 。

*/
/*
    BFS 借助queue来实现
*/

class Solution {

private:
    int rows;
    int cols;
public:
    bool is_vaild(int i,int j){
        return i>=0 && i<=rows-1 && j>=0 && j<=cols-1;
    }


    //BFS遍历
    int orangesRotting(vector<vector<int>>& grid) {
        rows = grid.size();
        cols = grid[0].size();
        queue<pair<int,int>>bfsque;
        //计算新鲜橙子的个数
        int Fresh_oraneges = 0;
        //记录哪一个顶点是访问过的 防止回环问题
        vector<vector<int>>grid_id(rows,vector<int>(cols,0));
        for(int i=0;i<rows;i++){
            for(int j=0;j<cols;j++){
                if(grid[i][j] == 1){
                    Fresh_oraneges++;
                }
                else if(grid[i][j] == 2){
                    bfsque.push(make_pair(i,j));
                }
            }
        }
        if(Fresh_oraneges == 0){
            return 0;
        }

        //BFS遍历 通过队列的方式处理
        int res = 0;
        while(Fresh_oraneges>0 && !bfsque.empty()){
            int n = bfsque.size();
            for(int i=0;i<n;i++){
                auto ids = bfsque.front();      
                bfsque.pop();
         
                if(!is_vaild(ids.first,ids.second)){
                    continue;
                }
                if(grid_id[ids.first][ids.second]!=0){
                    continue;
                }
                if(grid[ids.first][ids.second] == 0){
                    continue;
                }
                if(grid[ids.first][ids.second] == 1){
                    grid[ids.first][ids.second] = 2;
                    Fresh_oraneges--;
                    if(Fresh_oraneges == 0){
                        return res;
                    }
                }
                grid_id[ids.first][ids.second] = 1;
                bfsque.push(make_pair(ids.first+1,ids.second));
                bfsque.push(make_pair(ids.first-1,ids.second)); 
                bfsque.push(make_pair(ids.first,ids.second+1));          
                bfsque.push(make_pair(ids.first,ids.second-1));
            }
            res++;
        }
        return -1;
    }
};