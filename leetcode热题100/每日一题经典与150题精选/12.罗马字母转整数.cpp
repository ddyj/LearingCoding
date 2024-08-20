#include   "util.h"


/*
罗马数字包含以下七种字符： I， V， X， L，C，D 和 M。

字符          数值
I             1
V             5
X             10
L             50
C             100
D             500
M             1000
例如， 罗马数字 2 写做 II ，即为两个并列的 1。12 写做 XII ，即为 X + II 。 27 写做  XXVII, 即为 XX + V + II 。

通常情况下，罗马数字中小的数字在大的数字的右边。但也存在特例，例如 4 不写做 IIII，而是 IV。数字 1 在数字 5 的左边，所表示的数等于大数 5 减小数 1 得到的数值 4 。同样地，数字 9 表示为 IX。这个特殊的规则只适用于以下六种情况：

I 可以放在 V (5) 和 X (10) 的左边，来表示 4 和 9。
X 可以放在 L (50) 和 C (100) 的左边，来表示 40 和 90。 
C 可以放在 D (500) 和 M (1000) 的左边，来表示 400 和 900。
给你一个整数，将其转为罗马数字。

输入: num = 3
输出: "III"

*/

/*
法一 ：模拟 
    对于罗马数字，可以将其写成13种不同的组合
    我们从高位开始模拟 查找到底第一个小于当前值的元素 就是对应的罗马字母
*/
class Solution {
public:
    void init_(vector<pair<int,string>>&hash_table){
        hash_table.emplace_back(make_pair(1000,"M"));
        hash_table.emplace_back(make_pair(900,"CM"));
        hash_table.emplace_back(make_pair(500,"D"));
        hash_table.emplace_back(make_pair(400,"CD"));
        hash_table.emplace_back(make_pair(100,"C"));
        hash_table.emplace_back(make_pair(90,"XC"));
        hash_table.emplace_back(make_pair(50,"L"));
        hash_table.emplace_back(make_pair(40,"XL"));
        hash_table.emplace_back(make_pair(10,"X"));
        hash_table.emplace_back(make_pair(9,"IX"));
        hash_table.emplace_back(make_pair(5,"V"));
        hash_table.emplace_back(make_pair(4,"IV"));
        hash_table.emplace_back(make_pair(1,"I"));
    }
    string intToRoman(int num) {
        vector<pair<int,string>>hash_table;
        init_(hash_table);
        string str;
  
        for(auto &[k,m]:hash_table){
            //std::cout << k << " "<<m<<std::endl;
            while(num >= k){
                str += m;
                num -= k;
            }
            if(num == 0){
                break;
            }
        }
        return str;
    }
};

//法二 硬编码
//我们可以在num < 3999 内 对每一位数字的值进行编码，编码后程序只需要获取每一位的值就可以

class Solution {
public:

    string intToRoman(int num) {
        const string thousands[] = {"", "M", "MM", "MMM"};
        const string hundreds[]  = {"", "C", "CC", "CCC", "CD", "D", "DC", "DCC", "DCCC", "CM"};
        const string tens[]      = {"", "X", "XX", "XXX", "XL", "L", "LX", "LXX", "LXXX", "XC"};
        const string ones[]      = {"", "I", "II", "III", "IV", "V", "VI", "VII", "VIII", "IX"};

        return thousands[num/1000] + hundreds[num%1000 /100] + tens[num%100 /10] + ones[num%10];
    }
};