#include "util.h"


//参考链接：
//https://leetcode.cn/problems/number-of-islands/solutions/211211/dao-yu-lei-wen-ti-de-tong-yong-jie-fa-dfs-bian-li-


/*
思路 使用dfs遍历 有岛屿的区域 只要访问到陆地 就将其置为为2 表示已经访问过
每进入一次dfs 就表示有一块岛屿
*/


/*
200. 岛屿数量
给你一个由 '1'（陆地）和 '0'（水）组成的的二维网格，请你计算网格中岛屿的数量。
岛屿总是被水包围，并且每座岛屿只能由水平方向和/或竖直方向上相邻的陆地连接形成。
此外，你可以假设该网格的四条边均被水包围。
*/

class Solution {
private:
    int rows;
    int cols;
public:
    void dfs(vector<vector<char>>& grid,int i,int j){
        if(!vaild(i,j)){
            return;
        }
        if(grid[i][j]!='1'){
            return;
        }
        grid[i][j] = '2';

        dfs(grid,i+1,j);
        dfs(grid,i-1,j);
        dfs(grid,i,j+1);
        dfs(grid,i,j-1);
    }
    bool vaild(int i,int j){
        return i>=0 && i<=rows-1 && j>=0 && j<=cols-1;
    }


    int numIslands(vector<vector<char>>& grid) {
        rows = grid.size();
        cols = grid[0].size();

        int ans = 0;
        for(int i=0;i<rows;i++){
            for(int j=0;j<cols;j++){
                if(grid[i][j] == '1'){
                    dfs(grid,i,j);
                    ans++;
                }
            }
        }
        return ans;
    }
};


/*
leetcode 695 岛屿的最大面积
给你一个大小为 m x n 的二进制矩阵 grid 。

岛屿 是由一些相邻的 1 (代表土地) 构成的组合，
这里的「相邻」要求两个 1 必须在 水平或者竖直的四个方向上相邻。
你可以假设 grid 的四个边缘都被 0（代表水）包围着。

岛屿的面积是岛上值为 1 的单元格的数目。

计算并返回 grid 中最大的岛屿面积。如果没有岛屿，则返回面积为 0 。
*/

class Solution {
private:
    int rows;
    int cols;
public:
    bool vaild(int i,int j){
        return i>=0 && i<=rows-1 && j>=0 && j<=cols-1;
    }
    int dfs_area(vector<vector<int>>& grid,int i,int j){
        if(!vaild(i,j)){
            return 0;
        }
        if(grid[i][j]!= 1){
            return 0;
        }
        grid[i][j] = 2;
        return 1 + 
            + dfs_area(grid,i+1,j) 
            + dfs_area(grid,i-1,j) 
            + dfs_area(grid,i,j+1)
            + dfs_area(grid,i,j-1);
    }
    int maxAreaOfIsland(vector<vector<int>>& grid) {
        rows = grid.size();
        cols = grid[0].size();
        int max_arae = 0;
        for(int i=0;i<rows;i++){
            for(int j=0;j<cols;j++){
                if(grid[i][j]==1){
                    int area = dfs_area(grid,i,j);
                    max_arae = max(area,max_arae);
                }
            }
        }
        return max_arae;
    }
};

/*
leetcode 463 岛屿周长
给定一个 row x col 的二维网格地图 grid ，
其中：grid[i][j] = 1 表示陆地， grid[i][j] = 0 表示水域。

网格中的格子 水平和垂直 方向相连（对角线方向不相连）。
整个网格被水完全包围，但其中恰好有一个岛屿（或者说，一个或多个表示陆地的格子相连组成的岛屿）。

岛屿中没有“湖”（“湖” 指水域在岛屿内部且不和岛屿周围的水相连）。
格子是边长为 1 的正方形。网格为长方形，且宽度和高度均不超过 100 。
计算这个岛屿的周长。

*/

/*
在difs的角度上讲 如果此时遍历出边界 即!vaild(i,j)  那说明岛屿一定会和边界有一条边
对于 grid[i][j]!= 1  分成一下两个情况
    1. grid[i][j] == 0 那么岛屿一定经过一条和海洋响邻的边
    2. grid[i][j] == 2 那么岛屿周长并不影响
*/
class Solution {
private:
    int rows;
    int cols;
public:
    bool vaild(int i,int j){
        return i>=0 && i<=rows-1 && j>=0 && j<=cols-1;
    }

    int dfs(vector<vector<int>>& grid,int i,int j){
        if(!vaild(i,j)){
            return 1;
        }
        if(grid[i][j] == 2){return 0;}
        if(grid[i][j] == 0){return 1;}

        grid[i][j]=2;
        return 0
            + dfs(grid,i+1,j)
            + dfs(grid,i-1,j)
            + dfs(grid,i,j+1)
            + dfs(grid,i,j-1);
    }



    int islandPerimeter(vector<vector<int>>& grid) {
        rows = grid.size();
        cols = grid[0].size();
        int Perimeter = 0;
        for(int i=0;i<rows;i++){
            for(int j=0;j<cols;j++){
                if(grid[i][j] == 1){
                    Perimeter =max(dfs(grid,i,j),Perimeter) ;
                }
            }
        }
        return Perimeter;
    }
};

