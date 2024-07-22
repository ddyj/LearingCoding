#include <iostream>

#include "Connection.h"
#include "CommonConnectionPool.h"

using namespace std;

/*
CREATE DATABASE test;
USE test;
CREATE TABLE user (
    id INT PRIMARY KEY AUTO_INCREMENT,
    name VARCHAR(50),
    age INT,
    sex VARCHAR(10)
);
show tables;
//  查看表结构
desc user;

// 清空表数据
delete from user;
*/

int main(){
    clock_t begin = clock();
    for (int i = 0; i < 10000; ++i)
	{
        
		// Connection conn;
		// char sql[1024] = { 0 };
		// sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')",
		// 	"zhang san", 20, "male");
		// conn.connect("127.0.0.1", 3306, "root", "123456", "test");
		// conn.update(sql);

        ConnectionPool* cp = ConnectionPool::getConnectionPool();
		shared_ptr<Connection> sp = cp->getConnection();
		if(sp!=nullptr){
            char sql[1024] = { 0 };
            sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')",
                    "zhang san", 20, "male");
            sp->update(sql);
		}
	}

    ConnectionPool* cp = ConnectionPool::getConnectionPool();
    cp->stop();

    clock_t end = clock();
	cout << (double)(end - begin)/CLOCKS_PER_SEC << "s" << endl;
    return 0;
}