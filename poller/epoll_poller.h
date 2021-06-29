#ifndef _H_EPOLL_POLLER_H
#define _H_EPOLL_POLLER_H

#include "poller.h"

class EpollPoller : public Poller{
public:
    EpollPoller(std::shared_ptr<EventLoop> eventloop);
    // non-copyable
    EpollPoller(const EpollPoller&) = delete;
    EpollPoller& operator=(const EpollPoller&) = delete;
    ~EpollPoller();

public:
    // 多路复用 epoll
    virtual void Poll(std::vector<Channel*>& active_channels, long wait_time) override;

    // 更新或者添加channel
    // 判断是否为添加 -> 怎么判断-> 对比_channels中，看有没有channel -> 通过文件描述符
    // 一个描述符对应一个channel->哪里创建channel 
    virtual void UpdateChannel(Channel& channel) override;
    // 移除channel, 如何从vector中高效的移除channel
    virtual void RemoveChannel(Channel& channel) override;

private:
    void Epoll_ctl(int operation, Channel& channel);
    void SaveToChannel(int numEvents, std::vector<Channel*>& active_channels);
    int _poller_id; // poller描述符号
    std::vector<struct epoll_event> _revents; // 发生的事件
    std::set<Channel*> _channels; // 该poller所有监听的channel
};
#endif
