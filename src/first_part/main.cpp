#include<iostream>
using namespace std;
#include "Connection.hpp"
#include "CommonConnectionPool.hpp"
#include<string>
#include<sstream>

int main()
{

    Connection conn;
	conn.connect("127.0.0.1", 3306, "root", "lanzi", "poolconnect");

	/*Connection conn;
	char sql[1024] = { 0 };
	sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')",
		"zhang san", 20, "male");
	conn.connect("127.0.0.1", 3306, "root", "lanzi", "chat");
	conn.update(sql);*/

	clock_t begin = clock();
	
	thread t1([]() {
		ConnectionPool *cp = ConnectionPool::getInstance();
		for (int i = 0; i < 200; ++i)
		{
			/*char sql[1024] = { 0 };
			sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')",
				"zhang san", 20, "male");
			shared_ptr<Connection> sp = cp->getConnection();
			sp->update(sql);*/
			// Connection conn;
            shared_ptr<Connection> conn = cp->getConnection();
            string sql =  "INSERT INTO user (name,age,sex) VALUES ('zz',18,'male')";
			conn->update(sql);
		}
	});
	thread t2([]() {
		ConnectionPool *cp = ConnectionPool::getInstance();
		for (int i = 0; i < 200; ++i)
		{
			/*char sql[1024] = { 0 };
			sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')",
				"zhang san", 20, "male");
			shared_ptr<Connection> sp = cp->getConnection();
			sp->update(sql);*/
            shared_ptr<Connection> conn = cp->getConnection();
            string sql =  "INSERT INTO user (name,age,sex) VALUES ('zz',18,'male')";
			conn->update(sql);
		}
	});
	thread t3([]() {
		ConnectionPool *cp = ConnectionPool::getInstance();
		for (int i = 0; i < 200; ++i)
		{
			/*char sql[1024] = { 0 };
			sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')",
				"zhang san", 20, "male");
			shared_ptr<Connection> sp = cp->getConnection();
			sp->update(sql);*/
            shared_ptr<Connection> conn = cp->getConnection();
            string sql =  "INSERT INTO user (name,age,sex) VALUES ('zz',18,'male')";
			conn->update(sql);
		}
	});
	thread t4([]() {
		ConnectionPool *cp = ConnectionPool::getInstance();
		for (int i = 0; i < 200; ++i)
		{
			/*char sql[1024] = { 0 };
			sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')",
				"zhang san", 20, "male");
			shared_ptr<Connection> sp = cp->getConnection();
			sp->update(sql);*/
            shared_ptr<Connection> conn = cp->getConnection();
            string sql =  "INSERT INTO user (name,age,sex) VALUES ('zz',18,'male')";
			conn->update(sql);
		}
	});

	t1.join();
	t2.join();
	t3.join();
	t4.join();

	clock_t end = clock();
	cout << (end - begin) << "ms" << endl;
    // // 假设 Connection 是一个已经建立的数据库连接类
    // Connection conn;
    
    // // SQL 插入语句
    // string sql =  "INSERT INTO user (id, name, password, state) VALUES (18, 'zz', 'lanzi', 'offline')";
    // conn.connect("127.0.0.1",3306, "root", "lanzi", "chat");
    // // 执行插入语句
    // if (conn.update(sql)) {
    //     cout << "Data inserted successfully!" << endl;
    // } else {
    //     cout << "Failed to insert data." << endl;
    // }

    // clock_t begin = clock();
    // for(int i= 0;i<1000;i++)
    // {
    //     Connection conn;
    //     string sql =  "INSERT INTO user (name,age,sex) VALUES ('zz',18,'male')";
    //     conn.connect("127.0.0.1",3306, "root", "lanzi", "poolconnect");
    //     // 执行插入语句
    //     conn.update(sql);
    // }
    // clock_t end = clock();
    // cout << "time:" << (double)(end - begin) / CLOCKS_PER_SEC << endl;
    return 0;
}