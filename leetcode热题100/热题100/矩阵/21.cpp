#include "util.h"

/*
240. 搜索二维矩阵 II
编写一个高效的算法来搜索 m x n 矩阵 matrix 中的一个目标值 target 。该矩阵具有以下特性：
每行的元素从左到右升序排列。
每列的元素从上到下升序排列。

输入：matrix = [[1,4,7,11,15],[2,5,8,12,19],[3,6,9,16,22],[10,13,14,17,24],[18,21,23,26,30]], target = 5
输出：true
*/


//总结 ： 相比于用二分法 本题旨在构造双指针向心移动的场景 
//也即row/col 一个变大/一个变小 可以调控当前值和target的关系


class Solution {
public:
    //二分查找
    bool searchMatrix(vector<vector<int>>& matrix, int target) {
       for(int id=0;id<matrix.size();id++){
           auto res = lower_bound(matrix[id].begin(),matrix[id].end(),target);
           if(res!=matrix[id].end() && *res==target){return true;}
       } 
       return false;
    }
    //Z字查找  从右上角(0,n-1)开始 如果当前值大于target 列-- 小于行++
    //当然 从左下角点也是一样的 重点是找到一左一右两个遍历方向
    //右上角
    bool searchMatrix(vector<vector<int>>& matrix, int target) {
       int row = 0;
       int col = matrix[0].size()-1;
       while(row<matrix.size() && col>=0){
           if(matrix[row][col]>target){col--;}
           else if(matrix[row][col]<target){row++;}
           else{return true;}
       }
       return false;
    }
    //左下角
    bool searchMatrix(vector<vector<int>>& matrix, int target) {
       int row = matrix.size()-1;
       int col = 0;
       while( row>=0 && col<matrix[0].size()){
           if(matrix[row][col]>target){row--;}
           else if(matrix[row][col]<target){col++;}
           else{return true;}
       }
       return false;
    }
};