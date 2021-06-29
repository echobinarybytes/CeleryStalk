#include "largebuffer.h"
#include <cstring>
#include <fcntl.h>
#include <unistd.h>


#include <iostream>

LargeBuffer::LargeBuffer() : _cur_idx(0){
    memset(_large_buffer, 0, sizeof(_large_buffer));
}

LargeBuffer::~LargeBuffer() {

}

// 如果append成功，返回true;否则返回false
void LargeBuffer::Append(std::string& str) {

    strcpy(_large_buffer + _cur_idx, str.c_str());
    std::cout << str;
    _cur_idx += str.size();
    // printf("%p\n", _large_buffer);
}

// 返回还剩下的空间
int  LargeBuffer::Avail() const {
    return LARGEBUFFERSIZE - _cur_idx;
} 

void LargeBuffer::StoreToFile(std::string& filename) {
    // printf("consumer : %p\n", _large_buffer);
    int fd = open(filename.c_str(), O_WRONLY | O_CREAT);
    // 错误处理
    int size = write(fd, _large_buffer, strlen(_large_buffer));
    // 错误处理
    if (size != strlen(_large_buffer)) {

    } 
    close(fd);
}

void LargeBuffer::SetBufferZero() {
    memset(_large_buffer, 0, sizeof(_large_buffer));
    _cur_idx = 0;
}

