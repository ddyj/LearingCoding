#include "util.h"

/*

中位数是有序整数列表中的中间值。
如果列表的大小是偶数，则没有中间值，中位数是两个中间值的平均值。

例如 arr = [2,3,4] 的中位数是 3 。
例如 arr = [2,3] 的中位数是 (2 + 3) / 2 = 2.5 。
实现 MedianFinder 类:

MedianFinder() 初始化 MedianFinder 对象。

void addNum(int num) 将数据流中的整数 num 添加到数据结构中。

double findMedian() 返回到目前为止所有元素的中位数。
与实际答案相差 10-5 以内的答案将被接受。

输入
["MedianFinder", "addNum", "addNum", "findMedian", "addNum", "findMedian"]
[[], [1], [2], [], [3], []]
输出
[null, null, null, 1.5, null, 2.0]

解释
MedianFinder medianFinder = new MedianFinder();
medianFinder.addNum(1);    // arr = [1]
medianFinder.addNum(2);    // arr = [1, 2]
medianFinder.findMedian(); // 返回 1.5 ((1 + 2) / 2)
medianFinder.addNum(3);    // arr[1, 2, 3]
medianFinder.findMedian(); // return 2.0
*/


/*

我们定义两个priority_queue 左侧为由大到小 右侧为由小到大

Q：为什么这么定义 ？
A：首先我们借助优先队列的堆排方式，可以对数据流进行排序
   其次因为priority_queue并不想deque可以从两侧访问，只能对top的数据进行访问，
   我们将左侧的顺序写反，为的是可以访问到参与中位数运算的堆顶元素

接着对于插入数据，判断其和当前中位数关系，如果小于当前中位数，就加入左侧，反之加入右侧
然后对左右队列调整，直到有以下两种情况：
    1 右侧长度 == 左侧长度：中位数为右侧top + 左侧top的均值
    2 右侧长度 - 左侧长度 = 1 ：中位数为右侧top

调整逻辑：有以下两种情况需要调整：
    1.左侧长度大于右侧长度 -- 左侧top出进右侧
    2.右侧长度-左侧长度 >=2 -- 右侧top出进左侧
*/


/*
这类问题，一定要搞清楚在总长度分别为奇数/偶数的情况下，进/出栈是什么情况
*/

class MedianFinder {
private:
    //左侧由大到小 
    priority_queue<int,vector<int>,less<int>>nums_l;
    //右侧由小到大
    priority_queue<int,vector<int>,greater<int>>nums_r;

    int len_l;
    int len_r;
public:
    MedianFinder() {
        len_l = 0;
        len_r = 0;
    }
    
    void addNum(int num) {
        //如果一开始右侧没有数值，就直接加到右侧队列中去
        if(len_r==0){
            nums_r.push(num);
            len_r++;
        }
        else{
            //num > mid
            if(num > nums_r.top()){
                nums_r.push(num);
                len_r++;
            }
            else{
                nums_l.push(num);
                len_l++;
            }
        }

        while(len_r-len_l !=1 && len_r-len_l!=0){
            if(len_l > len_r){
                nums_r.push(nums_l.top());
                nums_l.pop();
                len_l--;
                len_r++;
            }
            else if(len_r - len_l >= 2){
                nums_l.push(nums_r.top());
                nums_r.pop();
                len_l++;
                len_r--;
            }
        }
    }
    
    double findMedian() {
        if(len_l == len_r){
            return double(nums_l.top() + nums_r.top()) / 2.0; 
        }
        return static_cast<double>(nums_r.top());
    }
};

/**
 * Your MedianFinder object will be instantiated and called as such:
 * MedianFinder* obj = new MedianFinder();
 * obj->addNum(num);
 * double param_2 = obj->findMedian();
 */


/*
相似题目  双端队列设计 1670
请你设计一个队列，支持在前，中，后三个位置的 push 和 pop 操作。

请你完成 FrontMiddleBack 类：

FrontMiddleBack() 初始化队列。
void pushFront(int val) 将 val 添加到队列的 最前面 。
void pushMiddle(int val) 将 val 添加到队列的 正中间 。
void pushBack(int val) 将 val 添加到队里的 最后面 。
int popFront() 将 最前面 的元素从队列中删除并返回值，如果删除之前队列为空，那么返回 -1 。
int popMiddle() 将 正中间 的元素从队列中删除并返回值，如果删除之前队列为空，那么返回 -1 。
int popBack() 将 最后面 的元素从队列中删除并返回值，如果删除之前队列为空，那么返回 -1 。
请注意当有 两个 中间位置的时候，选择靠前面的位置进行操作。比方说：

将 6 添加到 [1, 2, 3, 4, 5] 的中间位置，结果数组为 [1, 2, 6, 3, 4, 5] 。
从 [1, 2, 3, 4, 5, 6] 的中间位置弹出元素，返回 3 ，数组变为 [1, 2, 4, 5, 6] 。
*/

class FrontMiddleBackQueue {
private:
    deque<int>left;
    deque<int>right;
    int len_l;
    int len_r;
public:
    FrontMiddleBackQueue() {
        len_l=0;
        len_r=0;
    }
    
    void adjustque(int val){
        if(val>0){
            while(len_r-len_l!= 1 && len_r-len_l!=0){
                if(len_l > len_r){
                    right.push_front(left.back());
                    left.pop_back();
                    len_l--;
                    len_r++;
                }
                else if(len_r - len_l >=2){
                    left.push_back(right.front());
                    right.pop_front();
                    len_l++;
                    len_r--;
                }
            }
        }
        else{
            while(len_r-len_l!= 1 && len_r-len_l!=2){
                if(len_l >= len_r){
                    right.push_front(left.back());
                    left.pop_back();
                    len_l--;
                    len_r++;
                }
                else if(len_r - len_l > 2){
                    left.push_back(right.front());
                    right.pop_front();
                    len_l++;
                    len_r--;
                }
            }
        }

    }


    void pushFront(int val) {
        left.push_front(val);
        len_l++;
    }
    
    void pushMiddle(int val) {
        adjustque(1);
        right.push_front(val);
        len_r++;
    }
    
    void pushBack(int val) {
        right.push_back(val);
        len_r++;
    }
    
    int popFront() {
        if(len_l == 0){
            if(len_r == 0){return -1;}
            else{
               left.push_back(right.front());
               right.pop_front();
               len_l ++;
               len_r --; 
            }
        }
        int res = left.front();
        left.pop_front();
        len_l--;
        return res;
    }
    
    int popMiddle() {
        if(len_l == 0 && len_r == 0){
            return -1;
        }
        //std::cout<<len_l << " "<<len_r<<std::endl;
        adjustque(-1);
        int mid = right.front();
        right.pop_front();
        len_r--;
        return mid;
    }
    
    int popBack() {
        if(len_r == 0){
            if(len_l == 0){return -1;}
            else{
               right.push_front(left.back());
               left.pop_back();
               len_l --;
               len_r ++; 
            }
        }
        int res = right.back();
        right.pop_back();
        len_r--;
        return res;
    }
};

/**
 * Your FrontMiddleBackQueue object will be instantiated and called as such:
 * FrontMiddleBackQueue* obj = new FrontMiddleBackQueue();
 * obj->pushFront(val);
 * obj->pushMiddle(val);
 * obj->pushBack(val);
 * int param_4 = obj->popFront();
 * int param_5 = obj->popMiddle();
 * int param_6 = obj->popBack();
 */