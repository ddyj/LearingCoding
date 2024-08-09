#include "util.h"


class Solution {
public:
    /*
        查询问题 ： 
            对于当前柱子i 只有他右边第一个大于他的柱子会和他产生空隙来接雨水
            所以问题变为 查询右边第一个大于当前元素的值 -- 单调栈
    
    */
    int trap(vector<int>& height) {
        stack<int>sin_table;
        int len = height.size();
        int walls = 0;
        for(int i=0;i<len;i++){
            while(!sin_table.empty() && height[i] > height[sin_table.top()] ){
                int cur = sin_table.top();
                sin_table.pop();

                if(sin_table.empty()){break;}
                int pre = sin_table.top();

                int width = i - pre - 1;
                int high = min(height[pre],height[i]) -  height[cur];
                walls +=width * high;
            }
            sin_table.push(i);
        }
        return walls;
    }
};