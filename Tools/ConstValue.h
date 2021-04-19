#ifndef CONSTVALUE_H
#define CONSTVALUE_H

#include "MacroDefinition.h"
#include "AliasName.h"
#include <QString>
#include <QSize>

using namespace MacroDefinition;

namespace ConstValue {
    // 应用相关的一些属性
    inline namespace Window {
        // 窗口标题
        const QString appName = "Seat";
        // 窗口图标
        const QString appIconLoction = ":/images/Seat.ico";
        // 应用窗口大小
        const QSize windowSize = QSize(800, 480);
    }

    // 字体大小
    inline namespace FontSize {
        // 标题字体大小
        const int TitleFontSize = 15;
        // 表格表头字体大小
        const int TableHeaderFontSize = 13;
        // 表格内容字体大小
        const int TableContentFontSize = 12;
        // 对话框字体大小
        const int DialogFontSize = 13;
        // 座位号字体大小
        const int SeatNumFontSize = 12;
        // 其他说明性信息字体大小
        const int DescribeFontSize = 13;
    }

    // 管理员相关
    inline namespace Admin {
        // 超级管理员密码
        const std::string password = "123456";
    }

    // 操作界面相关
    inline namespace OperationPage {
        // 操作界面自动返回首页的时间
        const int maxDuration = 120;
        // 预约记录表格的列数
        const int appointmentRecordTableColumn = 3;
        // 现场预约Tab的字符串
        const QString makeAppoinmentString = "现场预约";
        // 我的预约Tab的字符串
        const QString appointmentRecordString = "我的预约";
    }


