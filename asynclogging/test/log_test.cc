#include "../asynclog.h"
#include <functional>
#include <vector>
#include <thread>

void trace() {
    while (true) {
        LOG_TRACE << "in trace functio";
    }
}

void debug() {
    while (true) {
        LOG_DEBUG << "in debug functio";
    }

}

void info() {
    while (true) {
        LOG_INFO << "in info functio";
    }

}

void warn() {
    while (true) {
        LOG_WARN << "in warn functio";
    }

}

void error() {
    while (true) {
        LOG_ERROR << "in error functio";
    }

}

void fatal() {
    while (true) {
        LOG_FATAL << "in fatal functio";

    }
}

int main() {
    AsyncLog::Init(4); // 初始化4个缓冲区

    std::vector<std::function<void()>> func = {trace, debug, info, warn, error, fatal};

    std::vector<std::thread> thread_pool;
    for (int i = 0; i < 6; ++ i) {
        thread_pool.push_back(std::thread(func[i]));
    }

    for (int i = 0; i < 6; ++ i) {
        thread_pool[i].join();
    }
    AsyncLog::Stop();
}
