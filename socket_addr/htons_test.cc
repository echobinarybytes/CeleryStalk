#include <netinet/in.h>
#include <iostream>

int main() {
    int port = 1234;
    std::cout << "port: " << port << std::endl;

    std::cout << "htons(port) : " << htons(port) << std::endl;
    std::cout << "htons(htons(port)) : " << htons(htons(port)) << std::endl;
}
