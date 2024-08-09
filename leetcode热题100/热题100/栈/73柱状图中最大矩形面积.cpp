#include "util.h"


/*
给定 n 个非负整数，用来表示柱状图中各个柱子的高度。每个柱子彼此相邻，且宽度为 1 。

求在该柱状图中，能够勾勒出来的矩形的最大面积。

输入：heights = [2,1,5,6,2,3]
输出：10
解释：最大的矩形为图中红色区域，面积为 10


//类比接雨水
*/


/*
    对于某一跟柱子 左侧比他的大的柱子和右侧比他大的柱子都不会影响由这条柱子为高组成的矩形
    所以问题转换为 对于一个heights[i] 找到他左侧、右侧第一个小于他的值

*/


//单调递减的栈 top的下一个元素是左侧第一个大于他的元素
//单调地增的栈 top的下一个元素是左侧第一个小于他的元素


class Solution {
public:
    int largestRectangleArea(vector<int>& heights) {
        int maxarea = 0;
        stack<int>table;
        //这里额外增加左右侧，因为和接雨水不同，这个题目中如果top元素下一位没有元素 -- 在两侧，任然会记录其矩形面积，所以对两侧需要加上约束
        //反例 1 2 3 4 5 -- 0
        heights.emplace_back(0);
        //反例 0 -- 9 8 7 6 5 如果前面不加0 9在出栈后就没有栈顶元素了 计算w时会报错
        heights.insert(heights.begin(),0);

        int len = heights.size();
        for(int i=0;i<len;i++){
            while(!table.empty() && heights[table.top()] > heights[i]){
                int h = heights[table.top()];
                table.pop();
                int w = i - table.top() - 1;
                maxarea = max(maxarea,w*h);        
            }
            table.push(i);
        }
        return maxarea;
    }
};


/*
T42 接雨水
给定 n 个非负整数表示每个宽度为 1 的柱子的高度图，计算按此排列的柱子，下雨之后能接多少雨水。

输入：height = [0,1,0,2,1,0,1,3,2,1,2,1]
输出：6
解释：上面是由数组 [0,1,0,2,1,0,1,3,2,1,2,1] 表示的高度图，在这种情况下，可以接 6 个单位的雨水（蓝色部分表示雨水）。 
*/

/*
对于承接雨水的柱子i 左侧比他小的柱子和右侧比他小的柱子都不会影响他接雨水 
问题转换为找到柱子i 左右两侧的比第一个大于他的值

*/


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