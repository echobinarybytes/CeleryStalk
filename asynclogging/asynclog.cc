#include "asynclog.h"
#include "largebuffer.h"
#include "../timestamp/timestamp.h"
#include <unistd.h>
#include <thread>

/////////////////// LOG LEVEL //////////////////////////////////////
const LogLevel AsyncLog::LOGLEVEL = LogLevel::TRACE;   ////////////
//////////////////////////////////////////////////////////////////


std::mutex AsyncLog::_producer_mtx; // 生产者线程锁
std::mutex AsyncLog::_consumer_mtx; // 消费者通知锁
std::condition_variable AsyncLog::_working_cond; 
bool AsyncLog::_working = false; // 表示消费者开始工作
bool AsyncLog::_running = true; // 表示程序正在运行, 原子类型
std::thread AsyncLog::_consumer_thread;

std::unique_ptr<LargeBuffer> AsyncLog::_cur_buffer = nullptr; // 当前生产者使用的缓冲区, 存入日志 
std::vector<std::unique_ptr<LargeBuffer>> AsyncLog::_empty_buffer_pool = {}; // 当前生产者可以使用的缓冲区块, 缓冲区快的大小是cnt
std::vector<std::unique_ptr<LargeBuffer>> AsyncLog::_full_buffer_pool = {}; // 已经填满了的缓冲块 

std::string AsyncLog::_pid = std::to_string(static_cast<int>(getpid())); // 进程id
std::string AsyncLog::_process_name = "main"; // 进程名字
std::string AsyncLog::_machine_name = "uubs"; // 机器名字

#include <iostream>
// 设置缓冲区块的多少cnt, 返回对象的引用
void AsyncLog::Init(int cnt) {
    
    for (int i = 0; i < cnt; ++ i) {
        std::unique_ptr<LargeBuffer> tmp(new LargeBuffer());
        _empty_buffer_pool.push_back(std::move(tmp));
    }
    _cur_buffer = std::move(_empty_buffer_pool.back());
    _empty_buffer_pool.pop_back();
    
    // 初始化
    _consumer_thread = std::thread(AsyncLog::Consume);
} 

AsyncLog::AsyncLog(std::string file, int line, LogLevel mode) {
    switch(mode) {
        case LogLevel::TRACE:
            _log_mode = "TRACE";
            break;
        case LogLevel::DEBUG:
            _log_mode = "DEBUG";
            break;
        case LogLevel::INFO :
            _log_mode = "INFO";
            break;
        case LogLevel::WARN :
            _log_mode = "WARN";
            break;
        case LogLevel::ERROR:
            _log_mode = "ERROR";
            break;
        case LogLevel::FATAL:
            _log_mode = "FATAL";
            break;
    }

    _fileandline = file + ":" + std::to_string(line);
}

AsyncLog::~AsyncLog() {
}

// 需要保证线程安全
AsyncLog& AsyncLog::operator<<(std::string body) {
    std::lock_guard<std::mutex> lg(_producer_mtx); // 消费者线程锁
    // 产生log_info
    std::string log_info = TimeStamp().LogString() + " "
                        + std::to_string(std::hash<std::thread::id>{}(std::this_thread::get_id())) + " "
                        + _log_mode + " "
                        + body + " - "
                        + _fileandline + "\n";
    if (_cur_buffer.get() == nullptr) {
        std::unique_lock<std::mutex> ulock(_consumer_mtx);
        _working = true;
        _working_cond.notify_one(); // 通知消费者线程
        if (!_empty_buffer_pool.empty()) {
            _cur_buffer = std::move(_empty_buffer_pool.back()); // 先移掉
            _empty_buffer_pool.pop_back(); // 再删除
        } else {
            // 丢掉日志
            // TODO: 如何做才能不丢掉日志
            return *this;
        }

    } else if ( _cur_buffer->Avail() < log_info.size()) { // 不能够存放下log
        std::unique_lock<std::mutex> ulock(_consumer_mtx);
        _full_buffer_pool.push_back(std::move(_cur_buffer));// 放到满队列中
        _working = true;
        _working_cond.notify_one(); // 通知消费者线程
        if (!_empty_buffer_pool.empty()) {
            _cur_buffer = std::move(_empty_buffer_pool.back()); // 先移掉
            _empty_buffer_pool.pop_back(); // 再删除
        } else {
            // 丢掉日志
            // TODO: 如何做才能不丢掉日志
            return *this;
        }

    }     
    if (_cur_buffer)
        _cur_buffer->Append(log_info); // 添加到日志中
    return *this;
}

void AsyncLog::Stop() {
    _working = true;
    _working_cond.notify_one();
    _running = false;
    _consumer_thread.join();
}

// 每过3秒，将_cur_buffer放到_full_buffer_poll
// 定时任务加到eventloop_threadpoll的任务队列中
void AsyncLog::TimerTask() { // 添加到任务队列中
    // TODO

}

// 用于消费者线程
void AsyncLog::Consume() { // TODO: 解决将最后一块记录写入？？
    int i = 0;
    while (_running) {
        std::unique_lock<std::mutex> ulock(_consumer_mtx);
        _working_cond.wait(ulock, [](){return _working;});
         
        // 产生文件名字timestamp.log
        // 对满的buffer中的每一个
        while (!_full_buffer_pool.empty()) {
            std::string file_name = _process_name + "." 
                                  + std::to_string(i ++ ) + "."
                                  + TimeStamp().LogFileName() +  "."
                                  + _machine_name + "."
                                  + _pid + "."
                                  + "log";
            _full_buffer_pool.back()->StoreToFile(file_name);
            // 清空buffer
            _full_buffer_pool.back()->SetBufferZero();
            // 放回_empty_buffer_pool中
            _empty_buffer_pool.push_back(std::move(_full_buffer_pool.back()));
            _full_buffer_pool.pop_back();
        }
        _working = false;
    }
}
