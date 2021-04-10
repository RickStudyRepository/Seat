#ifndef ALIASNAME_H
#define ALIASNAME_H

#include <vector>
#include <string>

namespace AliasName {
    // 时间范围，整点范围
    typedef std::pair<int, int> TimeScope;
    // 可用的时间范围列表
    typedef std::vector<TimeScope> TimeScopes;

    // 一条预约信息
    typedef struct Appointment {
        std::string studentNum;
        int seatNum;
        std::string time;
        std::string status;

        // 向数据库写入数据时需要完全构造
        Appointment(std::string studentNum, int seatNum, std::string time, std::string status) {
            this->studentNum = studentNum;
            this->seatNum = seatNum;
            this->time = time;
            this->status = status;
        }

        // 从数据库中取出某一个同学的预约信息，可以缺省构造
        Appointment(int seatNum, std::string time, std::string status) {
            this->studentNum = "";
            this->seatNum = seatNum;
            this->time = time;
            this->status = status;
        }
    } Appointment;
    // 预约信息列表
    typedef std::vector<Appointment> Appointments;

    // 座位信息
    typedef struct SeatInfo {
        int num;
        TimeScopes availableTimes;

        SeatInfo(int num, TimeScopes availableTimes) {
            this->num = num;
            this->availableTimes = availableTimes;
        }

        bool hasAvailableTimeScope() {
            return this->availableTimes.empty();
        }
    } SeatInfo;
}

#endif // ALIASNAME_H
