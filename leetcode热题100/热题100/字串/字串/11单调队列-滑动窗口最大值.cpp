#include"util.h"


/*
    239滑动窗口的最大值
    给你一个整数数组 nums，有一个大小为 k 的滑动窗口从数组的最左侧移动到数组的最右侧。
    你只可以看到在滑动窗口内的 k 个数字。滑动窗口每次只向右移动一位。
    返回 滑动窗口中的最大值 。

    输入：nums = [1,3,-1,-3,5,3,6,7], k = 3
    输出：[3,3,5,5,6,7]
    解释：
    滑动窗口的位置                最大值
    ---------------               -----
    [1  3  -1] -3  5  3  6  7       3
    1 [3  -1  -3] 5  3  6  7       3
    1  3 [-1  -3  5] 3  6  7       5
    1  3  -1 [-3  5  3] 6  7       5
    1  3  -1  -3 [5  3  6] 7       6
    1  3  -1  -3  5 [3  6  7]      7
*/



//注意学会单调队列的写法 
/*
单调队列真是一种让人感到五味杂陈的数据结构，它的维护过程更是如此.....
就拿此题来说，队头最大，往队尾方向单调......
有机会站在队头的老大永远心狠手辣，当它从队尾杀进去的时候，
如果它发现这里面没一个够自己打的，它会毫无人性地屠城，把原先队里的人头全部丢出去，转身建立起自己的政权，野心勃勃地准备开创一个新的王朝.....
这时候，它的人格竟发生了一百八十度大反转，它变成了一位胸怀宽广的慈父！
它热情地请那些新来的“小个子”们入住自己的王国......
然而，这些小个子似乎天性都是一样的——嫉妒心强，倘若见到比自己还小的居然更早入住王国，
它们会心狠手辣地找一个夜晚把它们通通干掉，好让自己享受更大的“蛋糕”；
当然，遇到比自己强大的，它们也没辙，乖乖夹起尾巴做人。
像这样的暗杀事件每天都在上演，虽然王国里日益笼罩上白色恐怖，但是好在没有后来者强大到足以干翻国王，江山还算能稳住。
直到有一天，
闯进来了一位真正厉害的角色，就像当年打江山的国王一样，手段狠辣，野心膨胀，于是又是大屠城......历史总是轮回的。
*/
class Solution {
public:
    vector<int> maxSlidingWindow(vector<int>& nums, int k) {
        int len = nums.size();
        vector<int>res;
        deque<int>que;   //维护一个还在窗口中的单调队列 从大到小 单调队列中存储的是下标
        for(int i=0;i<k;i++){
            while(!que.empty() && nums[i] >= nums[que.back()]){
                que.pop_back();
            }
            que.push_back(i);
        }
        //这里形成了第一个窗口
        res.emplace_back(nums[que.front()]);
        //之后维护单调队列 保证 
        /*
            1.新来的大元素一定是在队列中单调递减放置的
            2.如果窗口已近划过队列头 就要将队列头出栈
        */
        for(int i=k;i<len;i++){
            while(!que.empty() && nums[i] >= nums[que.back()]){
                que.pop_back();
            }
            que.push_back(i);
            while(que.front() <=i-k){
                que.pop_front();
            }
            res.emplace_back(nums[que.front()]);
        }
        return res;

    }
};


//单调队列同类型
/*
1438 绝对差不超过限制的最长连续子数组
给你一个整数数组 nums ，和一个表示限制的整数 limit，请你返回最长连续子数组的长度，
该子数组中的任意两个元素之间的绝对差必须小于或者等于 limit 。

如果不存在满足条件的子数组，则返回 0 。
输入：nums = [8,2,4,7], limit = 4
输出：2 
解释：所有子数组如下：
[8] 最大绝对差 |8-8| = 0 <= 4.
[8,2] 最大绝对差 |8-2| = 6 > 4. 
[8,2,4] 最大绝对差 |8-2| = 6 > 4.
[8,2,4,7] 最大绝对差 |8-2| = 6 > 4.
[2] 最大绝对差 |2-2| = 0 <= 4.
[2,4] 最大绝对差 |2-4| = 2 <= 4.
[2,4,7] 最大绝对差 |2-7| = 5 > 4.
[4] 最大绝对差 |4-4| = 0 <= 4.
[4,7] 最大绝对差 |4-7| = 3 <= 4.
[7] 最大绝对差 |7-7| = 0 <= 4. 
因此，满足题意的最长子数组的长度为 2 。
*/


// 滑动窗口维护连续子数组的长度  有序队列/单调队列记录当前窗口的最大/最小值
// 最大-最小的绝对值大于limit 则向左缩小窗口


class Solution {
public:
    //滑动窗口 + 有序队列模式
    int longestSubarray_01(vector<int>& nums, int limit) {
        int max_len = 1;
        int len = nums.size();
        if(len==1){return max_len;}
        //维护有序集合方式:
        /*
            最大值 - 最小值 绝对值小于等于limit 此时 添加新元素 否则移除队头
            同时需要两个变量维护队列中最大值和最小值的下标
            避免出现因为维护单调性时出栈数据的损失
        */
        multiset<int>table;
        int left = 0;
        int right = 0;
        while(right<len){
            table.insert(nums[right]);
            while(!table.empty() && *table.rbegin() - *table.begin() > limit){
                table.erase(table.find(nums[left]));
                left++;
            }
            max_len = max(max_len,right-left+1);
            right++;
        }
        

        return max_len;
    }
    //使用滑动窗口 + 单调队列 由于我们需要最大最小值 所以我们可以分别维护两个队列 分别记录最大最小值
    int longestSubarray(vector<int>& nums, int limit) {
        int max_len = 1;
        int len = nums.size();
        if(len==1){return max_len;}
        int left = 0;
        int right = 0;
        deque<int>dp_max,dp_min;
        while(right<len){
            while(!dp_max.empty()&&dp_max.back()<nums[right]){
                dp_max.pop_back();
            }
            dp_max.push_back(nums[right]);
            while(!dp_min.empty()&&dp_min.back()>nums[right]){
                dp_min.pop_back();
            }
            dp_min.push_back(nums[right]);

            while(!dp_max.empty() && !dp_min.empty() && abs(dp_max.front() - dp_min.front()) > limit ){
                if(nums[left]==dp_max.front()){
                    dp_max.pop_front();
                }
                if(nums[left]==dp_min.front()){
                    dp_min.pop_front();
                }
                left++;
            }
            max_len = max(max_len,right-left+1);
            right++;
        }
        

        return max_len;
    }
};