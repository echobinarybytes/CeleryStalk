#ifndef _H_EVENT_LOOP_H_
#define _H_EVENT_LOOP_H_

#include <thread>
#include <memory>
#include "../poller/poller.h"
#include <mutex>


// one-loop-per-thread:w
class EventLoop {
public:
    EventLoop();
    // non-copyable
    EventLoop(const EventLoop&) = delete;
    EventLoop& operator=(const EventLoop&) = delete;

    ~EventLoop();

public:
    void SetPoller(std::shared_ptr<EventLoop> eventloop);
    // loop必须得在创建EventLoop的线程中调用
    void Loop(); 
    // 停止loop, 在所有线程中都可以调用
    void StopLoop();

public:
    // 从poller监听的channels中移除channel
    void RemoveChannel(Channel& channel);

    // 更新或者添加, 更新poller中监听的channel
    void UpdateChannel(Channel& channel);

private:
    void AssertInLoopThread();

    bool IsInLoopThread() const;
    static EventLoop* GetEventLoopOfCurrentThread(); // 本线程的event_loop
    void AbortNotInLoopThread();
private:
    bool _looping; // 是否在运行
    const std::thread::id _threadid; // 存储eventloop对象对应的线程，构造时设置

    std::shared_ptr<Poller> _poller; // event-loop中的poller
    long _poll_time_ms;
    std::vector<Channel*> _active_channels; // 活跃的channel
    std::mutex _mtx;
};
#endif
