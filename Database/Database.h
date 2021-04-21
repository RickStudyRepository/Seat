#ifndef DATABASE_H
#define DATABASE_H

#include <QWidget>
#include <QString>
#include <string>
#include "Tools/AliasName.h"
#include "Tools/Tools.h"
#include "Tools/ConstValue.h"

// SQLite数据库相关
#include "./sqlite/sqlite3.h"

// 采用单例模式
class Database : public QWidget
{
    Q_OBJECT
public:
    // 获取单例变量实例的指针
    static Database*& getSingleDatabase(QWidget* parent = NULL);

    ~Database();

    // 查询学号是否已经存在
    bool isStudentExists(std::string studentNum, bool* success);

    // 插入一条学生表记录
    bool insertNewStudent(std::string studentNum);

    // 创建一条新的预约记录
    bool makeNewAppoinment(AliasName::Appointment newAppointment);

    // 获取学号为studentNum的学生的所有预约信息
    AliasName::Appointments getAllAppointmentsOf(const std::string studentNum, bool* success);

    // 获取数据库中的所有座位，在这里仅获取所有座位的所有信息
    AliasName::SeatInfos getAllSeats(bool* success);

    // 内部需要调用更新座位占用情况的方法
    bool continueAppointment(int appointmentId, const std::string newTime);

    // 取消预约，更新预约状态
    bool cancelAppointment(int appointmentId);

    // 获取某个座位的可用时间段
    AliasName::TimeScopes getAvailableTimeScopesOf(int seatNum, bool* success);

    // 获取某个座位当前可用的时间段
    AliasName::TimeScopes getCurrentAvailableTimeScopesOf(int seatNum, bool* success);

    // 获取所有座位当前的信息
    AliasName::SeatInfos getAllSeatsInfo(bool* success);

private:
    // 单例变量句柄
    static Database* singleDatabase;

    Database(QWidget* parent = NULL);

    // 数据库句柄
    sqlite3* database;

    // 打开数据库
    bool openDatabase();

    // 关闭数据库
    bool closeDatabase();

    // 初始化数据库，在打开数据库之后进行建表
    bool isInited;
    bool createStudentTable();
    bool createSeatTable();
    bool insertSeats();
    bool createOccupiedTimeTable();
    bool createAppointmentRecordTable();
    bool initDatabase();

    // 插入一条新的座位占用记录
    bool insertNewOccupiedTime(int seatNum, std::string time);

    // 获取某一个座位的占用时间段
    AliasName::TimeScopes getUnavailableTimeScopesOf(int seatNum, bool* success);

    std::vector< std::pair<int, std::string> > statusStringInt;
    void initStatusStringIntMap();
    // 状态字符串和状态值的转换
    int statusStringToInt(const std::string status);
    std::string intToStatusString(const int status);

signals:
    // 日志信号
    void logSignal(QString);
};

#endif // DATABASE_H
