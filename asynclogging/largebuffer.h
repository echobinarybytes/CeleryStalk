#ifndef _H_LARGE_BUFFER_H_
#define _H_LARGE_BUFFER_H_

#include <string>
#define LARGEBUFFERSIZE  4 * 1024 * 1024

class LargeBuffer {
public:
    LargeBuffer();
    // non-copyable
    LargeBuffer(const LargeBuffer&) = delete;
    LargeBuffer& operator=(const LargeBuffer&) = delete;

    ~LargeBuffer();

public:
    // 如果append成功，返回true;否则返回false
    void Append(std::string&);
    int  Avail() const; // 返回还剩下的空间
    void StoreToFile(std::string&);
    void SetBufferZero(); // 

private:
    char _large_buffer[LARGEBUFFERSIZE]; //固定大小:4M
    int _cur_idx;
};

#endif
