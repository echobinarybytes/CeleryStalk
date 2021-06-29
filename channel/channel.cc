#include "channel.h"
#include <sys/epoll.h>
#include "../buffer/buffer.h"

Channel::Channel(std::shared_ptr<EventLoop> eventloop, int fd) 
    : _fd(fd),
    _events(0),
    _revents(0),
    _readcallback(nullptr),
    _writecallback(nullptr),
    _closecallback(nullptr),
    _errorcallback(nullptr), 
    _eventloop(eventloop)  {


}

Channel::~Channel() {

}

// 处理事件

// 其他事件
// EPOLLONESHOT -> 只监听一次事件，当监听完这次事件之后，如果还需要监听这个socket的话，需要再次把这个socket加入到epoll队列里
// EPOLLWAKEUP ->

// 读事件
// EPOLLIN -> 对应的文件句柄可读，包括对端socket正常关闭
// EPOLLPRI -> 对应的文件句柄有紧急的数据可读

// 写事件
// EPOLLOUT -> 对应的文件句柄可写

// 错误事件
// EPOLLHUP -> 对应的文件句柄被挂断？
//当socket一端认为对方发来了一个不存在的4元组请求的时候，会回复一个RST响应，在epoll上会响应EPOLLHUP事件，当发生以下两种情况时会发RST响应:
// (1) 当客户端向一个没有在Listen的服务器端口发送connect的时候，服务器会返回一个RST，因为服务器根本就不知道这个4元组的存在
// (2) 当已经建立好连接的一对客户端和服务器，客户端突然操作系统崩溃，或者拔掉电源导致操作系统重启(kill pid或者正常关机不行，因为操作系统会发送FIN给对方), 这时服务器在原有的4元组上发送数据，会收到客户端返回RST, 因为客户端根本不知道这个4元组的存在
// EPOLLERR -> 对应的文件句柄发生错误
// EPOLLRDHUP ->
// EPOLLEXCLUSIVE ->
void Channel::HandleEvent() {
    // 读事件
    if (_revents & (EPOLLIN | EPOLLPRI)) {
        _readcallback();
    }

    // 写事件
    if (_revents & EPOLLOUT) {
        _writecallback();
    }
    //TODO 当发生其他事件时
    // 错误事件
    if (_revents & ( EPOLLERR)) {
        if (_errorcallback) _errorcallback();
    }
    // 关闭事件
    if ((_revents & EPOLLHUP) && !(_revents & EPOLLIN)) {
        if (_closecallback) _closecallback();
    }
}

// 设置回调函数
void Channel::SetReadCallBack(const ReadEventCallBack& callback) {
    _readcallback = callback;
}

void Channel::SetWriteCallBack(const WriteEventCallBack& callback) {
    _writecallback = callback;
}

void Channel::SetCloseCallBack(const EventCallBack& callback) {
    _closecallback = callback;
}

void Channel::SetErrorCallBack(const EventCallBack& callback) {
    _errorcallback = callback;
}

// 设置监听事件, 判断是否监听了某事件
// 设置监听事件, 覆盖channel中_event
void Channel::SetEvents(int event) {
    _events = event;
    UpdateEventLoop();
}

void Channel::EnableReading() {
    _events |= EPOLLIN | EPOLLPRI; // 用在epoll中
    UpdateEventLoop();
}

bool Channel::IsReading() {
    if (_events & EPOLLIN || _events & EPOLLPRI) {
        return true;
    }
    return false;
}

void Channel::DisableWriting() {
    int temp = _events & (~EPOLLOUT);
    _events &= temp;
    UpdateEventLoop();
}

void Channel::EnableWriting() {
    _events |= EPOLLOUT;
    UpdateEventLoop();
}

bool Channel::IsWriting() {
    if (_events & EPOLLOUT) {
        return true;
    }
    return false;
}

// 设置在描述符上已经发生的事件
void Channel::SetRevents(int events) {
    _revents = events;
}


// 获得fd
int Channel::GetFd() const {
    return _fd;
}

int Channel::GetEvents() const {
    return _events;
}
// 用来在Poll中设置事件, 
void Channel::UpdateEventLoop() {
    if (_events == 0) {
        _eventloop->RemoveChannel(*this);
    } else {
        _eventloop->UpdateChannel(*this);
    }
}

