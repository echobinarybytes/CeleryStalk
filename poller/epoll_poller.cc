#include "epoll_poller.h"
#include "../channel/channel.h"
#include <sys/epoll.h>
#include <unistd.h>
#include <iostream>

EpollPoller::EpollPoller(std::shared_ptr<EventLoop> eventloop) : Poller(eventloop), _revents(10) {
    _poller_id = epoll_create(1);
}

EpollPoller::~EpollPoller() {
    close(_poller_id);
}

void EpollPoller::Poll(std::vector<Channel*>& active_channels, long wait_time) {
    // 发生变化的文件描述符信息填入到_revents中
    // 文件描述符信息是什么？
    // 通过epoll_ctl设置的一些信息
    int num_events = epoll_wait(_poller_id, _revents.data(), static_cast<int>(_revents.size()), wait_time);
    if (num_events > 0) {
        // 将_revents中的数据保存在channels中
        SaveToChannel(num_events, active_channels);

        // 当事件数和存储事件的的数组大小相同，扩大存储事件的数组大小
        if (static_cast<int>(_revents.size()) == num_events) {
            _revents.resize(_revents.size() * 2);
        }

    } else if (num_events == 0) {
        // LOG_TRACE
        // std::cout << "nothing happend" << std::endl;
    } else {
        // LOG_SYSERR
        std::cout << "epoll_wait error" << std::endl;
    }
}

// 多线程中如何表现？
void EpollPoller::UpdateChannel(Channel& channel) {
    // 如果没有监听channel,添加
    // 如何快速判断一个channel有没有被监听，使用能够快速查找的数据结构, 使用set, map -> 只有一个元素，使用set
    if (_channels.find(&channel) == _channels.end()) {
        Epoll_ctl(EPOLL_CTL_ADD, channel);
        _channels.insert(&channel);
    }
    // 如果监听了channel, 更改
    Epoll_ctl(EPOLL_CTL_MOD, channel);

}

void EpollPoller::RemoveChannel(Channel& channel) {
    // 删除epoll的channel监听数组中的channel
    // 管理数组中应该存放的是channel的指针而不是智能指针，因为多个智能指针可能管理的是同一个channel
    // 如果只有channel*作为数组元素，那么channel-A被删除时，channel-B或得了channel-B的空间，意味着channel-A已经free了，则channel-A的空间比不与channel-B相同, 故只需要channel*就够了
    _channels.erase(&channel);
    Epoll_ctl(EPOLL_CTL_DEL, channel);
}

void EpollPoller::Epoll_ctl(int operation, Channel& channel) {
    struct epoll_event event;
    event.data.ptr = &channel;
    event.events = channel.GetEvents();
    int fd = channel.GetFd();
    // 这里传入event的作用, 用于EPOLL_CTL_ADD, EPOLL_CTL_MOD
    if (epoll_ctl(_poller_id, operation, fd, &event) == -1) {
        // SYS_ERROR
        // 错误处理
    }

}

void EpollPoller::SaveToChannel(int num_events, std::vector<Channel*>& active_channel) {
    active_channel.clear();
    for (int i = 0; i < num_events; ++ i) {
        // ptr里面存放的是Channel* a)
        // 放到active
        // 这里要不要添加引用，在新建一个shared_ptr变量 ?
        // 目前的处理方法是新建一个变量
        Channel* cur_channel = reinterpret_cast<Channel*>(_revents[i].data.ptr);

        cur_channel->SetRevents(_revents[i].events);
        active_channel.push_back(cur_channel);
    }
}
