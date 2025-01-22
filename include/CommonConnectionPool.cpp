#include "CommonConnectionPool.hpp"
#include<fstream>
#include<sstream>
//线程安全的懒汉单例模式
ConnectionPool* ConnectionPool::getInstance() //2 提供获取单例对象的接口
{
    static ConnectionPool instance;
    return &instance;
}

bool ConnectionPool::loadConfigFile()
{
    std::ifstream file("mysql.ini","r");
    if(file == nullptr)
    {
        LOG("mysql.ini file is not exist!!")
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
        
    }
    return true;
}

