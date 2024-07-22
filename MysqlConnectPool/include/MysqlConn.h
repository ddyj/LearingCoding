//
// Created by dai on 2024/6/1.
//
#ifndef MYSQLCONNECTPOOL_MYSQLCONN_H
#define MYSQLCONNECTPOOL_MYSQLCONN_H
//C++ mysql connection class
#pragma once
#include <iostream>
#include <mysql/mysql.h> //mysql提供的函数接口头文件
#include <chrono>
using namespace std;
using namespace chrono;
class MysqlConn
{
public:
    // 初始化数据库连接
    MysqlConn();
    // 释放数据库连接
    ~MysqlConn();
    // 连接数据库
    bool connect(string user, string passwd, string dbName,
                        string ip, unsigned short port = 3306);
    // 更新数据库: insert, update, delete
    bool update(string sql);
    // 查询数据库
    bool query(string sql);
    // 遍历查询得到的结果集
    bool next();
    // 得到结果集中的字段值
    string value(int index);
    // 事务操作
    bool transaction(bool is_auto_commit = false);
    // 提交事务
    bool commit();
    // 事务回滚
    bool rollback();
    // 刷新起始的空闲时间点
    void refreshAliveTime();
    // 计算连接存活的总时长
    long long getAliveTime();
private:
    //如果有查询的结果集，就释放 避免内存泄露
    void freeResult();
    MYSQL* m_conn = nullptr;
    //查询的结果集
    MYSQL_RES* m_result = nullptr;
    //记录查询结果集的行内容
    MYSQL_ROW m_row = nullptr;
    steady_clock::time_point m_alivetime;// 绝对时钟判断连接存活时长
};





#endif //MYSQLCONNECTPOOL_MYSQLCONN_H