    // RFID相关
    inline namespace RFID {
        // 读卡或写卡最大重试次数
        const unsigned short maxTryTimeOnCard = 10;
        // RFID卡片的默认密码
        const unsigned char rfidPassword[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
        // RFID默认操作的扇区号
        const uint32 sectorNum = 0;
        // RFID默认操作的块号
        const uint32 blockNum = 1;
    }

    // 数据库相关
    inline namespace Database {
        // 学号长度
        const size_t studentNumLength = 9;

        // 预约状态
        inline namespace AppoinmentStatus {
            const std::string UsedSeat = "已履约";
            const std::string UnusedSeat = "已失约";
            const std::string UsingSeat = "履约中";
            const std::string NotSigned = "未签到";
        }

        // 座位相关
        inline namespace Seat {
            // 座位数量
            const size_t seatNums = 100;

            // 默认可用时间段
            const AliasName::TimeScope maxTimeScope = AliasName::TimeScope(8, 22);
        }

        // 数据库相关的属性
        inline namespace DatabaseProperty {
            // 数据库名称
            const std::string databaseName = "./Database/Seat.db";

            // 表名
            // 学生表
            const std::string studentTableName = "Student";
            // 座位表
            const std::string seatTableName = "Seat";
            // 占用时间表
            const std::string occupiedTimeTableName = "OccupiedTime";
            // 预约记录表
            const std::string appointmentRecordTableName = "AppointmentRecord";
        }

        // 建表相关的SQL语句
        inline namespace CreatTableSQL {
            // 创建学生表SQL语句
            const std::string creatStudentTableSql =
                    "Create Table " + studentTableName + "(" +
                    // 学号固定长度为9
                    "number char(9) Primary Key Not NULL, " +
                    // 密码变动长度为16
                    "password varchar(16) Not NULL" +
                    ");";
            // 创建座位表SQL语句
            const std::string creatSeatTableSql =
                    "Create Table " + seatTableName + "(" +
                    // 座位表仅存储所有座位的座位号
                    "number int Primary Key Not NULL" +
                    ");";
            // 创建占用时间表SQL语句
            const std::string creatOccupiedTimeSql =
                    "Create Table " + occupiedTimeTableName + "(" +
                    // 将编号设置为自增长，这个时候必须设置数据类型为Integer（在SQLite数据库中）
                    "id Integer Primary Key AutoIncrement Not NULL, " +
                    "seatNum int Not NULL, " +
                    "time char(22) Not NULL, " +
                    // 将seatNum设置为外键
                    "Foreign Key(seatNum) References Seat(number)" +
                    ");";
            // 创建预约记录表SQL语句
            const std::string creatAppointmentRecordTableSql =
                    "Create Table " + appointmentRecordTableName + "(" +
                    // 将预约号设置为自增长
                    "id Integer Primary Key AutoIncrement Not NULL, " +
                    "studentNum char(9) Not NULL, " +
                    "appointmentInfo Integer Not NULL, " +
                    // status使用整数存储，取出时转化为相应的字符串
                    "status smallint Not NULL, " +
                    // 将学号设置为外键
                    "Foreign Key(studentNum) References Student(number), " +
                    // 将预约信息设置为外键
                    "Foreign Key(appointmentInfo) References OccupiedTime(id)" +
                    ");";
        }

        // 插入记录的SQL语句模板
        inline namespace InsertSQLTemplate {
            // 插入一条学生记录
            const std::string insertNewStudentSql =
                    "Insert Into " + studentTableName + " " +
                    "(number, password) " +
                    "Values('%q', '%q');";
            // 插入一条座位记录
            const std::string insertNewSeatSql =
                    "Insert Into " + seatTableName + " " +
                    "(number) " +
                    "Values(%d);";
            // TODO：自增长主键需要配合`sqlite3_last_insert_rowid`使用，获取自增长的id值
            // 插入一条新的座位占用时间
            const std::string insertNewOccupiedTimeSql =
                    "Insert Into " + occupiedTimeTableName + " " +
                    "(seatNum, time) " +
                    "Values(%d, '%q');";
            // 插入一条新的预约记录
            const std::string insertNewAppointmentSql =
                    "Insert Into " + appointmentRecordTableName + " " +
                    "(studentNum, appointmentInfo, status) " +
                    "Values('%q', %d, %d);";
        }

        // 查询相关的SQL语句模板
        inline namespace SelectSQLTemplate {
            // 查询某位同学是否存在
            const std::string selectStudentSql =
                    "Select number From " + studentTableName + " Where number = '%q';";
            // 查询所有的座位号
            const std::string selectAllSeatsSql =
                    "Select number From " + seatTableName + ";";
            // 查询某一座位被占用的时间段
            const std::string selectOccupiedTimeOfSeatSql =
                    "Select time From " + occupiedTimeTableName + " Where seatNum = %d;";
            // 查询某位同学的所有预约记录
            const std::string selectAllAppointmentsOfStudentSql =
                    "Select " +
                    appointmentRecordTableName + ".id, " +
                    occupiedTimeTableName + ".seatNum, " +
                    occupiedTimeTableName + ".time, " +
                    appointmentRecordTableName + ".status " +
                    "From " + occupiedTimeTableName + " Inner Join " + appointmentRecordTableName + " " +
                    "On " + occupiedTimeTableName + ".id = " + appointmentRecordTableName + ".appointmentInfo " +
                    "Where " + appointmentRecordTableName + ".studentNum = '%q';";
        }

        // 更新相关的SQL语句模板
        inline namespace UpdateSQLTemplate {
            // 更新一条预约记录的占用时间段
            const std::string updateSeatOccupiedTimeSql =
                    // 设置更新后的时间
                    // 连接占用时间表与预约记录表
                    "Update " + occupiedTimeTableName + " Set time = '%q' " +
                    // 与传入id相等
                    "Where " + occupiedTimeTableName + ".id = %d;";
            // 取消一条预约，并将其状态修改为已履约
            const std::string updateAppointmentStatusSql =
                    // 更新状态
                    "Update " + appointmentRecordTableName + " Set status = %d " +
                    "Where " + appointmentRecordTableName + ".id = %d;";
        }
    }

}

#endif // CONSTVALUE_H
