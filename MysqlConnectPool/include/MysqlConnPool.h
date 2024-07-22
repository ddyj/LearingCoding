//
// Created by dai on 2024/6/5.
//

#ifndef MYSQLCONNPOOL_MYSQLCONNPOOL_H
#define MYSQLCONNPOOL_MYSQLCONNPOOL_H

#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>
#include "MysqlConn.h"
#include "common.h"
#include <thread>
#include <atomic>


using namespace std;
class ConnectionPool
{
public:
    static ConnectionPool* getConnectPool(MysqlConnectPoolDefine&& mcpData); // 创建单例模式
    ConnectionPool(const ConnectionPool& obj) = delete; //禁止使用拷贝构造
    ConnectionPool& operator=(const ConnectionPool obj) = delete; // 禁止使用拷贝赋值运算符
    shared_ptr<MysqlConn> getConnection();
    void stop();
    ~ConnectionPool();
private:
    ConnectionPool(MysqlConnectPoolDefine&& mcpData); //构造函数设置为私有的，单例模式
    void produceConnection();
    void recycleConnection();
    void addConnection();

    string m_ip;// 数据库服务器ip地址
    string m_user;// 数据库服务器用户名
    string m_passwd;// 数据库服务器密码
    string m_dbName;// 数据库服务器的数据库名
    unsigned short m_port;// 数据库服务器绑定的端口
    int m_minSize;// 连接池维护的最小连接数
    int m_maxSize;// 连接池维护的最大连接数
    int m_timeout;// 连接池获取连接的超时时长
    int m_maxIdleTime;// 连接池中连接的最大空闲时长


    queue<MysqlConn*> m_connectionQ; // 连接队列
    mutex m_mutexQ;
    condition_variable m_cond;
    atomic_bool is_stop;
    std::atomic<int> working_threads;
};





#endif //MYSQLCONNPOOL_MYSQLCONNPOOL_H
