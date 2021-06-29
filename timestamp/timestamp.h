#ifndef _H_TIMESTAMP_H_
#define _H_TIMESTAMP_H_

#include <string>
class TimeStamp {
public:
    TimeStamp();
    explicit TimeStamp(int64_t micro_second_since_epoch);
    // non-copyable
    TimeStamp(const TimeStamp&);
    TimeStamp& operator=(const TimeStamp&);

    ~TimeStamp();
public:
    std::string LogFileName(); // 返回用来构造log文件名的字符串
    std::string LogString(); // 返回用来构造log条目的字符串
    int64_t MicroSecondSinceEpoch() const;

    static const int k_micro_seconds_per_second = 1000 * 1000;
private:
    int64_t _micro_second_since_epoch;
};

// 给时间戳增加时间seconds
// 返回新的timestamp
inline TimeStamp AddTime(TimeStamp now, int64_t micro_seconds) {
    return TimeStamp(now.MicroSecondSinceEpoch() + micro_seconds);
}

#endif
