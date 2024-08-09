//首先 给出基础堆排序的代码


#include"util.h"


//对num实现堆排序 并且去除第k的元素 或者交换到第i次


class Solution {
public:
    void my_swap(vector<int>& nums,int pose1,int pose2){
        int tmp = nums[pose1];
        nums[pose1] = nums[pose2];
        nums[pose2] = tmp;
    }
    //调整根堆结构
    void heapify(vector<int>& nums, int pose ,int len){
        int lson = pose*2 +1;
        int rson = pose*2 +2;

        int lagest = pose;
        //查找最大子叶结点的id
        if(lson < len && nums[lson] > nums[lagest]){
            lagest = lson;
        } 
        if(rson < len && nums[rson] > nums[lagest]){
            lagest = rson;
        }
        //此时需要维护
        if(lagest!=pose){
            my_swap(nums,pose,lagest);
            heapify(nums,lagest,len);
        }
    }
    //堆排序
    void heapsort(vector<int>&nums,int len){
        for(int i=(len-1-1)/2 ; i>=0; i--){
            heapify(nums,i,len);
        }
        //排序 每次选取根堆最后一个结点和根堆根节点交换 将剩余元素维护根堆
        for(int i=len-1;i>0;i--){
            my_swap(nums,0,i);
            heapify(nums,0,i);
        }
    }
    int findKthLargest(vector<int>& nums, int k) {
        //堆排序实现
        int len = nums.size();
        heapsort(nums,len);
        //debug
        for(int i : nums){std::cout<<i<<" ";}
        std::cout<<std::endl;

        return 1;
    }
};





/*
给定整数数组 nums 和整数 k，请返回数组中第 k 个最大的元素。
请注意，你需要找的是数组排序后的第 k 个最大的元素，
而不是第 k 个不同的元素。
你必须设计并实现时间复杂度为 O(n) 的算法解决此问题。

输入: [3,2,1,5,6,4], k = 2
输出: 5
*/
//对num实现堆排序 并且去除第k的元素 或者交换到第i次


class Solution {
public:
    void my_swap(vector<int>& nums,int pose1,int pose2){
        int tmp = nums[pose1];
        nums[pose1] = nums[pose2];
        nums[pose2] = tmp;
    }
    //调整根堆结构
    void heapify(vector<int>& nums, int pose ,int len){
        int lson = pose*2 +1;
        int rson = pose*2 +2;

        int lagest = pose;
        //查找最大子叶结点的id
        if(lson < len && nums[lson] > nums[lagest]){
            lagest = lson;
        } 
        if(rson < len && nums[rson] > nums[lagest]){
            lagest = rson;
        }
        //此时需要维护
        if(lagest!=pose){
            my_swap(nums,pose,lagest);
            heapify(nums,lagest,len);
        }
    }
    //堆排序
    int heapsort(vector<int>&nums,int len,int k){
        for(int i=(len-1-1)/2 ; i>=0; i--){
            heapify(nums,i,len);
        }
        //排序 每次选取根堆最后一个结点和根堆根节点交换 将剩余元素维护根堆
        //由于是取前k个元素，那么我们可以做一个记录 只需要记录已经出堆的第k个不同的元素即可
        int count = 0;

        for(int i=len-1;i>=0;i--){
            my_swap(nums,0,i);
            //std::cout<<nums[i]<<std::endl;
            count++;
            if(count == k){return nums[i];}
            
            
            heapify(nums,0,i);
        }
        return 0;
    }

    int findKthLargest(vector<int>& nums, int k) {
        //堆排序实现
        int len = nums.size();
        if(len==1){return nums[0];}
        return heapsort(nums,len,k);
        //debug
        //for(int i : nums){std::cout<<i<<" ";}
        //std::cout<<std::endl;

        //return nums[len-k+1];
    }
};