#ifndef _H_BUFFER_H_
#define _H_BUFFER_H_

#include <string>
#include <memory>
#include <iostream>

class Buffer {
public:
    Buffer(int size = 1024);

    // copyable
    Buffer(const Buffer&);
    Buffer& operator=(const Buffer&);

    // moveable
    Buffer(const Buffer&&);
    Buffer& operator=(const Buffer&&);

    ~Buffer();

public:
    // 取出和放入-处理粘包问题
    // 当发送来10K数据, 接受的情况可能是：
    // 10K, 5K + 5K, 2K + 2K + 2K + 2K , ...
    // 一个包并没有接受完全，只接受了部分的情况,buffer可以缓存部分的包
    // 从buffer中取出size字符, 取了放到哪里
    std::shared_ptr<std::string> Retrive(int size);

    // 添加string到缓冲区中
    void Append(std::string str);

    // 从fd描述符中读取数据到缓冲区，返回读取的字节数
    int ReadFd(int fd);
    // 写入内容到fd描述符中，返回写入的字节数
    int WriteFd(int fd);

    int GetSize() const;

    // 将buffer清空
    void ClearBuffer();

    void PrintBuffer() { std::cout << _buffer << std::endl; }

private:
    bool BufferEmpty() const;
    // 1. 使用queue作为缓冲区，方便取出和放入
    // 使用queue的效率可能会受到严重影响 ?? 
   // std::queue<char> _buffer; 直接使用queue不好操作，不能遍历
   // 2. 使用std::string作为一个queue
   //      std::string[0] --- std::string[n]
   //    <- erase                     append ->
   //    到一定时候再erase, 当_begin = 1024时，删除1024之前的
   //    当_begin = _end - 1时，buffer为空
   //    当_end指向的位置为空
    std::string _buffer;
    int _begin;
    int _end;
};

#endif
