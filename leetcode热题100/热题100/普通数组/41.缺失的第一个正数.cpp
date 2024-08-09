#include "util.h"

/*

难点 -- 时间复杂度O(N) 空间复杂度O(1)

方案1 -- 不考虑空间复杂度限制 此时构建一个哈希表
然后从0-N依次查询,直到查询到第一个没有的元素
不符合空间复杂度O(1)

方案2 -- 不考虑时间复杂度，对数据排序，
在有序数组中查找
不符合时间复杂度O(N)

方案3 -- 要找的数实在[1,N+1]的区间中，
我们希望将1放到0的下标，
将2放到下标为1的位置上，
第一个缺失的数就是我们要查找的

重点:如何构建这个数组
遍历 将num[i] 和 nums[nums[i]-1]交换 
在交换后还需要继续分析交换过来的数nums[nums[i]-1]会否符合下标i
这时一个循环
退出循环的方式有两个 要么这个数不在[1,N+1]区间
要么已经符合下标
*/

class Solution {
public:
    int firstMissingPositive(vector<int>& nums) {
        int len = nums.size();
        for(int i=0;i<len;++i){
            //这里有一个额外补充 此时需要判断两个交换的值是否是一样的，如果是一样的就会出现死循环
            while(  (nums[i] >=1 && nums[i] <= len) &&  
                                    nums[i]!=i+1 && 
                                        nums[i]!=nums[nums[i]-1]){
                swap(nums[i],nums[nums[i]-1]);
            }
        }
        int ans;
        for(int i=0;i<len;++i){
            if(nums[i]!=i+1){
                return i+1;
            }
        }
        return len+1;
    }
};