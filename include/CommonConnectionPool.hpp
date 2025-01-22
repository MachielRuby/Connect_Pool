#ifndef COMMONCONNECTIONPOOL_HPP
#define COMMONCONNECTIONPOOL_HPP

#include<string>
#include<queue>
#include<iostream>
#include "Connection.hpp"
using namespace std;
#include<mutex>
/*
实现连接池模块
*/
class ConnectionPool
{
    public:
    static ConnectionPool* getInstance(); //2 提供获取单例对象的接口
    private:
    //加载配置项
    bool loadConfigFile();
    ConnectionPool(){}//1 私化构造函数
    ConnectionPool(const ConnectionPool&) = delete; //3 私化拷贝构造函数
    ConnectionPool& operator=(const ConnectionPool&) = delete; //4 私化赋值运算符

    private:
    string _ip;
    unsigned short _port;
    string _username;
    string _password;
    int _initSize;
    int _maxSize;
    int _maxIdleTime;
    int _connectionTimeout;

    queue<Connection*>_connectionQue; //存储mysql连接的队列
    mutex _queMutex; //保护队列的互斥锁
    
};

#endif
