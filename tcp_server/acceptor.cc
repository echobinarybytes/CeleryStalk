#include "acceptor.h"
#include "../socket_addr/socket_addr.h"
#include "connector.h"
#include "../channel/channel.h"
#include "../asynclogging/asynclog.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>


// 完成accept之前的步骤
Acceptor::Acceptor(std::shared_ptr<EventLoop> eventloop, const SocketAddr& addr) 
     : _eventloop(eventloop),
       _server_addr(std::make_shared<SocketAddr>(addr)) {

    int listenfd = SocketBindListen();
    _channel = std::make_shared<Channel>(_eventloop, listenfd);
    
    // 设置_channel回调函数
    _channel->SetReadCallBack(
            std::bind(&Acceptor::HandleRead, this)
            );
}

Acceptor::~Acceptor() {

}

void Acceptor::SetNewConnectionCallBack(const NewConnectionCallBack& callback) {
    _newconnectioncallback = callback;
}

void Acceptor::Listen() {
    _channel->EnableReading();
}
// 当channel上有事件的回调函数
void Acceptor::HandleRead() {
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);

    int clientfd = accept(_channel->GetFd(), (struct sockaddr*)&client_addr, &addr_len);
    // 对clientfd的错误处理 TODO
    //
    //
    

    // 构造TcpConnectionHandler
    SocketAddr addr(client_addr.sin_addr.s_addr, client_addr.sin_port);
    // TODO 设置回调函数怎么设置, 在TcpConnectionHandler中设置还是在TcpServer中设置？？
   // new_connection_call_back怎莫返回数据, 使用bind操纵类 
    _newconnectioncallback(clientfd, addr);
}

int Acceptor::SocketBindListen() {
    // TODO
    // 添加一个文件描述符，当文件描述符都不够用的时候，使用这个文件描述符号，然后优雅的关闭

    int listenfd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, 0);
    if (listenfd == -1) {
        // ERROR-LOG TODO
        std::cout << "error : socket" << std::endl;
        exit(-1);
    }

    if (bind(listenfd, (struct sockaddr*)&_server_addr->GetAddr(), sizeof(_server_addr->GetAddr())) == -1) {
        // ERROR-LOG
        std::cout << "error : bind" << std::endl;
        exit(-1);
    }

    if (listen(listenfd, SOMAXCONN) == -1) {
        // ERROR-LOG
        std::cout << "error : listen" << std::endl;
        exit(-1);
    }

    return listenfd;
}
