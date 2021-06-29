#ifndef _H_EVENT_LOOP_THREAD_POOL_H_
#define _H_EVENT_LOOP_THREAD_POOL_H_

#include <memory>
#include <vector>
#include <thread>

class EventLoop;
class EventLoopThreadPool {
public:
    EventLoopThreadPool();
    EventLoopThreadPool(const EventLoopThreadPool&) = delete;
    EventLoopThreadPool& operator=(const EventLoopThreadPool&) = delete;
    ~EventLoopThreadPool();

public:
    void StartEventLoopThreadPool(int num); // 开启包含num个线程的eventloop thread pool
    void Start(); // thread的入口函数
    std::shared_ptr<EventLoop> Next(); // 轮询时获得下一个eventloop
    int GetNextId() const { return _next; }

private:
    std::vector<std::thread> _thread_pool;
    std::vector<std::shared_ptr<EventLoop>> _loop_pool;
    int _next;
    int _numthreads;
};
#endif
