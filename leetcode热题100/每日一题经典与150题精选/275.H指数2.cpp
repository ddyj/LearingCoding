/*
给你一个整数数组 citations ，
其中 citations[i] 表示研究者的第 i 篇论文被引用的次数，citations 已经按照 升序排列 。计算并返回该研究者的 h 指数。

h 指数的定义：h 代表“高引用次数”（high citations），一名科研人员的 h 指数是指他（她）的 （n 篇论文中）总共有 h 篇论文分别被引用了至少 h 次。

请你设计并实现对数时间复杂度的算法解决此问题。

输入：citations = [0,1,3,5,6]
输出：3
解释：给定数组表示研究者总共有 5 篇论文，每篇论文相应的被引用了 0, 1, 3, 5, 6 次。
     由于研究者有3篇论文每篇 至少 被引用了 3 次，其余两篇论文每篇被引用 不多于 3 次，所以她的 h 指数是 3 。
*/


#include"util.h"
//二分 ？ 
/*
    nums[mid] 表示至少有n-mid 篇论文被引用了 nums[mid]次  此时应该继续向右寻找
    nums[mid] >n-mid 表示mid-n的区域内的数字不满足题设 要向左寻找
    nums[mid] ==n-mid 表示mid-n区域满足题设

    返回返回的是left到n的距离 同时由于大于等于包含自己 不需要减一
*/

class Solution {
public:
    int hIndex(vector<int>& citations) {
        int len = citations.size();
        int right = citations.size()-1;
        int left = 0;
        while(left<=right){
            int mid = left + (right-left)/2 ; 
            if(citations[mid] > len-mid){
                right = mid-1;
            }
            else if(citations[mid] == len-mid){
                return citations[mid];
            }
            else{
                left = mid+1;
            }
        }
        //left 是下标的位置 需要的是left到n的距离
        return len-left;
    }
};