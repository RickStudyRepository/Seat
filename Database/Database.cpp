#include "Database.h"

void Database::makeNewAppoinment(AliasName::Appointment newAppointment) {
    qDebug() << "向数据库写入预约信息：\n"
             << "预约人：" << QString::fromStdString(newAppointment.studentNum) << "\n"
             << "座位号：" << newAppointment.seatNum << "\n"
             << "时间段：" << QString::fromStdString(newAppointment.time) << "\n"
             << "状态：" << QString::fromStdString(newAppointment.status);
}

AliasName::Appointments Database::getAllAppointmentsOf(const std::string studentNum) {
    AliasName::Appointments appointments;
    appointments.push_back(AliasName::Appointment(1, 23, "2020-04-09 08:00-09:00", ConstValue::UsedSeat));
    appointments.push_back(AliasName::Appointment(2, 34, "2020-04-09 18:00-21:00", ConstValue::UsingSeat));
    return appointments;
}

void Database::continueAppointment(unsigned int id, const std::string newTime) {
    qDebug() << "续约\n"
             << "预约编号：" << id << "\n"
             << "新的时间段：" << QString::fromStdString(newTime);
    // 这里要从数据库中拿出相应id号的预约信息，获取seatNum以便更新座位的占用情况
    updateSeatTimeScopesOf(34, Tools::databaseTimeToTimeScope(newTime));
}

void Database::cancelAppointment(unsigned int id) {
    qDebug() << "取消预约\n"
             << "预约编号：" << id << "\n";
}

void Database::updateSeatTimeScopesOf(unsigned int seatNum, AliasName::TimeScope newTimeScope) {
    qDebug() << "增加" << seatNum << "的占用时间段";
}

AliasName::TimeScopes Database::getAvailableTimesOf(unsigned int seatNum) {
    AliasName::TimeScopes test;
    test.push_back(AliasName::TimeScope(8, 12));
    test.push_back(AliasName::TimeScope(14, 22));
    return test;
}
