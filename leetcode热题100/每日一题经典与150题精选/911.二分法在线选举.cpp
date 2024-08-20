



/*
给你两个整数数组 persons 和 times 。
在选举中，第 i 张票是在时刻为 times[i] 时投给候选人 persons[i] 的。
对于发生在时刻 t 的每个查询，需要找出在 t 时刻在选举中领先的候选人的编号。
在 t 时刻投出的选票也将被计入我们的查询之中。
在平局的情况下，最近获得投票的候选人将会获胜。

实现 TopVotedCandidate 类：
TopVotedCandidate(int[] persons, int[] times) 使用 persons 和 times 数组初始化对象。
int q(int t) 根据前面描述的规则，返回在时刻 t 在选举中领先的候选人的编号。
 
示例：

输入：
["TopVotedCandidate", "q", "q", "q", "q", "q", "q"]
[[[0, 1, 1, 0, 0, 1, 0], [0, 5, 10, 15, 20, 25, 30]], [3], [12], [25], [15], [24], [8]]
输出：
[null, 0, 1, 1, 0, 0, 1]

解释：
TopVotedCandidate topVotedCandidate = new TopVotedCandidate([0, 1, 1, 0, 0, 1, 0], [0, 5, 10, 15, 20, 25, 30]);
topVotedCandidate.q(3); // 返回 0 ，在时刻 3 ，票数分布为 [0] ，编号为 0 的候选人领先。
topVotedCandidate.q(12); // 返回 1 ，在时刻 12 ，票数分布为 [0,1,1] ，编号为 1 的候选人领先。
topVotedCandidate.q(25); // 返回 1 ，在时刻 25 ，票数分布为 [0,1,1,0,0,1] ，编号为 1 的候选人领先。（在平局的情况下，1 是最近获得投票的候选人）。
topVotedCandidate.q(15); // 返回 0
topVotedCandidate.q(24); // 返回 0
topVotedCandidate.q(8); // 返回 1
*/













#include "util.h"

class TopVotedCandidate {
private:
    unordered_map<int,int>per_table;
    vector<int>cur_time;
    //记录在某个时间段内票数最高的人  top[i]中的i表示第i个时间段
    //由于只有在等于时间点的位置会出现对top的添加 所以应该是左闭右开的时间区间
    vector<int>top;
public:
    TopVotedCandidate(vector<int>& persons, vector<int>& times) {
        int len = persons.size();
        int id = 0;
        int top_id = -1;
        for(int id=0;id<len;id++){
            per_table[persons[id]]++;
            //注意这里是如何存储当前时刻下选票人的最大值的
            if(per_table[top_id] <= per_table[persons[id]]){
                top_id = persons[id];
            }
            top.emplace_back(top_id);
        }
        cur_time = times;
    }
    
    int q(int t) {
        //由t转化为时间段的数量  -- 这里使用二分法查找 加快查找速度
        //int count = cur_time.size()-1;
        // for(int i=1;i<cur_time.size();i++){
        //     if(t>=cur_time[i-1] && cur_time[i]>t){
        //         count = i-1;
        //         break;
        //     }
        // }
        int len = cur_time.size()-1;
        if(t > cur_time[len]){return top[len];}
        //pose 是小于等于t的第一个值的位置 
        int pose = upper_bound(cur_time.begin(),cur_time.end(),t) - cur_time.begin() - 1;
        return top[pose];
    }
};

/**
 * Your TopVotedCandidate object will be instantiated and called as such:
 * TopVotedCandidate* obj = new TopVotedCandidate(persons, times);
 * int param_1 = obj->q(t);
 */