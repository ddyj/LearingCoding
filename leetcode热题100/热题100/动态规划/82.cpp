#include "util.h"

/*
118 杨辉三角
给定一个非负整数 numRows，生成「杨辉三角」的前 numRows 行
输入: numRows = 5
输出: [[1],[1,1],[1,2,1],[1,3,3,1],[1,4,6,4,1]]
*/

class Solution {
public:
    vector<vector<int>> generate(int numRows) {
        vector<vector<int>>res(numRows);
        res[0] = {1};
        if(numRows ==2){
            res[1] = {1,1};
            return res;
        }
        if(numRows >= 3){
            res[1] = {1,1};
            for(int id=2;id<numRows;id++){
                res[id].resize(id+1);
                res[id][0] = 1;
                res[id][id] = 1;
                for(int col = 1;col<id;col++){
                    res[id][col] = res[id-1][col] + res[id-1][col-1];
                }
            }
        }
        return res;
    }
};