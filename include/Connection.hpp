#ifndef CONNECTION_HPP
#define CONNECTION_HPP
#include<mysql/mysql.h>
#include<string>
#include<ctime>
#include<memory>
using namespace std;


/*实现数据库的操作*/
class Connection
{
    public:
    //初始化数据库
    Connection();
    //释放资源
    ~Connection();
    //连接数据库
    bool connect(string ip,unsigned short port,string user,string pwd,string dbname);
    //更新数据库
    bool update(string sql);
    //查询数据库 select
    MYSQL_RES* query(string sql);
    //刷新空闲时间点
    void refreshAliveTime(){_aliveTime=clock();}
    //返回存活时间
    clock_t getAliveTime(){return clock()- _aliveTime;}
    private:
    MYSQL* _conn;
    clock_t _aliveTime; //记录进入空闲状态后的起始存货时间

};
#endif
