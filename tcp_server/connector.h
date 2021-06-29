#ifndef _H_TCP_CONNECTION_HANDLER_H_
#define _H_TCP_CONNECTION_HANDLER_H_

#include <memory>
#include "../function/function_typedef.h"

class Channel;
class SocketAddr;
class EventLoop;
// Connector是通过Acceptor创建，通过TcpServer管理的, 通过EventLoop操纵的
// tcp连接的信息和针对tcp连接的一些处理
class Connector {
public:
    // 设置新的连接的eventloop
    // eventloop可以是和acceptor一样的eventloop,
    // 也可以是新的eventloop
    Connector(int clientfd, std::shared_ptr<EventLoop> eventloop, const SocketAddr&, const SocketAddr&);
    // non-copyable
    Connector(const Connector&) = delete;
    Connector& operator=(const Connector&) = delete;

    ~Connector();
public:
    // 当eventloop中的channel有事件发生时，触发channel的回调函数
    // 进而触发的tcp_connection的回调函数, 对接收到的数据进行处理
    void SetRecvMessageCallBack(const RecvMessageCallBack& );
    // 发送完数据的一些处理
    void SetWriteFinishMessageCallBack(const WriteFinishMessageCallBack& );
    // 连接关闭的一些处理
    void SetCloseConnectionCallBack(const CloseConnectionCallBack&);

public:
    // 客户端和服务器开始相互发送信息
    void RecvMessage();
    // 将buffer中的信息发送给发送信息给客户端
    void SetSendBuffer(const std::string& send_data);
    void Send(const std::string& send_data);
    // server主动关闭连接
    void Close();

private:
    // tcp_connection对应的channel的回调函数
    void HandleRead();
    void HandleWrite();
    void HandleClose();
    void HandleError();

    std::shared_ptr<EventLoop> _eventloop; // 本client的eventloop
    std::shared_ptr<SocketAddr> _local_addr; // 本地的addr // 从acceptor中获取
    std::shared_ptr<SocketAddr> _client_addr; // 本client的addr
    std::shared_ptr<Channel> _channel; // 本client的channel
    
    // connection handler中要不要保存这些函数的指针 , 保存这些指针，可以达到在函数中回调这些函数，

    RecvMessageCallBack _recvmessagecallback; // 接收到数据的回调函数
    WriteFinishMessageCallBack _writefinishcallback; // 发送完数据的回调函数
    CloseConnectionCallBack _closeconnectioncallback; // 断开连接的回调函数

    std::shared_ptr<Buffer> _out_buffer; // sendmessagecallback buffer
    std::shared_ptr<Buffer> _in_buffer; // recvmessagecallback buffer
};

#endif
