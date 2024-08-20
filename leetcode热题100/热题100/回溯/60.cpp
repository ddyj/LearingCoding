#include "util.h"

/*
79. 单词搜索
给定一个 m x n 二维字符网格 board 和一个字符串单词 word 。
如果 word 存在于网格中，返回 true ；否则，返回 false 。

单词必须按照字母顺序，通过相邻的单元格内的字母构成，
其中“相邻”单元格是那些水平相邻或垂直相邻的单元格。同一个单元格内的字母不允许被重复使用。

输入：board = [["A","B","C","E"],["S","F","C","S"],["A","D","E","E"]], word = "ABCCED"
输出：true
*/



class Solution {
private:
    int cols = 0;
    int rows = 0;
public:

    void dfs(int row,int col,vector<vector<char>>& board,string word,int cur_id,int &res){
        //剪枝的核心 沿着字符顺序剪
        if(board[row][col]!=word[cur_id]){return;}
        if(cur_id==word.size()-1){res++;return;}
        if(res!=0){return;}
        //在选择路径后 需要标记已经选过的词
        board[row][col] = '/0';


        if(row+1<rows){dfs(row+1,col,board,word,cur_id+1,res);}
        if(row-1>=0){dfs(row-1,col,board,word,cur_id+1,res);}
        if(col-1>=0){dfs(row,col-1,board,word,cur_id+1,res);}
        if(col+1<cols){dfs(row,col+1,board,word,cur_id+1,res);}
        board[row][col] = word[cur_id];
    }


    //dfs 遍历网格
    /*
    在检索的时候 我们可以从每一个字符出发 查找是向word的下一个字符查找
    */
    bool exist(vector<vector<char>>& board, string word) {
        cols = board[0].size();
        rows = board.size();
        int res = 0;
        for(int r_id=0;r_id<rows;r_id++){
            for(int c_id=0;c_id<cols;c_id++){
                dfs(r_id,c_id,board,word,0,res);
            }
        }
        return res;
    }
};