#include "CommonConnectionPool.hpp"
#include<fstream>
#include<sstream>
#include "public.hpp"
//线程安全的懒汉单例模式

ConnectionPool::ConnectionPool()//1 私化构造函数
{
    if (!loadConfigFile()) {
        LOG("loadConfigFile failed");
    }
    //创建初始数量的连接
    for (int i = 0; i < _initSize; ++i) {
        Connection* p = new Connection();
        p->connect(_ip, _port, _username, _password, _dbname);
        _connectionQue.push(p);
        _connectionCnt++;
    }

    //生产者线程
    
}

ConnectionPool* ConnectionPool::getInstance() //2 提供获取单例对象的接口
{
    static ConnectionPool instance;
    return &instance;
}

bool ConnectionPool::loadConfigFile()
{
    std::ifstream file("/home/lanzi/Connect_Pool/include/mysql.ini");
    if (!file.is_open()) {
        std::cerr << "Failed to open mysql.ini file" << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line))
    {
        std::istringstream sin(line.substr(line.find("=") + 1));
        if (line.find("ip") == 0)
        {
            sin >> _ip;
        }
        else if (line.find("port") == 0)
        {
            sin >> _port;
        }
        else if (line.find("username") == 0)
        {
            sin >> _username;
        }
        else if (line.find("password") == 0)
        {
            sin >> _password;
        }
        else if (line.find("dbname") == 0)
        {
            sin >> _dbname;
        }
        else if (line.find("initSize") == 0)
        {
            sin >> _initSize;
        }
        else if (line.find("maxSize") == 0)
        {
            sin >> _maxSize;
        }
        else if (line.find("maxIdleTime") == 0)
        {
            sin >> _maxIdleTime;
        }
        else if (line.find("maxConnectionTimeout") == 0)
        {
            sin >> _connectionTimeout;
        }
    }
    return true;
}

