#include "util.h"

/*
   实现 pow(x, n) ，即计算 x 的整数 n 次幂函数（即，xn ）。
*/

/*
    快速幂  平方求逆
        例如· 3^10 = (3*3)^5 = 9^5 = 9 * 9^4 = 9* 81^2 = 9 * 6561^1 = 9 * 6561 * 6561^0
*/

class Solution {
public:
    //res 存的是每一组x，n的幂(递归每一层是子任务，存的结果也是子结果) 所以在n==0的时候 需要返回1.0
    //注意两个递归的位置  对于非0的偶数n 最后一定会除到1 所以偶数直接进入下一轮
    //                  对于奇数 让n-1 计算n-1的当前x的幂 计算完毕后再乘提出的1次幂
    //                  事实上 当n=1的时候 这里的x就是我们要的结果  所以(n==1)的if加不加都一样
    void quick_pow(double x,long long n,double &res){
        if(n==0){res = 1.0;return;}
        //if(n==1){res = x;return;}
        if(n%2==1){
            n=n-1;
            quick_pow(x,n,res);
            res = res*x;
        }
        else{
            n=n/2;
            res = x*x;
            quick_pow(x*x,n,res);
        }
    }


    double myPow(double x, int n) {
        double res = 1.0;
        long long N = n;
        quick_pow(x,abs(N),res);
        return n<0 ? 1.0/res : res;
    }
};