#ifndef _H_SOCKET_ADDR_H_
#define _H_SOCKET_ADDR_H_

#include <netinet/in.h>
#include <memory>

class SocketAddr {
public:
    SocketAddr(long ipv4_addr, int port); // 网络序
    // copyable
    SocketAddr(const SocketAddr&);
    SocketAddr& operator=(const SocketAddr&);
    // move ??

    ~SocketAddr();
public:
    // std::shared_ptr<sockaddr_in> GetAddr() const;
    const struct sockaddr_in& GetAddr() const;
private:
    struct sockaddr_in _socket_addr;
};
#endif
