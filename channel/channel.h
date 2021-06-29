#ifndef _H_CHANNEL_H_
#define _H_CHANNEL_H_

#include <functional>
#include "../event_loop/event_loop.h"
#include "../function/function_typedef.h"

class Buffer;
class Channel;

class Channel {
public:
    Channel(std::shared_ptr<EventLoop> eventloop, int fd);
    // non-copyable
    Channel(const Channel&) = delete;
    Channel& operator=(const Channel&) = delete;

    // move constructor

    ~Channel();
public:
    // 处理事件
    void HandleEvent();

public:
    // 设置回调函数
    void SetReadCallBack(const ReadEventCallBack& callback);
    void SetWriteCallBack(const WriteEventCallBack& callback);
    void SetCloseCallBack(const EventCallBack& callback);
    void SetErrorCallBack(const EventCallBack& callback);

public:
    // 设置监听事件, 判断是否监听了某事件
    void SetEvents(int event);
    void EnableReading();
    bool IsReading();
    void EnableWriting();
    void DisableWriting();
    bool IsWriting();

    // 设置在描述符上已经发生的事件
    void SetRevents(int events);

public:
    // 获得fd
    int GetFd() const;
    int GetEvents() const;
private:
    // 更新eventloop中对于该channel的处理
    void UpdateEventLoop();

private:
    int _fd; // 描述符: socketfd
    int _events; // 设置的描述符上监听的事件
    int _revents; // 描述符上发生的事件

    //  不同的事件发生时，不同的回调函数
    //  通过acceptor或者tcp_connection_handler设置
    ReadEventCallBack     _readcallback; 
    WriteEventCallBack    _writecallback;
    EventCallBack         _closecallback;
    EventCallBack         _errorcallback;

    // 本channel所属的eventloop
    std::shared_ptr<EventLoop> _eventloop;
};

#endif
