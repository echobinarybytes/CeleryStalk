#ifndef _H_TCP_SERVER_H_
#define _H_TCP_SERVER_H_

#include <memory>
#include <vector>
#include "../function/function_typedef.h"
#include "../socket_addr/socket_addr.h"
#include "../event_loop_thread_pool/event_loop_thread_pool.h"
class Acceptor;
class Connector;
class EventLoop;

typedef std::shared_ptr<Connector> ConnectionPtr;
typedef std::shared_ptr<EventLoop> EventLoopPtr;

// tcpserver中管理什么？？
class TcpServer {
public:
    TcpServer(EventLoopPtr eventloop, int port); // eventloop 是给acceptor的
    // non-copyable
    TcpServer(const TcpServer&) = delete;
    TcpServer& operator=(const TcpServer&) = delete;

    ~TcpServer();

public:
    void StartServer(int number); // number表示开几个线程

    void SetUserRecvMessageCallBack(RecvMessageCallBack& cb);
    void SetUserWriteFinishMessageCallBack(WriteFinishMessageCallBack& cb);
    void SetUserConnectionCallBack(UserConnectionCallBack& cb);

private:
    void NewConnectionCallBack(int fd, SocketAddr& addr); // 给acceptor的回调函数
    static void DefaultRecvMessageCallBack(Connector& conn, Buffer& buffer);
    static void DefaultWriteFinishMessageCallBack();
    static void DefaultCloseConnectionCallBack();

    SocketAddr _server_addr;
    static bool _multithread;
    std::shared_ptr<EventLoop> _acceptor_eventloop;
    std::shared_ptr<Acceptor> _acceptor;
    std::vector<ConnectionPtr> _connections;
    std::vector<EventLoopPtr> _connection_eventloops;  // 存储连接的eventloop

    // 通过用户设置, 或者默认设置
    RecvMessageCallBack _userrecvmessagecallback; // 对buffer数据进行处理
    WriteFinishMessageCallBack _userwritefinishcallback; // 
    CloseConnectionCallBack _usercloseconnectioncallback; 
    UserConnectionCallBack _userconnectioncallback; // 当有新连接的时候调用的用户回调函数
    EventLoopThreadPool _eventloopthreadpool;
};

#endif
