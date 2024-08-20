
#include "util.h"

/*
739.每日温度
给定一个整数数组 temperatures ，表示每天的温度，返回一个数组 answer ，
其中 answer[i] 是指对于第 i 天，下一个更高温度出现在几天后。如果气温在这之后都不会升高，请在该位置用 0 来代替
*/

//单调栈
//栈内记录了元素值和下标 计算出现间隔的时候就用下标的差进行计算

class Solution {
public:
    vector<int> dailyTemperatures(vector<int>& temperatures) {
        stack<pair<int,int>>table;
        int len = temperatures.size();
        vector<int>res(len,0);
        for(int id=0;id<len;id++){
            while(!table.empty() && table.top().first < temperatures[id]){
                int pos = table.top().second;
                table.pop();
                res[pos] = id-pos;
            }
            table.push(make_pair(temperatures[id],id));
        }
        return res;
    }
};