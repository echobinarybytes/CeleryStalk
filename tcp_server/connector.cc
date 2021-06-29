#include "connector.h"
#include "../socket_addr/socket_addr.h"
#include "../channel/channel.h"
#include "../buffer/buffer.h"
#include "../asynclogging/asynclog.h"
#include <functional>
#include <iostream>


// 设置新的连接的eventloop
// eventloop可以是和acceptor一样的eventloop,
// 也可以是新的eventloop
Connector::Connector(int clientfd, std::shared_ptr<EventLoop> eventloop, const SocketAddr& local_addr, const SocketAddr& client_addr) 
    : _eventloop(eventloop),
    _local_addr(new SocketAddr(local_addr)),
    _client_addr(new SocketAddr(client_addr)),
    _channel(new Channel(eventloop, clientfd)),
    _out_buffer(new Buffer()),
    _in_buffer(new Buffer()) {

    _channel->SetReadCallBack(
            std::bind(&Connector::HandleRead, this)
            );
    _channel->SetWriteCallBack(
            std::bind(&Connector::HandleWrite, this)
            );
    _channel->SetCloseCallBack(
            std::bind(&Connector::HandleClose, this)
            );

    _channel->SetErrorCallBack(
            std::bind(&Connector::HandleError, this)
            );
}

Connector::~Connector() {

}

// 当eventloop中的channel有事件发生时，触发channel的回调函数
// 进而触发的tcp_connection的回调函数
void Connector::SetRecvMessageCallBack(const RecvMessageCallBack& callback) {
    _recvmessagecallback = callback;
}

void Connector::SetWriteFinishMessageCallBack(const WriteFinishMessageCallBack& callback) {
    _writefinishcallback = callback;
}

// 连接关闭的一些处理
void Connector::SetCloseConnectionCallBack(const CloseConnectionCallBack& callback) {
    _closeconnectioncallback = callback;
}

// 客户端和服务器开始相互发送信息
void Connector::RecvMessage() {
    _channel->EnableReading(); // 开始监听channel
}

// 将buffer中的信息发送给发送信息给客户端
void Connector::SetSendBuffer(const std::string& send_data) {
    _out_buffer->ClearBuffer(); // 清楚缓冲区原有的内容
    _out_buffer->Append(send_data);
}

void Connector::Send(const std::string& send_data) {
    _channel->EnableWriting();
    _out_buffer->ClearBuffer(); // 清楚缓冲区原有的内容
    _out_buffer->Append(send_data);
    // 在多线程程中如何处理
    
}

// server主动关闭连接
void Connector::Close() {

}

// tcp_connection对应的channel的回调函数
// 套接字上读
void Connector::HandleRead() {
    // 读信息完毕
    // 读信息未完毕
    // 判断读完
    int ret = _in_buffer->ReadFd(_channel->GetFd());
    if (ret > 0) { // 对接受到的信息进行处理
        // 设置写的信号，要不要设置？
        _recvmessagecallback(*this, *_in_buffer);
    } else if (ret == 0) {
        HandleClose();
    } else {
        HandleError();
    }
}

// tcp_connection对应的channel的回调函数
void Connector::HandleWrite() {
    // 对发送的信息进行处理，依然写到
    // 发送完毕，
    // 发送未完毕
    int ret = _out_buffer->WriteFd(_channel->GetFd());
    // LOG_TRACE << std::string("writing :") + *_out_buffer->Retrive(_out_buffer->GetSize());
    // 判断写完
    if (ret == _out_buffer->GetSize()) {

    }
    _writefinishcallback();
    _channel->DisableWriting();
}

// tcp_connection对应的channel的回调函数, server被动关闭连接
void Connector::HandleClose() {
    // 不再监听
    _channel->SetEvents(0);
    // 在上层关闭
}

// tcp_connection对应的channel的回调函数
void Connector::HandleError() {
    // 获得errno
    // 打印出来
    // 推出进程

}
