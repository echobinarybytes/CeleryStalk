#include "socket_addr.h"
#include <sys/socket.h>
#include <iostream>
#include <cstring>

int main() {
    struct sockaddr_in raw_addr; 
    memset(&raw_addr, 0, sizeof(raw_addr));
    {
        SocketAddr addr(1024);
        raw_addr = addr.GetAddr();
    }
    std::cout << raw_addr.sin_family;
}
