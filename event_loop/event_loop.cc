#include "event_loop.h"
#include <assert.h>
#include <sys/poll.h>
#include <iostream>
#include "../channel/channel.h"

// 每个线程有一个局部变量
__thread EventLoop* _loopinthisthread = nullptr;

EventLoop::EventLoop() 
    : _looping(false),
    _threadid(std::this_thread::get_id()), 
    _poll_time_ms(1000),
    _active_channels() {

    if (_loopinthisthread) {
        // LOG
        std::cout << "error" << std::endl;
        exit(-1);
    } else {
        _loopinthisthread = this;
    }
}

EventLoop::~EventLoop() {
    std::cout << "end of event loop : " << _threadid << std::endl;
    _loopinthisthread = nullptr;
}

// 设置poller，必须在定义eventloop之后调用
void EventLoop::SetPoller(std::shared_ptr<EventLoop> eventloop) {
    _poller = Poller::NewDefaultPoller(eventloop);
}


// loop必须得在创建EventLoop的线程中调用
// 不能跨线程调用
void EventLoop::Loop() {
    assert(!_looping);
    AssertInLoopThread();

    _looping = true;

    while (_looping) {
        _poller->Poll(_active_channels, _poll_time_ms); // poller

        // 处理发生的事件
        for (auto& channel : _active_channels) {
            channel->HandleEvent();
        }
        _active_channels.clear();
    }

    // LOG
    _looping = false;
}


// 停止loop, 在所有线程中都可以调用
void EventLoop::StopLoop() {
    _looping = false;
}

// 从poller监听的channels中移除channel
void EventLoop::RemoveChannel(Channel& channel) {
    std::lock_guard<std::mutex> lg(_mtx);
    _poller->RemoveChannel(channel);
}

// 更新或者添加, 更新poller中监听的channel
void EventLoop::UpdateChannel(Channel& channel) {
    std::lock_guard<std::mutex> lg(_mtx);
    _poller->UpdateChannel(channel);
}

void EventLoop::AssertInLoopThread() {
    if (!IsInLoopThread()) {
        AbortNotInLoopThread();
    }
}

bool EventLoop::IsInLoopThread() const { 
    return _threadid == std::this_thread::get_id(); 
}


// 本线程的event_loop
EventLoop* EventLoop::GetEventLoopOfCurrentThread()  {
    return _loopinthisthread;
}

void EventLoop::AbortNotInLoopThread() {
    // LOG
    std::cout << "error" << std::endl;
    exit(-1);
}

