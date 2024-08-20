#include "util.h"







//TODO：
/*
「力扣」上的 0-1 背包问题：

「力扣」第 416 题：分割等和子集（中等）；
「力扣」第 474 题：一和零（中等）；
「力扣」第 494 题：目标和（中等）；
「力扣」第 879 题：盈利计划（困难）；
 

作者：liweiwei1419
链接：https://leetcode.cn/problems/partition-equal-subset-sum/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
0-1背包：
https://leetcode.cn/circle/discuss/KPsfIC/
https://leetcode.cn/problems/last-stone-weight-ii/solutions/805162/yi-pian-wen-zhang-chi-tou-bei-bao-wen-ti-5lfv/


文章总结的很全面，模板也很适用。但作为初学者，进来直接记模板，很容易弄混，也不知道所以然。因此，对每个题目有一定了解的基础上，再结合模板，会更容易记忆。因此下文会细化一些我的理解。
对于多层循环问题，我们最直观的理解，还是把状态定义成dp[i][j][k][...]的形式，从外层到内层的循环，一次是i、j、k...。而文中会出现有多层for循环，但实际dp状态定义，却比循环的维数少的情况。这些都是因为为了优化空间，故意将dp的维度减少的；或者最内侧循环不牵扯动态规划，而是一个累加过程。因此下文会对每一题的原始维度的dp状态的含义进行说明，方便大家记忆内外层循环顺序。（至于每题需要考虑到的细节问题，由每道题自己定，有时间再补充吧。）

1049. 最后一块石头的重量 II（首先，先将问题转换为背包问题）
dp[i][j]含义:从前i块石头中选取，选取值之和小于等于目标值j的最大值为dp。（i、j分别对应从外到内两层循环。）

322. 零钱兑换
dp[i][j]含义:前i种面额的硬币，组成总额为j金额使用的最少硬币数为dp。（i、j分别对应从外到内两层循环。）

416. 分割等和子集（首先，先将问题转换为背包问题）
dp[i][j]含义:对nums的前i个元素进行选取，dp记录是否存在选取值相加结果为目标值j。（dp是bool型）（i、j分别对应从外到内两层循环。）

494. 目标和（首先，先将问题转换为背包问题）
dp[i][j]含义:对nums的前i个元素进行选取相加值为目标值j的组合个数为dp。（i、j分别对应从外到内两层循环。）

279. 完全平方数（首先，先将问题转换为背包问题）
dp[i][j]含义:前i个完全平方数，组成目标值j时，使用的完全平方数最少个数为dp。（i、j分别对应从外到内两层循环。）

377. 组合总和 Ⅳ
dp[i][j]含义:组成目标值为i的组合的最后一个数是nums[j]的组合的个数为dp。（i、j分别对应从外到内两层循环。）。注：因为本题需要考虑组合的顺序，顺序不同则被视为不同的组合。所以状态定义要考虑到每种目标值的最后一个nums的选取上。

518. 零钱兑换 II
dp[i][j]含义:前i种面额的硬币，组成总额为j的金额的不同组合个数为dp（i、j分别对应从外到内两层循环。）。

1155. 掷骰子的N种方法：
dp[i][j]含义:前i个骰子总点数为j的不同组合个数为dp（i、j分别对应从外到内两层循环。）。最内侧对骰子面数的循环，是一个累加过程（最后一个骰子每个面朝上组合数的累加）。
*/








/*
416 分割等和子集
给你一个 只包含正整数 的 非空 数组 nums 。
请你判断是否可以将这个数组分割成两个子集，使得两个子集的元素和相等。

输入：nums = [1,5,11,5]
输出：true
解释：数组可以分割成 [1, 5, 5] 和 [11] 。
*/

/*

「0 - 1」 背包问题的思路
作为「0-1 背包问题」，它的特点是：「每个数只能用一次」。
解决的基本思路是：物品一个一个选，容量也一点一点增加去考虑，
这一点是「动态规划」的思想，特别重要。
在实际生活中，我们也是这样做的，一个一个地尝试把候选物品放入「背包」，
通过比较得出一个物品要不要拿走。
具体做法是：画一个 len 行，target + 1 列的表格。
这里 len 是物品的个数，target 是背包的容量。len 行表示一个一个物品考虑，
target + 1多出来的那 1 列，表示背包容量从 0 开始考虑。
很多时候，我们需要考虑这个容量为 0 的数值。

状态转移方程
dp[i][j] = dp[i - 1][j] or dp[i - 1][j - nums[i]]


作者：liweiwei1419
链接：https://leetcode.cn/problems/partition-equal-subset-sum/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
*/

/*
  每个元素都有从进入左子列和进入右子列的两个选择
  0-1背包问题
  本题可以考虑为 能否在数组中找到一个子数组 让这个子数组的综合等于总数组和的一半 
*/

