//
// Created by dai on 2024/6/5.
//

#include "MysqlConnPool.h"


ConnectionPool* ConnectionPool::getConnectPool(MysqlConnectPoolDefine&& mcpData)
{
    static ConnectionPool pool(std::move(mcpData));
    return &pool;
}

ConnectionPool::ConnectionPool(MysqlConnectPoolDefine&& mcpData)
{
    m_ip = mcpData.get_ip();
    m_user = mcpData.get_user();
    m_passwd = mcpData.get_passwd();
    m_dbName = mcpData.get_dbName();
    m_port = mcpData.get_port();
    m_minSize = mcpData.get_minSize();
    m_maxSize = mcpData.get_maxSize();
    m_timeout = mcpData.get_timeout();
    m_maxIdleTime = mcpData.get_maxIdleTime();
    is_stop = false;
    working_threads.store(2);
    for (int i = 0; i < m_minSize; ++i) // 默认情况保证有minsize个数的连接就行了。
    {
        addConnection();
    }
    thread producer(&ConnectionPool::produceConnection, this);// 生产连接
    thread recycler(&ConnectionPool::recycleConnection, this);// 看有没有需要销毁的连接
    producer.detach();
    recycler.detach();
}

// 生产连接线程函数
void ConnectionPool::produceConnection()
{
    while (!is_stop)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        unique_lock<mutex> locker(m_mutexQ); // 锁
        while (!is_stop || m_connectionQ.size() >= m_minSize)
        {
            m_cond.wait(locker); // 条件变量
        }
        addConnection(); // 执行连接
        //m_cond.notify_all(); // 通知消费者线程消费
        m_cond.notify_one(); //修改为通知一个线程执行
    }
    working_threads.fetch_sub(1);
}


// 销毁连接
void ConnectionPool::recycleConnection()
{
    while (!is_stop)
    {
        this_thread::sleep_for(chrono::milliseconds(500));
        unique_lock<mutex> locker(m_mutexQ);
        while (!is_stop && m_connectionQ.size() > m_minSize)
        {
            MysqlConn* front_ptr = m_connectionQ.front();
            if (front_ptr == nullptr || front_ptr->getAliveTime() >= m_maxIdleTime) // 判断连接存活时长，如果满足条件就行删除
            {
                m_connectionQ.pop();
                if(front_ptr!= nullptr){
                    delete front_ptr;
                }
                m_cond.notify_one();
            }
            else
            {
                break;
            }
        }
    }
    working_threads.fetch_sub(1);
}

void ConnectionPool::addConnection()
{
    MysqlConn* conn = new MysqlConn;
    bool is_connection = false;
    do{
        is_connection = conn->connect(m_user, m_passwd,
                m_dbName, m_ip, m_port);
    }while(!is_connection);

    conn->transaction(false);
    conn->refreshAliveTime();
    m_connectionQ.push(conn);
}

// 取出可用连接
shared_ptr<MysqlConn> ConnectionPool::getConnection()
{
    unique_lock<mutex> locker(m_mutexQ);
    while (m_connectionQ.empty()) // 判断连接是否为空
    {
        // 如果等待时间超过一个连接的生存周期 就需要判断当前队列是不是没有元素了
        if (cv_status::timeout == m_cond.wait_for(locker, chrono::milliseconds(m_timeout)))// 阻塞一段时间
        {
            if (m_connectionQ.empty())
            {
                return nullptr;
            }
        }
    }
    // lambda表达式定义shared_ptr的销毁函数。
    shared_ptr<MysqlConn> connptr(m_connectionQ.front(), [this](MysqlConn* conn) {
        unique_lock<mutex> locker(m_mutexQ);
        conn->refreshAliveTime();
        conn->transaction(false);
        m_connectionQ.push(conn);
    });
    m_connectionQ.pop();
    m_cond.notify_one();// 唤醒生产者线程，附带着也会唤醒消费者线程
    return connptr;
}
void ConnectionPool::stop(){
    is_stop = true;
    m_cond.notify_all();
    //等待两个分离线程回收
    while(working_threads.load()!=0){continue;}
    unique_lock<mutex> locker(m_mutexQ);
    while (!m_connectionQ.empty())
    {
        MysqlConn* conn = m_connectionQ.front();
        m_connectionQ.pop();
        delete conn;
    }
}
ConnectionPool::~ConnectionPool()
{
    std::cout <<"Bye" << std::endl;
}



