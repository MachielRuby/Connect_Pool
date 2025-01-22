#ifndef COMMONCONNECTIONPOOL_HPP
#define COMMONCONNECTIONPOOL_HPP

#include<string>
#include<queue>
#include<iostream>
#include<thread>
#include "Connection.hpp"
using namespace std;
#include<mutex>
#include<atomic>
#include<memory>
#include<functional>
#include<condition_variable>
/*
实现连接池模块
*/
class ConnectionPool
{
    public:
    static ConnectionPool* getInstance(); //2 提供获取单例对象的接口
    shared_ptr<Connection>getConnection(); //从连接池中获取一个可用连接
    private:
    //加载配置项
    bool loadConfigFile();
    ConnectionPool();//1 私化构造函数
    ConnectionPool(const ConnectionPool&) = delete; //3 私化拷贝构造函数
    ConnectionPool& operator=(const ConnectionPool&) = delete; //4 私化赋值运算符
    //运行独立的线程负责新连接
    void produceConnectionTask();

    private:
    string _ip;
    unsigned short _port;
    string _username;
    string _password;
    string _dbname;
    int _initSize;
    int _maxSize;
    int _maxIdleTime;
    int _connectionTimeout;

    queue<Connection*>_connectionQue; //存储mysql连接的队列
    mutex _queMutex; //保护队列的互斥锁
    atomic_int _connectionCnt; //当前连接池中连接的数量
    condition_variable _cv; //条件变量用于连接生产线程和消费线程的通信
    public:
    void print()
    {
        cout << _ip << _port << _username << _password << _initSize << _maxSize << _maxIdleTime << _connectionTimeout << endl;
    }
    
};

#endif
