//
// Created by dai on 2024/6/5.
//

#include <iostream>

#include <cstdlib>
#include <ctime>
#include <random>
#include <iomanip>
#include <sstream>

#include "MysqlConnPool.h"

//多线程访问数据库连接池中的锁
std::mutex mtx;
atomic_bool is_stop(false);

/*
 * localhost dai 0
 * connectPool
 * 表明 test1   表结构:主键id 、 随机数、随机数构成asii码构成字符串、时间
 * */

int get_age(){
    int min = 18;
    int max = 100;
    return min + rand() % (max - min + 1);
}

string get_char(){
    string str(3,0);
    str[0] = 'a' + rand() % 26;
    str[1] = 'a' + rand() % 26;
    str[2] = 'a' + rand() % 26;
    return str;
}
std::string getCurrentTime() {
    std::time_t now = std::time(nullptr);
    std::tm* now_tm = std::localtime(&now);
    std::ostringstream oss;
    oss << std::put_time(now_tm, "%H:%M:%S");
    return oss.str();
}

void test_1(){
    clock_t begin = clock();

    int minSize = 20;
    int maxSize = 1024;
    int timeout = 60; //s
    int maxIdleTime = 100; //ms

    MysqlConnectPoolDefine connenct_message("127.0.0.1","dai","0","connectPool",3306,
                                            minSize,maxSize,timeout,maxIdleTime);
    ConnectionPool* pool = ConnectionPool::getConnectPool(std::move(connenct_message));

    srand(static_cast<unsigned int>(time(nullptr)));

    for(int i=0;i<10000;++i){
        shared_ptr<MysqlConn>c_ptr = pool->getConnection();

        do{
            c_ptr = pool->getConnection();
        }while(c_ptr==nullptr);

        int random_age = get_age();
        std::string random_name = get_char();
        std::string currentTime = getCurrentTime();

        std::string insert_command = "insert into test1(age,name,time) values("
                                     + std::to_string(random_age) + ",'"
                                     + random_name + "','"
                                     + currentTime + "')";
        std::cout << insert_command << std::endl;
        if(!c_ptr->update(insert_command)){
            c_ptr->rollback();
        }
        else{
            c_ptr->commit();
        }

    }

    pool->stop();
    clock_t end = clock();
    cout << (double)(end - begin)/CLOCKS_PER_SEC << "s" << endl;
}


void process_sql(ConnectionPool* pool,int thread_id){
    while(!is_stop){

        //尝试获取连接
        shared_ptr<MysqlConn>c_ptr = pool->getConnection();
        do{
            c_ptr = pool->getConnection();
            std::this_thread::sleep_for(std::chrono::microseconds(500));
        }while(c_ptr==nullptr);
        unique_lock<mutex> lock(mtx);
        int process_id = rand() % (10);
        if(process_id<=3){
            int random_age = get_age();
            std::string random_name = get_char();
            std::string currentTime = getCurrentTime();

            std::string insert_command;
            insert_command = "insert into test1(age,name,time) values("
                             + std::to_string(random_age) + ",'"
                             + random_name + "','"
                             + currentTime + "')";
            std::cout << "thread id :" <<thread_id <<" process insert name is : " <<random_name<< std::endl;
            if(!c_ptr->update(insert_command)){
                c_ptr->rollback();
            }
            else{
                c_ptr->commit();
            }
            std::this_thread::sleep_for(std::chrono::microseconds(200));
        }
        else if(process_id >= 6 && process_id <=10){
            std::string select_command = "select id,name,age,time from test1 where name like 'b__'";
            if(!c_ptr->query(select_command)){
                c_ptr->rollback();
            }
            else{
                c_ptr->commit();
                while(c_ptr->next()){
                    std::cout << "thread id :" <<thread_id <<" process select : "<<c_ptr->value(1) << std::endl;
                }
            }
            std::this_thread::sleep_for(std::chrono::microseconds(100));
        }
        else{
            std::string insert_command;
            insert_command = "delete from test1 where name like 'b__' or age >= 65";
            std::cout << "thread id :" <<thread_id <<" process delete name : " << std::endl;
            if(!c_ptr->update(insert_command)){
                c_ptr->rollback();
            }
            else{
                c_ptr->commit();
            }
            std::this_thread::sleep_for(std::chrono::microseconds(200));
        }
    }
}



int main(){
    int minSize = 10;
    int maxSize = 1024;
    int timeout = 60; //s
    int maxIdleTime = 100; //ms
    //随机数种子
    srand(static_cast<unsigned int>(time(nullptr)));
    MysqlConnectPoolDefine connenct_message("127.0.0.1","dai","0","connectPool",3306,
                                            minSize,maxSize,timeout,maxIdleTime);
    ConnectionPool* pool = ConnectionPool::getConnectPool(std::move(connenct_message));

    std::thread t1(process_sql,pool,1);
    std::thread t2(process_sql,pool,2);
    t1.join();
    t2.join();
//    test_1();
    return 0;
}