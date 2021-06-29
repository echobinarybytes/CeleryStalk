#ifndef _H_ACCEPTOR_H_
#define _H_ACCEPTOR_H_

#include <memory>
#include <functional>
#include "../function/function_typedef.h"

class SocketAddr;
class TcpConnectionHandler;
class Channel;
class EventLoop;

// Acceptor是通过TcpServer创建和管理的
class Acceptor {
public:
    // 创建套接字
    Acceptor(std::shared_ptr<EventLoop> eventloop, const SocketAddr&);
    // non-copyable
    Acceptor(const Acceptor&) = delete;
    Acceptor& operator=(const Acceptor&) = delete;
    ~Acceptor();

public:
    // tcpserver中设置当新连接到来Acceptor回调的函数
    // 当eventloop中的channel有事件发生时，触发channel的回调函数
    // 进而触发acceptor的回调函数
    void SetNewConnectionCallBack(const NewConnectionCallBack&); // acceptor的回调函数

    // acceptor开始监听
    void Listen();
    
private:
    // 返回listenfd
    int SocketBindListen();
    // 注册到对应本描述符channel中的函数
    void HandleRead();

    std::shared_ptr<EventLoop> _eventloop; // acceptor的eventloop
    std::shared_ptr<SocketAddr> _server_addr; // 存储server的套接字地址
    std::shared_ptr<Channel> _channel; // listenfd对应的channel
    NewConnectionCallBack _newconnectioncallback; // 需要设置到_channel中
};

#endif
