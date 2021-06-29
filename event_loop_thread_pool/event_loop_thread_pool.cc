#include "event_loop_thread_pool.h"
#include "../event_loop/event_loop.h"

EventLoopThreadPool::EventLoopThreadPool() : _next(0){

}

EventLoopThreadPool::~EventLoopThreadPool() {
    for (int i = 0; i < _numthreads; ++ i) {
        _thread_pool[i].join();
    }
}

// 开启包含num个线程的eventloop thread pool
void EventLoopThreadPool::StartEventLoopThreadPool(int num) {
    _numthreads = num;
    for (int i = 0; i < num; ++ i) {
        _thread_pool.push_back(std::thread(&EventLoopThreadPool::Start, this));
    }

} 
// thread的入口函数
void EventLoopThreadPool::Start()  {
    // 初始化threadloop
    std::shared_ptr<EventLoop> eventloop = std::make_shared<EventLoop>();
    eventloop->SetPoller(eventloop);
    _loop_pool.push_back(eventloop);
    eventloop->Loop();
}

// 轮询时获得下一个eventloop
std::shared_ptr<EventLoop> EventLoopThreadPool::Next(){
    std::shared_ptr<EventLoop> ret = _loop_pool[_next];
    _next = (_next + 1) % _numthreads; // 轮询
    return ret;
}
