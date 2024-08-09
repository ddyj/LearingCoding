#include "util.h"


/*
我们给出一般快排的实现，由于不断的调用循环计算 导致时间复杂度很高
*/
class Solution {
public:
    void quickSort(vector<int>&nums,int left,int right){
        if(left >= right){return;}
        int partion = nums[left];
        int start = left;
        int end = right;
        while(left < right){
            while(left < right && nums[right] >= partion){
                --right;
            }
            if(left < right &&nums[right] < partion){
                nums[left] = nums[right];
                ++left;
            }
            while(left < right && nums[left] <= partion){
                ++left;
            }
            if(left < right &&nums[right] < partion){
                nums[right] = nums[left];
                --right;
            }
        }

        int partion_pose = left;
        nums[partion_pose] = partion; 
        quickSort(nums,start,partion_pose-1);
        quickSort(nums,partion_pose+1,end);
    }


    vector<int> sortArray(vector<int>& nums) {
        quickSort(nums,0,nums.size()-1);
        return nums;
    }
};


/*
改进计算part的部分
并不需要双指针向内收缩，实际上这个这是一个类似荷兰三色问题

定义最右侧为目标值
写一个前推的pose 在遍历的时候使用双指针 将小于partion的pose位置其交换 保证从left - pose-1是小于partion的

最后将right的值和pose的值交换 使得pose+1 - right都是大于等于partion的

使用双指针只用了一个循环就实现了分区
*/
class Solution {
public:
    void quickSort(vector<int>&nums,int left,int right){
        if(left >= right){return;}
        int partion = nums[right];
        int start = left;
        int end = right;

        int pose = left;
        for(int i=start;i<end;++i){
            if(nums[i] < partion){
                swap(nums[i],nums[pose]);
                ++pose;
            }
        }
        swap(nums[right],nums[pose]);
        quickSort(nums,start,pose-1);
        quickSort(nums,pose+1,end);
    }


    vector<int> sortArray(vector<int>& nums) {
        quickSort(nums,0,nums.size()-1);
        return nums;
    }
};

/*
再次改进 
对于主元的选取，除了选取最右侧的值 还可以使用随机数的方式 随机选取一个主元
*/
class Solution {
public:
    int cal_partion(vector<int>&nums,int left,int right){
        int partion = nums[right];
        int start = left;
        int end = right;

        int pose = left;
        for(int i=start;i<end;++i){
            if(nums[i] < partion){
                swap(nums[i],nums[pose]);
                ++pose;
            }
        }
        swap(nums[right],nums[pose]);
        return pose;
    }

    int random_partion(vector<int>&nums,int left,int right){
        int i= rand() % (right - left + 1) + left;
        swap(nums[i],nums[right]);
        return cal_partion(nums,left,right);
    }
    void quickSort(vector<int>&nums,int left,int right){
        if(left > right){return;}
        int pose = random_partion(nums,left,right);
        quickSort(nums,left,pose-1);
        quickSort(nums,pose+1,right);
    }
    vector<int> sortArray(vector<int>& nums) {
        srand((unsigned)time(NULL));
        quickSort(nums,0,nums.size()-1);
        return nums;
    }
};


