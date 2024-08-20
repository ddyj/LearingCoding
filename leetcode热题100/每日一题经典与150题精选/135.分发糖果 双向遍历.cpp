#include "util.h"


/*
n 个孩子站成一排。给你一个整数数组 ratings 表示每个孩子的评分。

你需要按照以下要求，给这些孩子分发糖果：

每个孩子至少分配到 1 个糖果。
相邻两个孩子评分更高的孩子会获得更多的糖果。
请你给每个孩子分发糖果，计算并返回需要准备的 最少糖果数目 。


输入：ratings = [1,0,2]
输出：5
解释：你可以分别给第一个、第二个、第三个孩子分发 2、1、2 颗糖果。
*/

/*
模拟 
https://leetcode.cn/problems/candy/solutions/17847/candy-cong-zuo-zhi-you-cong-you-zhi-zuo-qu-zui-da-/

*/

class Solution {
public:
    //两次遍历 先给每一个孩子一颗糖
    //左侧规则  如果r[i] > r[i-1]  r[i-1]  +1 
    //右侧规则  如果r[i] > r[i+1]  r[i+1] +1
    // 先左后右 然后取最大值
    int candy(vector<int>& ratings) {
        int len = ratings.size();
        vector<int>givesLeft(len,1);
        vector<int>givesRight(len,1);


        for(int i=1;i<len;i++){
            if(ratings[i] > ratings[i-1]){
                givesLeft[i] = givesLeft[i-1] + 1;
            }
        }
        int res = 0;
        for(int i=len-2;i>=0;i--){
            
            if(ratings[i] > ratings[i+1]){
                givesRight[i] = givesRight[i+1] + 1;
            }
            res += max(givesLeft[i],givesRight[i]);
        }
        return res + max(givesLeft[len-1],givesRight[len-1]);
    }
};