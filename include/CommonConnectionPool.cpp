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
    else
    {
        cout << "Loaded Config: "
     << "_ip=" << _ip
     << ", _port=" << _port
     <<",_password = "<<_password
     << ", _username=" << _username
     << ", _dbname=" << _dbname
     << ", _initSize=" << _initSize
     << ", _maxSize=" << _maxSize << endl;

    }
    
    cout<<"初始化连接池"<<endl;
    //创建初始数量的连接
    for (int i = 0; i < _initSize; ++i) {
        Connection* p = new Connection();
        p->connect(_ip, _port, _username, _password, _dbname);
        p->refreshAliveTime(); //刷新一下开始空闲的起始时间
        _connectionQue.push(p);
        _connectionCnt++;
    }
	// 启动一个新的线程，作为连接的生产者 linux thread => pthread_create
	thread produce(std::bind(&ConnectionPool::produceConnectionTask, this));
	produce.detach();

	// 启动一个新的定时线程，扫描超过maxIdleTime时间的空闲连接，进行对于的连接回收
	thread scanner(std::bind(&ConnectionPool::scannerConnectionTask, this));
	scanner.detach();
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
            conn->refreshAliveTime(); //刷新一下开始空闲的起始时间
            _connectionQue.push(conn);
            _connectionCnt++;
        }
        _cv.notify_all(); //唤醒一个消费者线程
    }
}


// 扫描超过maxIdleTime时间的空闲连接，进行对于的连接回收
void ConnectionPool::scannerConnectionTask()
{
	for (;;)
	{
		// 通过sleep模拟定时效果
		this_thread::sleep_for(chrono::seconds(_maxIdleTime));

		// 扫描整个队列，释放多余的连接
		unique_lock<mutex> lock(_queMutex);
		while (_connectionCnt > _initSize)
		{
			Connection *p = _connectionQue.front();
			if (p->getAliveTime() >= (_maxIdleTime * 1000))
			{
				_connectionQue.pop();
				_connectionCnt--;
				delete p; // 调用~Connection()释放连接
			}
			else
			{
				break; // 队头的连接没有超过_maxIdleTime，其它连接肯定没有
			}
		}
	}
}