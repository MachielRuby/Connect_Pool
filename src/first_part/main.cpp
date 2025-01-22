#include<iostream>
using namespace std;
#include "Connection.hpp"
#include<string>

int main()
{
    // 假设 Connection 是一个已经建立的数据库连接类
    Connection conn;
    
    // SQL 插入语句
    string sql =  "INSERT INTO user (id, name, password, state) VALUES (18, 'zz', 'lanzi', 'offline')";
    conn.connect("127.0.0.1",3306, "root", "lanzi", "chat");
    // 执行插入语句
    if (conn.update(sql)) {
        cout << "Data inserted successfully!" << endl;
    } else {
        cout << "Failed to insert data." << endl;
    }

    return 0;
}