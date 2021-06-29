#include "socket_addr.h"
#include <cstring>

SocketAddr::SocketAddr(long ipv4_net_addr, int port_net) {
    memset(&_socket_addr, 0, sizeof(_socket_addr));
    _socket_addr.sin_family = AF_INET;
    _socket_addr.sin_addr.s_addr = ipv4_net_addr;
    _socket_addr.sin_port = port_net;
}

// copyable
SocketAddr::SocketAddr(const SocketAddr& addr) {
    _socket_addr.sin_family = addr.GetAddr().sin_family;
    _socket_addr.sin_addr.s_addr = addr.GetAddr().sin_addr.s_addr;
    _socket_addr.sin_port = addr.GetAddr().sin_port;
}

SocketAddr& SocketAddr::operator=(const SocketAddr& addr) {
    _socket_addr.sin_family = addr.GetAddr().sin_family;
    _socket_addr.sin_addr.s_addr = addr.GetAddr().sin_addr.s_addr;
    _socket_addr.sin_port = addr.GetAddr().sin_port;
    return *this;
}
    // move ??

SocketAddr::~SocketAddr() {

}
//std::shared_ptr<sockaddr_in> SocketAddr::GetAddr() const {
//    return std::make_shared<sockaddr_in>(_socket_addr);
//}
//

const struct sockaddr_in& SocketAddr::GetAddr() const {
    return _socket_addr;
}
