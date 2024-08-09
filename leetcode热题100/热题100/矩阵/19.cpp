
/*
54. 螺旋矩阵

给你一个 m 行 n 列的矩阵 matrix ，请按照 顺时针螺旋顺序 ，返回矩阵中的所有元素。
输入：matrix = [[1,2,3],[4,5,6],[7,8,9]]
输出：[1,2,3,6,9,8,7,4,5]

*/

#include "util.h"

class Solution {
public:
    vector<int> spiralOrder(vector<vector<int>>& matrix) {
        vector<int>res;

        //定义四组边界 每当跑完一条边 就会缩小对应的边界值
        int upper_used = 0;
        int down_used = matrix.size()-1;
        int left_used = 0;
        int right_used = matrix[0].size()-1;

        //Q:如何判断当前是怎么走的
        //A：可以写一个方向数组 不断循环
        int a[4] = {1,2,3,4}; //1 向右 2 向下 3 向左 4 向上
        int cur_pos = 0 ; //当前走了几步 我们可以对这个值取模来判断当前是什么状态
        while(upper_used <= down_used && left_used <= right_used ){
            int flag = a[cur_pos%4];
            if(flag==1){
                for(int id=left_used;id<=right_used;id++){
                    res.emplace_back(matrix[upper_used][id]);
                }
                upper_used++;
            }
            else if(flag==2){
                for(int id=upper_used;id<=down_used;id++){
                    res.emplace_back(matrix[id][right_used]);
                }
                right_used--;
            }
            else if(flag==3){
                for(int id=right_used;id>=left_used;id--){
                    res.emplace_back(matrix[down_used][id]);
                }
                down_used--;
            }
            else if(flag==4){
                for(int id=down_used;id>=upper_used;id--){
                    res.emplace_back(matrix[id][left_used]);
                }
                left_used++;
            }
            cur_pos++;
        }
        
        return res;
    }
};