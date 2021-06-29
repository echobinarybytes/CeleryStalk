#include "buffer.h"
#include "../asynclogging/asynclog.h"
#include <unistd.h>

Buffer::Buffer(int size) : _buffer(size, 0), _begin(0), _end(0) {

}

Buffer::Buffer(const Buffer& buf) {
    _buffer = buf._buffer;
    _begin = buf._begin;
    _end = buf._end;
}

Buffer& Buffer::operator=(const Buffer& buf) {
    _buffer = buf._buffer;
    _begin = buf._begin;
    _end = buf._end;
    return *this;
}

// moveable 没有实现
Buffer::Buffer(const Buffer&&) {

}

// moveable 没有实现
Buffer& Buffer::operator=(const Buffer&&) {
    return *this;
}

Buffer::~Buffer() {

}

#include <iostream>
// 从_buffer中取出size大小的缓冲
std::shared_ptr<std::string> Buffer::Retrive(int size) {
    if (_end - _begin < size) {
        // 输出错误信息
        // 没有这么多的缓冲区数据
        std::cout << "size of char in buffer is smaller than size" << std::endl;
        return nullptr;
    }

   
    std::string buffer_tmp;
    for (int i = _begin; i < _end; ++ i) {
        buffer_tmp.push_back(_buffer[i]);
        ++ _begin;
    }
    std::shared_ptr<std::string> tmp = std::make_shared<std::string>(buffer_tmp);
    // 判断是否要erase前面的字符, 回收前面没有使用的空间
    while (_begin >= 1024) {
        _buffer.erase(0, 1024);
        _begin = _begin - 1024;
        _end =  _end - 1024;
    }

    // TODO 是否需要返回获得的字符串？？

    return tmp;
}

// 添加到buffer中
void Buffer::Append(std::string str) {
    for (int i = 0; str[i] != 0; ++ i) {
        _buffer[_end] = str[i];
        ++_end;
    }
}

int Buffer::ReadFd(int fd) {
    // 获得可以操作系统缓冲区的字符的数量
    // TODO:
    int size = 1024;
    // 创建合适大小的临时缓冲区
    std::string tmp_buffer(size, 0);
    int ret = read(fd, &tmp_buffer[0], tmp_buffer.size());
    // TODO 
    // 如果缓冲区不够存放系统缓冲区中的数据

    // 添加到缓冲区
    ClearBuffer();
    Append(tmp_buffer);

    return ret;
}

// 将buffer中的所有数据写到fd中
int Buffer::WriteFd(int fd) {
    // write(fd, "uubs", 5);
    // printf("\n%s\n", &_buffer[_begin]);
    int ret = write(fd, &_buffer[_begin], GetSize());
    // LOG_TRACE << std::string("write :") + _buffer[_begin];
    // TODO
    // 如果系统缓冲区不能完全装下buffer中的内容？
    
    
    return ret;
}

int Buffer::GetSize() const {
    return _end - _begin;
}

// 将buffer清空
void Buffer::ClearBuffer() {
    _end = _begin;
}

// 为空，返回true
bool Buffer::BufferEmpty() const {
    return !(_end - _begin);
}

