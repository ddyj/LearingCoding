#include"util.h"



/*
你正在探访一家农场，农场从左到右种植了一排果树。
这些树用一个整数数组 fruits 表示，其中 fruits[i] 是第 i 棵树上的水果 种类 。

你想要尽可能多地收集水果。然而，农场的主人设定了一些严格的规矩，你必须按照要求采摘水果：

你只有 两个 篮子，并且每个篮子只能装 单一类型 的水果。每个篮子能够装的水果总量没有限制。
你可以选择任意一棵树开始采摘，你必须从 每棵 树（包括开始采摘的树）上 恰好摘一个水果 。
采摘的水果应当符合篮子中的水果类型。每采摘一次，你将会向右移动到下一棵树，并继续采摘。
一旦你走到某棵树前，但水果不符合篮子的水果类型，那么就必须停止采摘。
给你一个整数数组 fruits ，返回你可以收集的水果的 最大 数目。


示例 1：

输入：fruits = [1,2,1]
输出：3
解释：可以采摘全部 3 棵树。
示例 2：

输入：fruits = [0,1,2,2]
输出：3
解释：可以采摘 [1,2,2] 这三棵树。
如果从第一棵树开始采摘，则只能采摘 [0,1] 这两棵树。
示例 3：

输入：fruits = [1,2,3,2,2]
输出：4
解释：可以采摘 [2,3,2,2] 这四棵树。
如果从第一棵树开始采摘，则只能采摘 [1,2] 这两棵树。
*/

/*
    数据结构声明  
        我们定义一个deque<pair<int,int>> 
        第一个值用来记录篮子的种类 第二值用来记录这个种类为最新出现种类且连续出现时候起点的id
        对于种类 我们始终保证最新出现的种类的处于队列尾
        对于id  记录的是连续出现的起点的id 用于更新左值
        例如3,3,3,1,2,1,1,3,3,4  当种类为 1 2是 出现3的时候 此时种类为1 3 
                                               左值应该是从下标为5的'1'开始 而不是为6的'1' 
        

*/

class Solution {
public:
    //只包含两种元素的最长连续子序列
    int totalFruit(vector<int>& fruits) {
        //first 类型 second 位置
        deque<pair<int,int>>dp;  
        int left = 0;
        int right = 0;
        int res = 0;
        int len = fruits.size();
        while(right < len){
            //如果当前种类数不足2种
            if(dp.size()<2){
                if(dp.empty() || (!dp.empty() && dp.front().first!=fruits[right])){
                    dp.push_back(make_pair(fruits[right],right));
                }
                right++;
            }
            //如果当前种类数达到2种
            else{
                //如果当前元素是dp中后入列的元素 就继续移动窗口
                if(fruits[right] == dp.back().first){right++;}
                //如果当前元素是dp中先入列的元素 就更新dp中的先后关系
                else if(fruits[right] == dp.front().first){dp.pop_front();dp.push_back(make_pair(fruits[right],right));right++;}
                //如果新元素在dp中没有出现 就把dp中旧的元素出栈 输入新的元素
                else{
                    res = max(res,right-left);
                    dp.pop_front();
                    dp.push_back(make_pair(fruits[right],right));
                    //更新左值 左值应该是当前上一个出现元素在数组中的一次出现位置
                    //例如 0 1 6 6 4 4 6  dp 1 6 此时fruits[right] = 4
                    //执行上述操作后 dp为 6 4 left应该找第一次出现6的位置 
                    // while(left<right){
                    //     if(fruits[left] == dp.front()){break;}
                    //     left++;
                    // }
                    left = dp.front().second;
                    //对右值的更新 继续加一即可
                    right++;
                }
            }
            res = max(res,right-left);
        }
        return res;
    }
};