#ifndef _H_FUNCTION_TYPEDEF_H_
#define _H_FUNCTION_TYPEDEF_H_

#include <functional>
class TcpConnectionHandler;
class Buffer;
class SocketAddr;
class Connector;

typedef std::function<void()> EventCallBack;

typedef EventCallBack ReadEventCallBack;
typedef EventCallBack WriteEventCallBack;

typedef std::function<void(int, SocketAddr&)> NewConnectionCallBack;

typedef std::function<void (Connector&, Buffer&)> RecvMessageCallBack;
typedef std::function<void ()> WriteFinishMessageCallBack;
typedef std::function<void ()> CloseConnectionCallBack;

typedef std::function<void ()> ConnectionCallBack;

typedef std::function<void ()> UserConnectionCallBack;
#endif
