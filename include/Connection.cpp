#include "Connection.hpp"
#include "public.hpp"
#include<iostream>
using namespace std;
//初始化数据库
// 初始化数据库
Connection::Connection()
{
    _conn = mysql_init(nullptr);
}

//释放资源
Connection::~Connection()
{
    if(!_conn)
    {
        mysql_close(_conn);
    }
}
//连接数据库
bool Connection::connect(string ip,unsigned short port,string user,string pwd,string dbname)
{
    MYSQL*p = mysql_real_connect(_conn,ip.c_str(),user.c_str(),pwd.c_str(),dbname.c_str(),port,nullptr,0);
    if(p == nullptr)
    {
        return false;
    }
    return true;
}
//更新数据库
bool Connection::update(string sql)
{
    if(mysql_query(_conn,sql.c_str()))
    {
        LOG("更新失败："+sql);
        return false;
    }
}
//查询数据库 select
MYSQL_RES* Connection::query(string sql)
{
    if(mysql_query(_conn,sql.c_str()))
    {
        LOG("查询失败："+sql);
        return nullptr;
    }
    return mysql_use_result(_conn);
}