
#include "util.h"



/*
155. 最小栈
设计一个支持 push ，pop ，top 操作，并能在常数时间内检索到最小元素的栈。

实现 MinStack 类:

MinStack() 初始化堆栈对象。
void push(int val) 将元素val推入堆栈。
void pop() 删除堆栈顶部的元素。
int top() 获取堆栈顶部的元素。
int getMin() 获取堆栈中的最小元素。

输入：
["MinStack","push","push","push","getMin","pop","top","getMin"]
[[],[-2],[0],[-3],[],[],[],[]]

输出：
[null,null,null,null,-3,null,0,-2]

解释：
MinStack minStack = new MinStack();
minStack.push(-2);
minStack.push(0);
minStack.push(-3);
minStack.getMin();   --> 返回 -3.
minStack.pop();
minStack.top();      --> 返回 0.
minStack.getMin();   --> 返回 -2.

*/

/*

关于题中最小栈的说明 我们设定一个栈 他和记录数据的栈同步大小
例如当table中有i个数据的时候 那么最小栈min_table 中记录的是i个数据中的最小值

最小栈中实现原理:始终让栈顶元素front和当前值中取最小  我们始终保证栈顶元素是最小的 那么最小栈的入栈元素是所有元素中最小的
这一点可以用数学归纳法  当i=2是(插入第二个元素) min_table将插入的数据是min(num1,num2)  ……
 
https://leetcode.cn/problems/min-stack/solutions/242190/zui-xiao-zhan-by-leetcode-solution/?envType=study-plan-v2&envId=top-100-liked
*/


class MinStack {
private:
    stack <int>min_table;  //维护一个最小栈
    stack<int>table;
public:
    MinStack() {

    }
    
    void push(int val) {
        table.push(val);
        if(!min_table.empty()){
            min_table.push(min(min_table.top(),val));
        }
        else{
            min_table.push(val);
        }
    }
    
    void pop() {
        int temp = table.top();
        table.pop();
        min_table.pop();
    }
    
    int top() {
        return table.top();
    }
    
    int getMin() {
        return min_table.top();
    }
};

/**
 * Your MinStack object will be instantiated and called as such:
 * MinStack* obj = new MinStack();
 * obj->push(val);
 * obj->pop();
 * int param_3 = obj->top();
 * int param_4 = obj->getMin();
 */