class Solution {
public:
    bool canPartition(vector<int>& nums) {
        int len = nums.size();
        if(len<2){return false;}
        int sum = 0;
        int max_num = 0;
        for(int &p :nums){
            sum+=p;
            max_num = max(max_num,p);
        }
        //只有当数组总和为偶数的时候 才存在可以拆分的可能
        if(sum%2 != 0){return false;}
        int target = sum / 2 ;
        //如果数组中有一个比target大的数 由于数组所有元素非负 这里就可以直接返回false
        if(max_num > target ){return false;}
        //dp[i][j] 表示从0-i数组中选取子数组的和达到j的可能
        //边界条件  dp[i][0] = true    dp[i][nums[id]] = true;
        //递推关系  dp[i][j] = dp[i-1][j-nums[i]]  (j >= nums[i])  
        //          dp[i][j] = dp[i-1][j](不选取)      all
        vector<vector<bool>>dp(len,vector<bool>(target+1,false));
        
        //边界初始化
        for(int id=0;id<len;id++){
            if(nums[id] < target){dp[id][nums[id]] = true;}
            dp[id][0] = true;
        }

        //计算每一个区域
        for(int id=1;id<len;id++){
            for(int j=1;j<target+1;j++){
                if(j<nums[id]){
                    dp[id][j] = dp[id-1][j];
                }
                else{
                    dp[id][j] = dp[id-1][j] | dp[id-1][j-nums[id]];
                }
            }
        }

        return dp[len-1][target];
    }
};




/*
一和零

给你一个二进制字符串数组 strs 和两个整数 m 和 n 。
请你找出并返回 strs 的最大子集的长度，该子集中 最多 有 m 个 0 和 n 个 1 。
如果 x 的所有元素也是 y 的元素，集合 x 是集合 y 的 子集 。

输入：strs = ["10", "0001", "111001", "1", "0"], m = 5, n = 3
输出：4
解释：最多有 5 个 0 和 3 个 1 的最大子集是 {"10","0001","1","0"} ，因此答案是 4 。
其他满足题意但较小的子集包括 {"0001","1"} 和 {"10","1","0"} 。{"111001"} 不满足题意，因为它含 4 个 1 ，大于 n 的值 3 。
*/

//亮点 高纬度dp
//dp[len][m+1][n+1]   
//dp[i][j][k]表示当数组长度为0-i的时候 子集中的0的个数最多j和1的个数最多k的时候最多的子集数量  
//最终返回值是 dp[len][m][k]
//边界 dp[i][0][0] = 0  dp[0][hash[strs[0]fistr][hash[strs[0]second]] = 1
//递推关系
/*
  dp[i][j][k] =  
    选择strs[i]   max (dp[i-1][j][k] , dp[i-1][j-first][k-second])   (后两个括号大于0)
    不选strs[i]   dp[i-1][j][k]
*/   

class Solution {
private:
    unordered_map<string,pair<int,int>>hash_table; // string - 0 - 1 
public:
    int findMaxForm(vector<string>& strs, int m, int n) {
        int len = strs.size();
        //建表
        for(string str:strs){   
            int str_len = str.length();
            int zero_num = 0 ;
            for(int i=0;i<str_len;i++){
                if(str[i]=='0'){zero_num++;}
            }
            hash_table[str] = make_pair(zero_num,str_len-zero_num);
        }
        if(len==1){
            if(m>=hash_table[strs[0]].first && n>=hash_table[strs[0]].second){return 1;}
            return 0;
        }

               
        vector<vector<vector<int>>>dp(len,vector<vector<int>>(m+1,vector<int>(n+1,0)));
        //处理边界
        int zero_num = hash_table[strs[0]].first;
        int one_num = hash_table[strs[0]].second;
        //由于是最大含有0-1 所以大于当前zero_num、one_num的区域都是1
        for(int j=0;j<=m;j++){
            for(int k=0;k<=n;k++){
                if(k>=one_num && j>=zero_num){
                    dp[0][j][k] = 1;
                }
            }
        }
        
        //递推
        for(int id=1;id<len;id++){
            int zero_num = hash_table[strs[id]].first;
            int one_num = hash_table[strs[id]].second;
            //这里jk可以从0开始 因为会存在两者一个不为0的情况
            for(int j=0;j<=m;j++){
                for(int k=0;k<=n;k++){
                    dp[id][j][k] = dp[id-1][j][k];
                    if(k>=one_num && j>=zero_num){
                        dp[id][j][k] = max (dp[id][j][k] , dp[id-1][j-zero_num][k-one_num] + 1);
                    }
                }
            }
        }
        return dp[len-1][m][n];
    }
};


