#include "tcp_server/tcp_server.h"
#include "channel/channel.h"
#include "./asynclogging/asynclog.h"


void Echo() {

}

int main() {
    ////////////////////启动日志系统/////////////////
    AsyncLog::Init(4);
    ////////////////////////////////////////
    
    std::shared_ptr<EventLoop> eventloop = std::make_shared<EventLoop>();
    eventloop->SetPoller(eventloop);
    TcpServer server(eventloop, 1234);
    server.StartServer(5);

    ////////////////////关闭日志系统/////////////////
    AsyncLog::Stop();
    ////////////////////////////////////////
}
