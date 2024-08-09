#include "util.h"


/*
给定一个 m x n 的矩阵，如果一个元素为 0 ，则将其所在行和列的所有元素都设为 0 。请使用 原地 算法。


输入：matrix = [[1,1,1],[1,0,1],[1,1,1]]
输出：[[1,0,1],[0,0,0],[1,0,1]]
*/



class Solution {
public:
    void setZeroes(vector<vector<int>>& matrix) {
        unordered_set<int>rows;
        unordered_set<int>cols;

        //初始化哈希表
        for(int row = 0;row <matrix.size();row++){
            for(int col = 0;col<matrix[0].size();col++){
                if(matrix[row][col]==0){
                    rows.insert(row);
                    cols.insert(col);
                }
            }
        }

        //更新行
        for(unordered_set<int>::iterator it = rows.begin();it!=rows.end() ;it++){
            vector<int>(matrix[*it].size(),0).swap(matrix[*it]);
        }
        //更新列
        for(unordered_set<int>::iterator it = cols.begin();it!=cols.end() ;it++){
            for(int id=0;id<matrix.size();id++){
                matrix[id][*it] = 0;
            }
        }

    }
}