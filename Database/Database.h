#ifndef DATABASE_H
#define DATABASE_H

#include <QDebug>
#include <QString>
#include "Tools/AliasName.h"
#include "Tools/Tools.h"
#include "Tools/ConstValue.h"

class Database
{
public:
    // 创建一条新的预约记录
    static void makeNewAppoinment(AliasName::Appointment newAppointment);

    // 获取学号为studentNum的所有预约信息
    static AliasName::Appointments getAllAppointmentsOf(const std::string studentNum);

    // 获取座位号为seatNum的所有占用时间段
    // 内部需要调用更新座位占用情况的方法
    static void continueAppointment(unsigned int id, const std::string newTime);

    // 取消预约，更新预约状态
    static void cancelAppointment(unsigned int id);

    // 更新座位占用情况
    // 传入新增的座位占用时间段及座位号
    static void updateSeatTimeScopesOf(unsigned int seatNum, AliasName::TimeScope newTimeScope);

    // 获取某个座位的可用时间段
    static AliasName::TimeScopes getAvailableTimesOf(unsigned int seatNum);
};

#endif // DATABASE_H