/*
leetcode 872 最大人工岛
给你一个大小为 n x n 二进制矩阵 grid 。
最多 只能将一格 0 变成 1 。
返回执行此操作后，grid 中最大的岛屿面积是多少？

岛屿 由一组上、下、左、右四个方向相连的 1 形成。
*/


class Solution {
private:
    int rows;
    int cols;
    unordered_map<int,int>key_area;
public:

    bool is_vaild(int i,int j){
        return i>=0 && i<=rows-1 && j>=0 && j<=cols-1;
    }

    int dfs_area(vector<vector<int>>& grid,int i,int j,int cur_key){
        if(!is_vaild(i,j)){
            return 0;
        }
        if(grid[i][j]!=1){
            return 0;
        }
        grid[i][j] = cur_key;

        return 1 
            + dfs_area(grid,i+1,j,cur_key)
            + dfs_area(grid,i-1,j,cur_key)
            + dfs_area(grid,i,j+1,cur_key)
            + dfs_area(grid,i,j-1,cur_key);
    }

    int dfs_addLand(vector<vector<int>>& grid,int i,int j){
        grid[i][j] = -1;
        
        int add_area = 1;
        //需要判断当前海洋的格子周边不是一个个岛屿
        int top_key = 0;
        int left_key = 0;
        int right_key = 0;
        int down_key = 0;

        // down
        if(is_vaild(i+1,j) && grid[i+1][j] >0 ){
            down_key = grid[i+1][j];
            if(down_key!=right_key && down_key!=left_key&& down_key!=top_key){
                add_area += key_area[down_key]; 
            } 
        }
        //top
        if(is_vaild(i-1,j) && grid[i-1][j] >0 ){
            top_key = grid[i-1][j];
            if(top_key!=right_key && top_key!=left_key&& top_key!=down_key){
                add_area += key_area[top_key];
            } 
        }
        //right
        if(is_vaild(i,j+1) && grid[i][j+1] >0 ){
            right_key = grid[i][j+1];
            if(right_key!=down_key && right_key!=left_key&& right_key!=top_key){
                add_area += key_area[right_key];
            } 
        }
        //left
        if(is_vaild(i,j-1) &&  grid[i][j-1] >0 ){
            left_key = grid[i][j-1];
            if(left_key!=right_key && left_key!=down_key&& left_key!=top_key){
                add_area += key_area[left_key];
            } 
        }
        
        return add_area;

    }


    int largestIsland(vector<vector<int>>& grid) {
        rows = grid.size();
        cols = grid[0].size();
        int res = 0;
        //先dfs遍历一遍陆地 计算每一个岛屿的面积 对每一个岛屿编号 构建key和面积的哈希关系
        int cur_key = 2;
        for(int i=0;i<rows;i++){
            for(int j=0;j<cols;j++){
                if(grid[i][j]==1){
                    int area = dfs_area(grid,i,j,cur_key);
                    key_area[cur_key] = area;
                    cur_key++; 
                    res = max(area,res);
                }
            }
        }   
        
        //再使用dfs遍历一次海洋 判断海洋模块哪一个添加为陆地后就可以获取最大面积
        for(int i=0;i<rows;i++){
            for(int j=0;j<cols;j++){
                if(grid[i][j] == 0){
                    int max_area = dfs_addLand(grid,i,j);
                    res = max(res,max_area);
                }
            }
        }
        return res;

    }
};


/*
leetcode 130 被围绕的区域

给你一个 m x n 的矩阵 board ，
由若干字符 'X' 和 'O' ，
找到所有被 'X' 围绕的区域，
并将这些区域里所有的 'O' 用 'X' 填充。
 

*/

class Solution {
private:
    int rows;
    int cols;
   
public:
    bool is_vaild(int i,int j){
        return  i>=0 && i<=rows-1 && j>=0 && j<=cols-1;
    }
    
    void dfs(vector<vector<char>>& board,int i,int j){
        if(!is_vaild(i,j)){
            return;
        }
        if(board[i][j]!='O'){
            return;
        }
        board[i][j] = '.';
   
        dfs(board,i+1,j);
        dfs(board,i-1,j);
        dfs(board,i,j+1);
        dfs(board,i,j-1);
    }

    //思路 从四周边界开始找岛屿 将找到的岛屿标记为'.' 剩下的'O'就是我们要修改的内容
    void solve(vector<vector<char>>& board) {
        rows = board.size();
        cols = board[0].size();
       

        for(int i=0;i<rows;i++){
            if(board[i][0]=='O' ){
                dfs(board,i,0);
            } 
            if(board[i][cols-1]=='O' ){
                dfs(board,i,cols-1);
            } 
        }
        for(int j=0;j<cols;j++){
            if(board[0][j]=='O' ){
                dfs(board,0,j);
            } 
            if(board[rows-1][j]=='O' ){
                dfs(board,rows-1,j);
            } 
        }


        
        for(int i=0;i<rows;i++){
            for(int j=0;j<cols;j++){
                if(board[i][j]=='O'){
                    board[i][j] = 'X';
                }
                else if(board[i][j]=='.'){
                    board[i][j] = 'O';
                }
            }
        }    
    }
};