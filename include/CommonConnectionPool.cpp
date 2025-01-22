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
    thread produce(std::bind(&ConnectionPool::produceConnectionTask, this));


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



shared_ptr<Connection>ConnectionPool::getConnection() //从连接池中获取一个可用连接
{
    unique_lock<mutex> lock(_queMutex);
    if(_connectionQue.empty())
    {
        //sleep
        if(cv_status::timeout == _cv.wait_for(lock, chrono::milliseconds(_connectionTimeout)))
        {
            if(_connectionQue.empty())
            {
                LOG("get connection timeout");
                return nullptr;
            }
        }

    }
    /*
    shared_ptr智能指针析构会把connection资源delete
    需要自定义释放的方式 这样子就不会delete了
    */
    shared_ptr<Connection> conn(_connectionQue.front(),[&](Connection* pcon)
    {
        unique_lock<mutex> lock(_queMutex);
        _connectionQue.push(pcon);
    });
    _connectionQue.pop();
    _cv.notify_all(); //通知生产者线程
    return conn;
}


void ConnectionPool::produceConnectionTask()
{
    for(;;)
    {
        std::unique_lock<std::mutex> lock(_queMutex);

        while(!_connectionQue.empty())
        {
            _cv.wait(lock); //队列不为空，等待消费
        }
        //没有到达上线就可以继续创建
        if(_connectionCnt<_maxSize)
        {
            Connection* conn = new Connection();
            conn->connect(_ip, _port, _username, _password, _dbname);
            _connectionQue.push(conn);
            _connectionCnt++;
        }
        _cv.notify_all(); //唤醒一个消费者线程
    }
}
