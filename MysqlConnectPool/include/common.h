//
// Created by dai on 2024/6/5.
//

#ifndef MYSQLCONNPOOL_COMMON_H
#define MYSQLCONNPOOL_COMMON_H

#include <iostream>
#include <string>

#include <stdexcept>
class MyCustomException : public std::runtime_error {
public:
    MyCustomException(const std::string& what_arg)
            : std::runtime_error(what_arg) {}
};

class MysqlConnectPoolDefine{
private:
    std::string m_ip;// 数据库服务器ip地址
    std::string m_user;// 数据库服务器用户名
    std::string m_passwd;// 数据库服务器密码
    std::string m_dbName;// 数据库服务器的数据库名
    unsigned short m_port;// 数据库服务器绑定的端口
    int m_minSize;// 连接池维护的最小连接数
    int m_maxSize;// 连接池维护的最大连接数
    int m_timeout;// 连接池获取连接的超时时长
    int m_maxIdleTime;// 连接池中连接的最大空闲时长
public:
    MysqlConnectPoolDefine(std::string ip,std::string user,std::string password,
                    std::string dbName,unsigned short port,int minSize,int maxSize,
                    int timeout,int maxIdleTime):
                    m_ip(ip),m_user(user),m_passwd(password),m_dbName(dbName),m_port(port),
                    m_minSize(minSize),m_maxSize(maxSize),m_timeout(timeout),
                    m_maxIdleTime(m_maxIdleTime)
    {
    }
    ~MysqlConnectPoolDefine(){

    }
    inline void print()const{
        std::cout << "m_ip : " << m_ip
                << "m_user : " << m_user
                << "m_passwd : " << m_passwd
                << "m_dbName : " << m_dbName
                << "m_port : " << m_port
                << "m_minSize : " << m_minSize
                << "m_maxSize : " << m_maxSize
                << "m_timeout : " << m_timeout
                << "m_maxIdleTime : " << m_maxIdleTime
                << std::endl;
    }
    inline std::string get_ip() const {return m_ip;}
    inline std::string get_user() const {return m_user;}
    inline std::string get_passwd() const {return m_passwd;}
    inline std::string get_dbName() const {return m_dbName;}
    inline unsigned short get_port() const {return m_port;}
    inline int get_minSize() const {return m_minSize;}
    inline int get_maxSize() const {return m_maxSize;}
    inline int get_timeout() const {return m_timeout;}
    inline int get_maxIdleTime() const {return m_maxIdleTime;}
};




#endif //MYSQLCONNPOOL_COMMON_H
