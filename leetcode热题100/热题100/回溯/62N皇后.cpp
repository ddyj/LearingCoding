#include "util.h"


class Solution {
public:
    bool is_vaild(vector<string>cur_res,int row,int col,int len){
        //检查列
        for(int i=0;i<row;i++){
            if(cur_res[i][col] == 'Q'){
                return false;
            }
        }
        //检查斜侧角度
        //左下角
        for(int i = row - 1, j = col - 1; i >= 0 && j >= 0; i--, j--) {
            if(cur_res[i][j] == 'Q'){
                return false;
            }
        }
        //右下角
        for(int i = row - 1, j = col + 1; i >= 0 && j >= 0; i--, j++) {
            if(cur_res[i][j] == 'Q'){
                return false;
            }
        }
        return true;
    }

    void back_word_NQueens(vector<vector<string>>&res, vector<string>&cur_res,
                            int cur_id,int len){
 
        if(cur_id == len){
            res.emplace_back(cur_res);
            return;
        }
        for(int i=0;i<len;i++){
            if(is_vaild(cur_res,cur_id,i,len)){    
                cur_res[cur_id][i] = 'Q';
                back_word_NQueens(res,cur_res,cur_id+1,len);
                cur_res[cur_id][i] = '.';
            }
        }
    }

    vector<vector<string>> solveNQueens(int n) {
        vector<vector<string>>res;
        if(n == 1){
            res.emplace_back(vector<string>(1,"Q"));
            return res;
        }
        vector<string>cur_res(n,string(n,'.'));
        back_word_NQueens(res,cur_res,0,n);
        return res;
    }
};


//同质方法：
class Solution {
public:
    bool is_vaild(vector<string>cur_res,int row,int col,int len){
        //检查列
        for(int i=0;i<row;i++){
            if(cur_res[i][col] == 'Q'){
                return false;
            }
        }

        //检查斜侧角度
        //左下角
        for(int i = row - 1, j = col - 1; i >= 0 && j >= 0; i--, j--) {
            if(cur_res[i][j] == 'Q'){
                return false;
            }
        }
        //右下角
        for(int i = row - 1, j = col + 1; i >= 0 && j >= 0; i--, j++) {
            if(cur_res[i][j] == 'Q'){
                return false;
            }
        }
        return true;
    }
    string create_str(int col,int len){
        string str;
        for(int i=0;i<len;i++){
            if(i==col){
                str += 'Q';
            }
            else{
                str+='.';
            }
        }
        return str;
    }
    void back_word_NQueens(vector<vector<string>>&res, vector<string> cur_res,
                            int cur_id,int len){
 
        if(cur_id == len){
            res.emplace_back(cur_res);
            return;
        }
        for(int i=0;i<len;i++){
            if(is_vaild(cur_res,cur_id,i,len)){
                string add_str = create_str(i,len);
                cur_res.emplace_back(add_str);
                back_word_NQueens(res,cur_res,cur_id+1,len);
                cur_res.pop_back();
            }
        }

    }



    vector<vector<string>> solveNQueens(int n) {
        vector<vector<string>>res;
        if(n == 1){
            res.emplace_back(vector<string>(1,"Q"));
            return res;
        }
        vector<string>cur_res;

        back_word_NQueens(res,cur_res,0,n);


        return res;
    }
};
