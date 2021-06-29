#include "tcp_server.h"
#include "acceptor.h"
#include "connector.h"
#include "../socket_addr/socket_addr.h"
#include "../event_loop/event_loop.h"
#include "../buffer/buffer.h"
#include "../asynclogging/asynclog.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>


bool TcpServer::_multithread = false;
// eventloop 是给acceptor的
TcpServer::TcpServer(EventLoopPtr eventloop, int port)  
    :_server_addr(htonl(INADDR_ANY), htons(port)),
     _acceptor_eventloop(eventloop),  
     _connections(),
     _connection_eventloops(), 
    _userrecvmessagecallback(DefaultRecvMessageCallBack), 
    _userwritefinishcallback(DefaultWriteFinishMessageCallBack), 
    _usercloseconnectioncallback(DefaultCloseConnectionCallBack),
    _eventloopthreadpool()  {

    _acceptor = std::make_shared<Acceptor>(eventloop, _server_addr);
    _acceptor->SetNewConnectionCallBack(
            std::bind(&TcpServer::NewConnectionCallBack, this, std::placeholders::_1, std::placeholders::_2)
            );
}

TcpServer::~TcpServer() {

}

void TcpServer::StartServer(int number) {
    _acceptor->Listen();
    if (number > 1) {
        _multithread = true; // 多线程
        _eventloopthreadpool.StartEventLoopThreadPool(number);
    }

 // 按照number启动多线程
    _acceptor_eventloop->Loop();
}


void TcpServer::SetUserRecvMessageCallBack(RecvMessageCallBack& cb) {
    _userrecvmessagecallback = cb;
}

void TcpServer::SetUserWriteFinishMessageCallBack(WriteFinishMessageCallBack& cb) {
    _userwritefinishcallback = cb;
}

void TcpServer::SetUserConnectionCallBack(UserConnectionCallBack& cb) {
    _userconnectioncallback = cb;
}


void TcpServer::NewConnectionCallBack(int fd, SocketAddr& addr) {
    ConnectionPtr new_connection;
    if (!_multithread) {
        new_connection = std::make_shared<Connector>(fd, _acceptor_eventloop, _server_addr, addr);


    } else {
        // TODO: 在多线程的情况下如何处理 ？
        int id = _eventloopthreadpool.GetNextId();
        new_connection = std::make_shared<Connector>(fd, _eventloopthreadpool.Next(), _server_addr, addr);
        char buff[50] = "Client";
        inet_ntop(AF_INET, &addr.GetAddr().sin_addr, buff + 6, sizeof(buff));
        LOG_TRACE << std::string(buff) + ":" + std::to_string(ntohs(addr.GetAddr().sin_port))
         + " eventloop-" + std::to_string(id);
    }

        _connections.push_back(new_connection);
        // 设置回调函数
        new_connection->SetRecvMessageCallBack(_userrecvmessagecallback);
        new_connection->SetWriteFinishMessageCallBack(_userwritefinishcallback);
        new_connection->SetCloseConnectionCallBack(_usercloseconnectioncallback);
    
        // 哪里设置监听的信号？
        new_connection->RecvMessage();
    if (_userconnectioncallback) {
        _userconnectioncallback();
    }
}

void TcpServer::DefaultRecvMessageCallBack(Connector& conn, Buffer& buffer) {
    // buffer.PrintBuffer();
    conn.Send(*buffer.Retrive(buffer.GetSize()));
}

void TcpServer::DefaultWriteFinishMessageCallBack() {

}

void TcpServer::DefaultCloseConnectionCallBack() {

}
