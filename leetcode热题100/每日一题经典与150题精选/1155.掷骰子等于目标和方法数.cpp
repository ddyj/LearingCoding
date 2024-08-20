#include "util.h"


/*
1155. 掷骰子等于目标和的方法数

这里有 n 个一样的骰子，每个骰子上都有 k 个面，分别标号为 1 到 k 。
给定三个整数 n ,  k 和 target ，返回可能的方式(从总共 kn 种方式中)滚动骰子的数量，
使正面朝上的数字之和等于 target 。
答案可能很大，你需要对 109 + 7 取模 。

*/

class Solution {
private:
    static constexpr int mod = 1e9 + 7;
public:
    //分析 每个骰子有1-6的可能 本质上是一个dfs和情况 但是 dfs时间复杂度会超时
    //这里使用动态规划
    /*
        dp[i][j]表示扔i个骰子的和为j的方案数
        f(0,0) = 0  f(1,k) = 1
        最终求f(n,target)
        状态转移方程 如果第i个骰子结果是k f(i,j) = f(i-1,j-k)
        即 f(i,j) = f(i-1,j-1) + f(i-1,j-2) + ……
        
    */
    //组合和的情况
    int numRollsToTarget(int n, int k, int target) {
        //题中设定的最大值  即使n、k取最大 30*30 最大和也是小于target10001的最大值
        //由于目标值是target 所以我们可以设置和最大值为target 大于target我们不在维护
        int f[n+1][target+1];  
        memset(f,0,sizeof f);
        f[0][0] = 1;  
        for(int id =1;id<=min(k,target);id++){
            f[1][id] = 1;
        }
        for(int i=2;i<=n;i++){
            //这里i*k表示在当前i个骰子的情况下 最大的和的情况
            //min(target,i*k) 表示我们只维护和到target的结果 大于这个值的结果我们都不需要
           for(int j=1;j<=min(target,i*k);j++){
               //这里遍历j-1到j-k 描述每一种抛出结果的值
               for(int h=1;h<=k;h++){
                   if(j-h>=0){
                       f[i][j] = (f[i][j] + f[i-1][j-h]) % mod;
                   }   
               }
           } 
        }

        return f[n][target];
    }

    //1030更新
    int numRollsToTarget_1030(int n, int k, int target) {
        //dp[i][j] i个骰子和为j的方法数 返回dp[k][target]
        vector<vector<int>>dp(n+1,vector<int>(target+1,0));
        //边界值 dp[][0] = 0 dp[1][k] = 1 
         
        for(int id=1;id<=target;id++){
            if(id<=k){dp[1][id] = 1;}
        }
        //遍历
        for(int id=2;id<=n;id++){
            for(int j=1;j<=target;j++){
                for(int h=1;h<=k;h++){
                    if(j-h>=0){
                        dp[id][j] = (dp[id][j] + dp[id-1][j-h]) % mod;
                    }  
                }   
            }
        }
        return dp[n][target]%mod;
    }
};