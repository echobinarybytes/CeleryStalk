#include "timestamp.h"
#include <sys/time.h>

TimeStamp::TimeStamp() : _micro_second_since_epoch(0) {
    struct timeval tv;
    gettimeofday(&tv, nullptr); // 获取当前时间
    _micro_second_since_epoch = tv.tv_sec * k_micro_seconds_per_second + tv.tv_usec;
}

TimeStamp::TimeStamp(int64_t micro_second_since_epoch) {
    _micro_second_since_epoch = micro_second_since_epoch;
}

// non-copyable
TimeStamp::TimeStamp(const TimeStamp& ts) {
    _micro_second_since_epoch = ts._micro_second_since_epoch;
}

TimeStamp& TimeStamp::operator=(const TimeStamp& ts) {
    _micro_second_since_epoch = ts._micro_second_since_epoch;
    return *this;
}

TimeStamp::~TimeStamp() {

}

// 返回用来构造log文件名的字符串
// 0123456789abcdef
// 20120603 0802:46
// 20120603-080246
std::string TimeStamp::LogFileName() {
    std::string tmp = LogString().substr(0, 17);
    tmp[8] = '-';
    tmp.erase(tmp.begin() + 11);
    tmp.erase(tmp.begin() + 13);
    return tmp;
} 

// 返回用来构造log条目的字符串
// 日期     时间      微秒   线程  级别  正文  源文件名:行号
// 20120603 08:02:46.125770Z 23261 INFO Hello world - test.cc:51
std::string TimeStamp::LogString() {
    time_t seconds = static_cast<time_t>(_micro_second_since_epoch / k_micro_seconds_per_second);
    int64_t micro_seconds = _micro_second_since_epoch % k_micro_seconds_per_second;
    struct tm time;
    gmtime_r(&seconds, &time);

    char buf[32] = {0};
    sprintf(buf, "%d%02d%02d %02d:%02d:%02d.%06d",
            time.tm_year + 1900,
            time.tm_mon + 1,
            time.tm_mday,
            time.tm_hour,
            time.tm_min,
            time.tm_sec,
            static_cast<int>(micro_seconds));
    return buf;
}


int64_t TimeStamp::MicroSecondSinceEpoch() const {
    return _micro_second_since_epoch;
}

