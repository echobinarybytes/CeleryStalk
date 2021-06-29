#ifndef _H_ASYNCLOG_H_
#define _H_ASYNCLOG_H_

#include <memory>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <thread>

enum class LogLevel {
    TRACE,
    DEBUG,
    INFO ,
    WARN ,
    ERROR,
    FATAL,
};

#define LOG_TRACE if (AsyncLog::LOGLEVEL <= LogLevel::TRACE) \
                        AsyncLog(__FILE__, __LINE__, LogLevel::TRACE)
#define LOG_DEBUG if (AsyncLog::LOGLEVEL <= LogLevel::DEBUG) \
                        AsyncLog(__FILE__, __LINE__, LogLevel::DEBUG)
#define LOG_INFO if (AsyncLog::LOGLEVEL <= LogLevel::INFO) \
                        AsyncLog(__FILE__, __LINE__, LogLevel::INFO)
#define LOG_WARN AsyncLog(__FILE__, __LINE__, LogLevel::WARN)
#define LOG_ERROR AsyncLog(__FILE__, __LINE__, LogLevel::ERROR)
#define LOG_FATAL AsyncLog(__FILE__, __LINE__, LogLevel::FATAL)


// 日志文件名
// 进程名字.创建时间.机器名称.进程id.后缀
// main.20120603-080246.uubs.3605.log
// 日志格式
// 日期     时间      微秒   线程  级别  正文  源文件名:行号
// 20120603 08:02:46.125770Z 23261 INFO Hello world - test.cc:51
class LargeBuffer; // 4MB
class AsyncLog {
public:
    AsyncLog(std::string file, int line, LogLevel mode);
    ~AsyncLog();

public:
    static void Init(int cnt); // 设置缓冲区块的多少cnt, 返回对象的引用

    AsyncLog& operator<<(std::string log_info); // 用于生产者线程,1.如何把这个函数做成原子操作

    static void Consume(); // 用于消费者线程,一个，写入磁盘

    static void Stop(); // 停止日志系统

    void TimerTask(); // 每过3秒，将_cur_buffer放到_full_buffer_poll

    static const LogLevel LOGLEVEL;
private:
    static std::mutex _producer_mtx; // 生产者线程锁
    static std::mutex _consumer_mtx; // 消费者通知锁
    static std::condition_variable _working_cond; 
    static bool _working; // 表示消费者开始工作
    static bool _running; // 表示程序正在运行, 原子类型
    static std::thread _consumer_thread;

    // 空缓冲区 <- 生产者
    // 满缓冲区 -> 消费者
    static std::unique_ptr<LargeBuffer> _cur_buffer; // 当前生产者使用的缓冲区, 存入日志 
    static std::vector<std::unique_ptr<LargeBuffer>> _empty_buffer_pool; // 当前生产者可以使用的缓冲区块, 缓冲区快的大小是cnt
    static std::vector<std::unique_ptr<LargeBuffer>> _full_buffer_pool; // 已经填满了的缓冲块 

    static std::string _pid; // 进程id
    static std::string _process_name; // 进程名字
    static std::string _machine_name; // 机器名字
    std::string _log_mode;
    std::string _fileandline;
};
#endif
