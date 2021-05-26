# CeleryStalk
高并发服务器的实现


## 服务器编写的4大杀手

## 粘包问题

## busy loop

## poll
poll 使用基本流程

### EMFILE的处理
accept(2)返回EMFILE的处理
- 调高进程文件描述符的数目：治标不治本
- 死等
- 退出程序：不能达到7x24小时服务
- 关闭监听套接字。那什么时候重新打开？
- 如果是epoll模型，可以改用edge trigger.问题是如果漏掉一次accept(2),程序再也不会收到新连接
- 准备一个空闲的文件描述符。遇到这种情况，先关闭这个空闲文件，获得一个文件描述符名额；再accept(2)拿到socket连接的文件描述符；随后立刻close(2),这样就优雅地断开了与客户端的连接；最后重新打开空闲文件，把“坑填上", 以备再次出现这种情况时使用


### connfd的事件有那些

## epoll

```c++
#include <sys/epoll.h>

int epoll_create(int size); // size参数忽略
int epoll_create1(int flags);
```

### poll模型和epoll模型对比
poll模型 : 每次调用poll函数的时候都需要把监听套接字与已连接套接字所感兴趣的事件数组，拷贝到内核
epoll模型 : 不需要拷贝

### connfd的事件有那些












## 服务器
ssh root@123.60.33.30
password: uhost-g1jgpi3q
