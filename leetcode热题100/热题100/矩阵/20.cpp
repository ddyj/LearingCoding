
#include "util.h"
/*
48. 旋转图像
给定一个 n × n 的二维矩阵 matrix 表示一个图像。请你将图像顺时针旋转 90 度。
你必须在 原地 旋转图像，这意味着你需要直接修改输入的二维矩阵。请不要 使用另一个矩阵来旋转图像

示例 1：
输入：matrix = [[1,2,3],[4,5,6],[7,8,9]]
输出：[[7,4,1],[8,5,2],[9,6,3]]
*/

/*
 1 2 3      1 4 7       7 4 1
 4 5 6  ->  2 5 8  ->   8 5 2
 7 8 9      3 6 9       9 6 3
    根据主对角线换    替换左右两列（第i列和第n-i-1列替换）

5 1 9 11           5  2    13  15
2 4 8 10      ->   1   4    3  14  
13 3 6 7           9   8    6  12
15 14 12 16        11  10   7  16

*/



class Solution {
public:
    void rotate(vector<vector<int>>& matrix) {
        for(int row=0;row<matrix.size();row++){
            for(int col=0;col<=row;col++){
                int temp = matrix[row][col];
                matrix[row][col] = matrix[col][row];
                matrix[col][row] = temp;
            }
        }
        int left = 0;
        int right = matrix[0].size()-1;
        while(left<right){
            for(int row=0;row<matrix.size();row++){
                int temp = matrix[row][left];
                matrix[row][left] = matrix[row][right];
                matrix[row][right] = temp;
            }
            left++;
            right--;
        }
    }
};