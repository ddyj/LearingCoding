#include "util.h"

/*
56. 合并区间

以数组 intervals 表示若干个区间的集合，其中单个区间为 intervals[i] = [starti, endi] 。
请你合并所有重叠的区间，并返回 一个不重叠的区间数组，该数组需恰好覆盖输入中的所有区间 。

 

示例 1：

输入：intervals = [[1,3],[2,6],[8,10],[15,18]]
输出：[[1,6],[8,10],[15,18]]
解释：区间 [1,3] 和 [2,6] 重叠, 将它们合并为 [1,6].
*/



//排序加双指针
class Solution {
public:
    vector<vector<int>> merge(vector<vector<int>>& intervals) {
        vector<vector<int>>res;
        if(intervals.size()==1){
            return intervals;
        }
        //排列原则 比较第一个值 由小到大 如果第一个一样 比较第二个值
        sort(intervals.begin(),intervals.end());
        int left_num = intervals[0][0];
        int right_num = intervals[0][1];
        //什么样的区间是可以合并的 因为已经排序过 所以不存在b_left > a_left
        /*
        * a_left    b_left <= a_right b_right
        * a_left    b_left  b_right   <= a_right 
        * b_left    a_left  a_right   b_right (舍)
        */
        for(int id=1;id<intervals.size();id++){
            //可以合并的必要条件
            if(intervals[id][0] >= left_num && intervals[id][0] <=right_num){
                //更新左右
                //left_num = min(left_num,intervals[id][0]);
                right_num = max(right_num,intervals[id][1]);
            }
            else{
                vector<int>temp(2,0);
                temp[0] = left_num;
                temp[1] = right_num;
                res.emplace_back(temp);
                left_num = intervals[id][0];
                right_num = intervals[id][1];
            }

            if(id == intervals.size()-1){
                vector<int>temp(2,0);
                temp[0] = left_num;
                temp[1] = right_num;
                res.emplace_back(temp);
            }
        }
        return res;
    }
};