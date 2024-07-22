//
// Created by dai on 2024/6/1.
//
#include "MysqlConn.h"

// 初始化数据库连接
MysqlConn::MysqlConn(){
    m_conn = mysql_init(nullptr);
    mysql_set_character_set(m_conn, "utf8"); // 设置utf-8编码格式
}
// 释放数据库连接
MysqlConn::~MysqlConn(){
    if (m_conn != nullptr)
    {
        mysql_close(m_conn); //关闭连接
    }
    freeResult(); // 释放一个结果集合使用的内存。
}
// 连接数据库
bool MysqlConn::connect(string user, string passwd,
                string dbName, string ip, unsigned short port){
    MYSQL* ptr = mysql_real_connect(m_conn, ip.c_str(), user.c_str(), passwd.c_str(), dbName.c_str(), port, nullptr, 0); //连接mysql服务器
    return ptr != nullptr;
}
// 更新数据库: insert, update, delete
bool MysqlConn::update(string sql){
    freeResult();
    if (mysql_query(m_conn, sql.c_str()))
    {
        return false;
    }
    return true;
}
// 查询数据库
bool MysqlConn::query(string sql){
    freeResult();
    if (mysql_query(m_conn, sql.c_str()))
    {
        return false;
    }
    m_result = mysql_store_result(m_conn); // 保存结果集
    return true;
}
// 遍历查询得到的结果集
bool MysqlConn::next(){
    if (m_result != nullptr)
    {
        m_row = mysql_fetch_row(m_result);
        if (m_row != nullptr)
        {
            return true;
        }
    }
    return false;
}
// 得到结果集中的字段值
// 例如 此时查询的是 id、name、values 如果输入index是1 此时返回的string就是name
string MysqlConn::value(int index){
    int rowCount = mysql_num_fields(m_result);
    if (index >= rowCount || index < 0 || m_result == nullptr)
    {
        return string();
    }
    //这里查到的是这一行数据的首地址。后面需要长度还捕获全部信息
    char* val = m_row[index];
    unsigned long length = mysql_fetch_lengths(m_result)[index];
    return string(val, length);
}
// 事务操作
bool MysqlConn::transaction(bool is_auto_commit){
    return mysql_autocommit(m_conn, is_auto_commit); //手动提交，不自动提交事务。
}
// 提交事务
bool MysqlConn::commit(){
    return mysql_commit(m_conn);
}
// 事务回滚
bool MysqlConn::rollback(){
    return mysql_rollback(m_conn);
}
// 刷新起始的空闲时间点
void MysqlConn::refreshAliveTime(){
    m_alivetime = steady_clock::now();
}
// 计算连接存活的总时长
long long MysqlConn::getAliveTime(){
    nanoseconds res = steady_clock::now() - m_alivetime;// 纳秒 较高精度
    milliseconds millsec = duration_cast<milliseconds>(res);// 毫秒 精度相对较低
    return millsec.count();
}


void MysqlConn::freeResult()
{
    if (m_result)
    {
        mysql_free_result(m_result);
        m_result = nullptr;
    }
}