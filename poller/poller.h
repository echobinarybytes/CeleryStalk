#ifndef _H_Poller_H_
#define _H_Poller_H_

#include <set>
#include <vector>
#include <memory>
class Channel;
class EventLoop;

class Poller {
public:
    Poller(std::shared_ptr<EventLoop> eventloop);
    // non-copyable
    Poller(const Poller&) = delete;
    Poller& operator=(const Poller&) = delete;
    virtual ~Poller(); // 不加的话可能不会调用子类的析构函数，造成资源泄漏

public:
    // 多路复用 select/poll/epoll
    virtual void Poll(std::vector<Channel*>& active_channels, long wait_time) = 0;

    // 更新或者添加channel
    // 判断是否为添加 -> 怎么判断-> 对比_channels中，看有没有channel -> 通过文件描述符
    // 一个描述符对应一个channel->哪里创建channel 
    virtual void UpdateChannel(Channel& channel) = 0;
    // 移除channel, 如何从vector中高效的移除channel
    virtual void RemoveChannel(Channel& channel) = 0;

    static std::shared_ptr<Poller> NewDefaultPoller(std::shared_ptr<EventLoop> eventloop);
private:
    std::shared_ptr<EventLoop> _eventloop;
};

#endif
