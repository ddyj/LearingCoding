#include "util.h"


/*
62 不同路径
    一个机器人位于一个 m x n 网格的左上角 （起始点在下图中标记为 “Start” ）。
机器人每次只能向下或者向右移动一步。机器人试图达到网格的右下角（在下图中标记为 “Finish” ）。
问总共有多少条不同的路径？

输入：m = 3, n = 2
输出：3
解释：
从左上角开始，总共有 3 条路径可以到达右下角。
1. 向右 -> 向下 -> 向下
2. 向下 -> 向下 -> 向右
3. 向下 -> 向右 -> 向下

*/

class Solution {
public:
    int uniquePaths(int m, int n) {
        //dp[i][j] 当走到i,j的时候  走的路径的总数
        vector<vector<int>>dp(m,vector<int>(n,0));
        //边界 dp[0][0] = 1  dp[0][] = 1  dp[][0] = 1
        for(int i=0;i<m;i++){
            dp[i][0] = 1;
        }
        for(int j=0;j<n;j++){
            dp[0][j] = 1;
        }
        dp[0][0] = 1;
        //dp[i][j] = dp[i-1][j]  + dp[i][j-1]     
        for(int i=1;i<m;i++){
            for(int j=1;j<n;j++){
                    dp[i][j] = dp[i-1][j]  + dp[i][j-1];
            }
        }
        return dp[m-1][n-1];
    }
};



/*
64 最小路径和
给定一个包含非负整数的 m x n 网格 grid ，请找出一条从左上角到右下角的路径，使得路径上的数字总和为最小。

说明：每次只能向下或者向右移动一步。
*/
class Solution {
public:
    int minPathSum(vector<vector<int>>& grid) {
        int rows = grid.size();
        int cols = grid[0].size();
        //dp[i][j] 走到ij路径最小和
        vector<vector<int>>dp(rows,vector<int>(cols,0));   
        dp[0][0] = grid[0][0];
        for(int i=1;i<rows;i++){
            dp[i][0] = dp[i-1][0] + grid[i][0]; 
        }
        for(int j=1;j<cols;j++){
            dp[0][j] = dp[0][j-1]+grid[0][j]; 
        }
        //递推关系
        for(int i=1;i<rows;i++){
            for(int j=1;j<cols;j++){
                dp[i][j] = min(dp[i-1][j],dp[i][j-1]) + grid[i][j]; 
            }
        }
        return dp[rows-1][cols-1];
    }
};