/*
494. 目标和
给你一个非负整数数组 nums 和一个整数 target 。

向数组中的每个整数前添加 '+' 或 '-' ，然后串联起所有整数，可以构造一个 表达式 ：

例如，nums = [2, 1] ，可以在 2 之前添加 '+' ，在 1 之前添加 '-' ，然后串联起来得到表达式 "+2-1" 。
返回可以通过上述方法构造的、运算结果等于 target 的不同 表达式 的数目。

输入：nums = [1,1,1,1,1], target = 3
输出：5
解释：一共有 5 种方法让最终目标和为 3 。
-1 + 1 + 1 + 1 + 1 = 3
+1 - 1 + 1 + 1 + 1 = 3
+1 + 1 - 1 + 1 + 1 = 3
+1 + 1 + 1 - 1 + 1 = 3
+1 + 1 + 1 + 1 - 1 = 3
*/


//0-1问题转换(重点 直接存和的话 就会有一个负值问题的考虑 不利于dp数组的存储 但是neg一定是大于0的)

//统计所有的数组和sum 因为每一个num大于0 sum一定大于0的 设前面加负号的和为neg 那么一定满足 sum - neg - neg = target
//neg = (sum - target )/2   -- 如果sum - target不是非负偶数 就不可能返回
//有几种子序列和为neg的方案 就是几个不同表达式的问题

//dp[len][neg+1]  长度为i 和为j的方案数
//dp[0][0] = nums[0] == 0 ? 1 : 0
//dp[0][nums[0]] = 1
//dp[i][j] = dp[i-1][j] 
//dp[i][j] = dp[i-1][j]+dp[i-1][j-nums[id]] j - nums[id] > 0


class Solution {
public:
    int findTargetSumWays(vector<int>& nums, int target) {
        int sum = 0;
        int neg = 0;
        int len = nums.size();
        for(int p:nums){sum+=p;}
        if(sum - target < 0 || (sum-target)%2!=0 ){return 0;}
        neg = (sum-target)/2;
        vector<vector<int>>dp(len+1,vector<int>(neg+1,0));
        //边界位置
        dp[0][0] = 1;
        for(int id=1;id<=len;id++){
            for(int j=0;j<=neg;j++){
                int num = nums[id-1];
                dp[id][j] = dp[id-1][j];
                if(j >= num){
                    dp[id][j] += dp[id-1][j-num];
                }
            }
        }
        return dp[len][neg];
    }
};

/*
1049 最后一块石头的重量II

有一堆石头，用整数数组 stones 表示。其中 stones[i] 表示第 i 块石头的重量。

每一回合，从中选出任意两块石头，然后将它们一起粉碎。假设石头的重量分别为 x 和 y，且 x <= y。那么粉碎的可能结果如下：

如果 x == y，那么两块石头都会被完全粉碎；
如果 x != y，那么重量为 x 的石头将会完全粉碎，而重量为 y 的石头新重量为 y-x。
最后，最多只会剩下一块 石头。返回此石头 最小的可能重量 。如果没有石头剩下，就返回 0
输入：stones = [2,7,4,1,8,1]
输出：1
解释：
组合 2 和 4，得到 2，所以数组转化为 [2,7,1,8,1]，
组合 7 和 8，得到 1，所以数组转化为 [2,1,1,1]，
组合 2 和 1，得到 1，所以数组转化为 [1,1,1]，
组合 1 和 1，得到 0，所以数组转化为 [1]，这就是最优值。
*/

/*
    类似目标和问题  如果选取ab碰撞，那么实际上是消失了ab两块石头 生成了一块a-b的石头
    在选取一块石头c 那么就是c - (a-b) 块石头
    以此类推，实际上是求 对数组每一个数选择合理的正负值 使得其结果最小
    在分析一下 就是选取多少石头使得其重量的和 尽可能接近sum/2
*/


class Solution {
public:
    int lastStoneWeightII(vector<int>& stones) {  
        int len = stones.size();
        if(len==1){return stones[0];}
        if(len==2){return abs(stones[1]-stones[0]);}  
        int sum = 0;
        for(int &p : stones){
            sum+=p;
        }
        
        int target = sum/2;
        //dp[i][j] 前i个石头 和不超过j的最大值
        vector<vector<int>>dp(len+1,vector<int>(target+1,0));
        //边界
        for(int i=1;i<=target;i++){
            if(i>=stones[0]){
                dp[1][i] = stones[0];
            }
        }
        //递推
        for(int i=2;i<=len;i++){
            for(int j=0;j<=target;j++){
                dp[i][j] = dp[i-1][j];
                if(j>=stones[i-1]){
                    dp[i][j] = max(dp[i][j],dp[i-1][j-stones[i-1]]+stones[i-1]);
                }
            }
        }
        return sum - 2*dp[len][target];
    }
};