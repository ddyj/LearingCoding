#include "util.h"

//11.盛最多水的容器



/*

给定一个长度为 n 的整数数组 height 。有 n 条垂线，第 i 条线的两个端点是 (i, 0) 和 (i, height[i]) 。
找出其中的两条线，使得它们与 x 轴共同构成的容器可以容纳最多的水。
返回容器可以储存的最大水量。
说明：你不能倾斜容器。

输入：[1,8,6,2,5,4,8,3,7]
输出：49 
解释：图中垂直线代表输入数组 [1,8,6,2,5,4,8,3,7]。在此情况下，容器能够容纳水（表示为蓝色部分）的最大值为 49。
*/


//题目本事使用双指针从vec两侧向心 这也是双指针的一种经典布局
//对于这类问题 分析的时候要考虑什么情况下area会变大
//我们不论left++还是right--都会让(right-left)变小 所以只能让min(left_num,right_num)变大
//也就是我们每次移动指针 只能移动高度小的

class Solution {
public:
    int getArea(int left,int right ,int left_num,int right_num){
        return (right-left) * min(left_num,right_num);
    }
    int maxArea(vector<int>& height) {
        int left = 0;
        int right = height.size()-1;
        int Area = getArea(left,right,height[left],height[right]);
        while(left<right) {
            if(height[left] < height[right]){
                left ++;
                Area = max(Area,getArea(left,right,height[left],height[right]));
            }
            else{
                right--;
                Area = max(Area,getArea(left,right,height[left],height[right]));
            }
        }
        return Area;
    }
};