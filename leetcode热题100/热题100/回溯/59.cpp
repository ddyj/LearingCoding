


#include "util.h"

/*
22. 括号生成
数字 n 代表生成括号的对数，请你设计一个函数，用于能够生成所有可能的并且 有效的 括号组合
示例 1：

输入：n = 3
输出：["((()))","(()())","(())()","()(())","()()()"]
*/


//1. 暴力dfs  -- 时间利用率太差
class Solution {
public:
    //front_size 前括号的数量 只有这个值大于0或等于n的时候 才能生成后括号的分支 
    //patch_size 匹配括号的数量
    void dfs(vector<string>&res,int n,int front_size,int patch_size,string path){
        if(patch_size==n && front_size==0){res.emplace_back(path);return;}
        if(front_size>n){return;}
        if(path.length()>n*2){return;}
        if(front_size==0){
            path += "(" ;
            front_size++;
            dfs(res,n,front_size,patch_size,path);
            front_size--;
            path = path.substr(0,path.length()-1);
        }
        else if(front_size==n){
            path += ")" ;
            front_size--;
            patch_size++;
            dfs(res,n,front_size,patch_size,path);
            patch_size--;
            front_size++;
            path = path.substr(0,path.length()-1);
        }
        else{
            for(int id=0;id<2;id++){
                if(id%2==0){
                    path += ")" ;
                    front_size--;
                    patch_size++;
                    dfs(res,n,front_size,patch_size,path);
                    patch_size--;
                    front_size++;
                    path = path.substr(0,path.length()-1);
                }
                else{
                    path += "(" ;
                    front_size++;
                    dfs(res,n,front_size,patch_size,path);
                    front_size--;
                    path = path.substr(0,path.length()-1);
                }
            }
        }
    }


    //难点 如何保证dfs的时候 括号是正常匹配的
    vector<string> generateParenthesis(int n) {
        vector<string>res;
        string path;
        dfs(res,n,0,0,path);
        return res;
    }
};


//改进： 用左右括号数量约束 而不是用生成括号总数约束 这样避免了很多过多深入的枝干
class Solution {
public:
    //front_size 前括号的可用数量 只有这个值大于0或等于n的时候 才能生成后括号的分支 
    //back_size  后括号的可用数量 只有这个值大于0且小于front_size值的大小才会产生前括号分值
    void dfs(vector<string>&res,int n,int front_size,int back_size,string path){
        if(front_size ==0 && back_size ==0){res.emplace_back(path);return;}
        /// 剪枝 左括号可以使用的个数严格大于右括号可以使用的个数，才剪枝
        if(front_size>back_size){return;}
        //if(path.length()>n*2){return;}
        if(front_size>0){
            path += "(";
            dfs(res,n,front_size-1,back_size,path);
            path = path.substr(0,path.length()-1);
        }
        if(back_size>0){
            path += ")";
            dfs(res,n,front_size,back_size-1,path);
            path = path.substr(0,path.length()-1);
        }
    }


    //难点 如何保证dfs的时候 括号是正常匹配的
    vector<string> generateParenthesis(int n) {
        vector<string>res;
        string path ="";
        if(n==0){return res;}
        dfs(res,n,n,n,path);
        return res;
    }
};