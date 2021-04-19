#include "Database.h"
#include <algorithm>
#include <QFile>
#include <QDebug>

// 初始化单例句柄
Database* Database::singleDatabase = NULL;

Database *&Database::getSingleDatabase(QWidget* parent) {
    // 若没有声明一个实例，则new一个实例并返回
    if (Database::singleDatabase == NULL) {
        Database::singleDatabase = new Database(parent);
    }
    return Database::singleDatabase;
}

Database::Database(QWidget* parent) : QWidget(parent) {
    database = NULL;
    // 初始化状态字符串和状态值之间的对应关系
    initStatusStringIntMap();
    // 检查数据库文件是否存在
    isInited = QFile::exists(QString::fromStdString(ConstValue::databaseName));
    // 打开数据库成功则检查是否需要初始化数据库
    if (openDatabase() == true) {
        // 如果数据库文件不存在，则执行建表方法
        if (isInited == false) {
            // 如果初始化数据库失败，关闭数据库
            if (initDatabase() == false) {
                closeDatabase();
            }
        }
        // 数据库文件已存在，不再执行建表的方法
        else {
            qDebug() << "Init database successfully! Database already exists";
        }
    }
    // 打开失败关闭数据库，释放SQLite内部可能占用的资源
    else {
        closeDatabase();
    }
}

// 通过给数据库找一个父亲，让QT的对象树帮我们调用delete，
// 利用delete会调用析构函数的特性实现空间的释放及数据库的关闭
Database::~Database() {
    closeDatabase();
}

// 打开数据库
bool Database::openDatabase() {
    if (database == NULL) {
        int result = sqlite3_open(ConstValue::databaseName.c_str(), &database);
        if (result != SQLITE_OK) {
            qDebug() << tr("Database：open database ") +
                        QString::fromStdString(ConstValue::databaseName) +
                        tr(" failed，ERROR CODE: ") + QString::number(result) +
                        tr("ErrorMessage：") + QString(sqlite3_errmsg(database));
            emit logSignal(
                        tr("数据库：打开数据库") +
                        QString::fromStdString(ConstValue::databaseName) +
                        tr("失败，ERROR CODE: ") + QString::number(result) +
                        tr("错误信息：") + QString(sqlite3_errmsg(database))
            );
            return false;
        }
    }
    qDebug() << "Open database successfully!";
    return true;
}

bool Database::closeDatabase() {
    if (database != NULL) {
        if (sqlite3_close(database) == SQLITE_BUSY) {
            qDebug() << "Close database failed!\n"
                     << "Please use sqlite3_finalize() to deallocate SQL statement"
                     << "prepared by using sqlite3_prepare() or sqlite3_prepare16().";
            emit logSignal(tr("数据库：关闭数据库失败，请检查是否有未销毁的预编译SQL语句"));
            return false;
        }
        database = NULL;
    }
    qDebug() << "Database: close database";
    return true;
}

bool Database::initDatabase() {
    if (createStudentTable() == false) {
        return false;
    }
    qDebug() << "Database: create student table in init database";

    if (createSeatTable() == false) {
        return false;
    }
    qDebug() << "Database: create seat table in init database";

    if (insertSeats() == false) {
        return false;
    }
    qDebug() << "Database: insert seats in init database";

    if (createOccupiedTimeTable() == false) {
        return false;
    }
    qDebug() << "Database: create occupied time table in init database";

    if (createAppointmentRecordTable() == false) {
        return false;
    }
    qDebug() << "Database: create appointment record table in init database";

    // 将数据库成功初始化的标志设为true
    isInited = true;

    qDebug() << "Init database successfully!";
    // 所有的表均创建成功，返回true
    return true;
}

bool Database::createStudentTable() {
    int result;
    char* errorMessage;
    emit logSignal(tr("数据库：待执行的SQL语句：") + QString::fromStdString(ConstValue::creatStudentTableSql));
    qDebug() << "Database: create student table sql:" << QString::fromStdString(ConstValue::creatStudentTableSql);
    // 创建学生表
    result = sqlite3_exec(
                database,
                ConstValue::creatStudentTableSql.c_str(),
                NULL,
                NULL,
                &errorMessage
    );
    if (result != SQLITE_OK) {
        emit logSignal(tr("数据库：创建学生表失败，错误码为：") + QString::number(result) + tr("，错误提示：") + QString(errorMessage));
        qDebug() << "Creat Student Table failed! ERROR Code:" << result << "ErrorMessage:" << QString(errorMessage);
        sqlite3_free(errorMessage);
        return false;
    }
    return true;
}

bool Database::createSeatTable() {
    int result;
    char* errorMessage;
    emit logSignal(tr("数据库：待执行的SQL语句：") + QString::fromStdString(ConstValue::creatSeatTableSql));
    qDebug() << "Database: sql:" << QString::fromStdString(ConstValue::creatSeatTableSql);
    // 创建座位表
    result = sqlite3_exec(
                database,
                ConstValue::creatSeatTableSql.c_str(),
                NULL,
                NULL,
                &errorMessage
    );
    if (result != SQLITE_OK) {
        emit logSignal(tr("数据库：创建座位表失败，错误码为：") + QString::number(result) + tr("，错误提示：") + QString(errorMessage));
        qDebug() << "Creat Seat Table failed! ERROR Code:" << result << "ErrorMessage:" << QString(errorMessage);
        sqlite3_free(errorMessage);
        return false;
    }

    return true;
}

// 插入一定数量的座位
bool Database::insertSeats() {
    char* realInsertSql = NULL;
    for (size_t i = 0; i < ConstValue::seatNums; i++) {
        // 构造真实的SQL语句
        realInsertSql = sqlite3_mprintf(
                    ConstValue::insertNewSeatSql.c_str(),
                    i + 1
        );
        // 执行插入座位的SQL语句
        result = sqlite3_exec(
                    database,
                    realInsertSql,
                    NULL,
                    NULL,
                    &errorMessage
        );
        // 释放sqlite3_mprintf内部申请的动态内存
        sqlite3_free(realInsertSql);
        if (result != SQLITE_OK) {
            emit logSignal(tr("数据库：创建学生表失败，错误码为：") + QString::number(result) + tr("，错误提示：") + QString(errorMessage));
            qDebug() << "Insert seat failed at" << i + 1 << ", ERROR Code:" << result << "ErrorMessage:" << QString(errorMessage);
            // 释放SQLite内部申请的动态内存
            sqlite3_free(errorMessage);
            return false;
        }
    }
    return true;
}

bool Database::createOccupiedTimeTable() {
    int result;
    char* errorMessage;
    emit logSignal(tr("数据库：待执行的SQL语句：") + QString::fromStdString(ConstValue::creatOccupiedTimeSql));
    qDebug() << "Database: sql:" << QString::fromStdString(ConstValue::creatOccupiedTimeSql);
    // 创建占用时间表
    result = sqlite3_exec(
                database,
                ConstValue::creatOccupiedTimeSql.c_str(),
                NULL,
                NULL,
                &errorMessage
    );
    // 创建失败
    if (result != SQLITE_OK) {
        emit logSignal(tr("数据库：创建占用时间表失败，错误码为：") + QString::number(result) + tr("，错误提示：") + QString(errorMessage));
        qDebug() << "Creat OccupiedTime Table failed! ERROR Code:" << result << "ErrorMessage:" << QString(errorMessage);
        sqlite3_free(errorMessage);
        return false;
    }
    return true;
}

bool Database::createAppointmentRecordTable() {
    int result;
    char* errorMessage;
    emit logSignal(tr("数据库：待执行的SQL语句：") + QString::fromStdString(ConstValue::creatAppointmentRecordTableSql));
    qDebug() << "Database: sql:" << QString::fromStdString(ConstValue::creatAppointmentRecordTableSql);
    // 创建预约记录表
    result = sqlite3_exec(
                database,
                ConstValue::creatAppointmentRecordTableSql.c_str(),
                NULL,
                NULL,
                &errorMessage
    );
    // 创建失败
    if (result != SQLITE_OK) {
        emit logSignal(tr("数据库：创建学生表失败，错误码为：") + QString::number(result) + tr("，错误提示：") + QString(errorMessage));
        qDebug() << "Creat AppointmentRecord Table failed! ERROR Code:" << result << "ErrorMessage:" << QString(errorMessage);
        sqlite3_free(errorMessage);
        return false;
    }
    return true;
}

void Database::initStatusStringIntMap() {
    statusStringInt.push_back(make_pair(1, ConstValue::UsedSeat));
    statusStringInt.push_back(make_pair(2, ConstValue::UnusedSeat));
    statusStringInt.push_back(make_pair(3, ConstValue::UsingSeat));
    statusStringInt.push_back(make_pair(4, ConstValue::NotSigned));
}

int Database::statusStringToInt(const std::string status) {
    for (size_t i = 0; i < statusStringInt.size(); i++) {
        if (status.compare(statusStringInt[i].second) == 0) {
            return statusStringInt[i].first;
        }
    }
    return -1;
}

std::string Database::intToStatusString(const int status) {
    for (size_t i = 0; i < statusStringInt.size(); i++) {
        if (statusStringInt[i].first == status) {
            return statusStringInt[i].second;
        }
    }
    return "";
}

// 查询学号是否已经存在
bool Database::isStudentExists(std::string studentNum, bool* success) {
    bool result = false;
    // 构造查询SQL语句
    char* realSelectSql = sqlite3_mprintf(
                ConstValue::selectStudentSql.c_str(),
                studentNum.c_str()
    );
    emit logSignal(tr("数据库：待执行的SQL语句：") + QString(realSelectSql));

    // 预编译结果
    int prepareResult;
    // 预编译得到的SQL语句
    sqlite3_stmt* preparedSql = NULL;
    // 未被编译的SQL语句的起始指针
    const char* unpreparedSqlPointer = NULL;
    // 预编译SQL语句
    prepareResult = sqlite3_prepare_v2(
                database,
                realSelectSql,
                -1,
                &preparedSql,
                &unpreparedSqlPointer
    );

    // 释放sqlite3_mprintf内部申请的动态内存
    sqlite3_free(realSelectSql);

    // 预编译失败
    if (prepareResult != SQLITE_OK) {
        emit logSignal(tr("数据库：预编译SQL语句失败，错误码：") + QString::number(prepareResult));
        *success = false;
        return result;
    }

    // 查询结果
    int selectResult;
    // 预编译成功
    selectResult = sqlite3_step(preparedSql);
    // 若有一行，那么学号一定存在
    if (selectResult == SQLITE_ROW) {
        result = true;
    }
    // 查询成功
    *success = true;
    // 释放预编译得到的SQL语句占用的动态内存
    sqlite3_finalize(preparedSql);

    return result;
}

// 插入新的学号
bool Database::insertNewStudent(std::string studentNum) {
    emit logSignal(tr("数据库：插入新的学号：") + QString::fromStdString(studentNum));
    // 构造插入SQL语句
    char* realInsertSql = sqlite3_mprintf(
                ConstValue::insertNewStudentSql.c_str(),
                studentNum.c_str(),
                // 默认密码为学号
                studentNum.c_str()
    );
    emit logSignal(tr("数据库：待执行的SQL语句：") + QString(realInsertSql));

    int result;
    char* errorMessage = NULL;
    result = sqlite3_exec(
                database,
                realInsertSql,
                NULL,
                NULL,
                &errorMessage
    );
    // 释放sqlite3_mprintf内部申请的动态内存
    sqlite3_free(realInsertSql);
    // 插入失败
    if (result != SQLITE_OK) {
        emit logSignal(tr("数据库：插入新的学号失败，错误码为：") + QString::number(result) + tr("错误提示：") + QString(errorMessage));
        // 释放错误信息字符串占用的动态内存
        sqlite3_free(errorMessage);
        return false;
    }
    emit logSignal(tr("数据库：插入新的学号成功"));
    return true;
}

// 插入新的占用时间记录
bool Database::insertNewOccupiedTime(const int seatNum, const std::string time) {
    emit logSignal(tr("数据库：插入新的座位占用时间"));
    // 构造插入座位占用时间的SQL语句
    char* realInsertSql = sqlite3_mprintf(
                ConstValue::insertNewOccupiedTimeSql.c_str(),
                seatNum,
                time.c_str()
    );
    emit logSignal(tr("数据库：待执行的SQL语句：") + QString(realInsertSql));
    char* errorMessage;
    int result;
    result = sqlite3_exec(
                database,
                realInsertSql,
                NULL,
                NULL,
                &errorMessage
    );
    // 释放sqlite3_mprintf申请的动态内存
    sqlite3_free(realInsertSql);
    if (result != SQLITE_OK) {
        emit logSignal(tr("数据库：插入占用时间失败，错误提示：") + QString(errorMessage));
        // 释放SQLite内部申请的动态内存
        sqlite3_free(errorMessage);
        return false;
    }

    emit logSignal(tr("数据库：插入新的座位占用时间成功"));
    return true;
}

// 插入一条新的预约记录
bool Database::makeNewAppoinment(AliasName::Appointment newAppointment) {
    emit logSignal(tr("数据库：插入新的预约记录"));
    // 如果插入占用时间失败，则直接返回
    if (false == insertNewOccupiedTime(newAppointment.seatNum, newAppointment.time)) {
        emit logSignal(tr("数据库：插入新的占用时间失败，结束插入预约记录"));
        return false;
    }

    // 获取新插入的占用时间记录自增的主键值
    int occupiedTimeId = sqlite3_last_insert_rowid(database);
    qDebug() << "Status in make appointment:" << QString::fromStdString(newAppointment.status);
    // 构造插入预约记录的SQL语句
    char* realInsertSql = sqlite3_mprintf(
                ConstValue::insertNewAppointmentSql.c_str(),
                newAppointment.studentNum.c_str(),
                occupiedTimeId,
                statusStringToInt(newAppointment.status)
    );
    emit logSignal(tr("数据库：待执行的SQL语句：") + QString(realInsertSql));
    char* errorMessage;
    int result = sqlite3_exec(
                database,
                realInsertSql,
                NULL,
                NULL,
                &errorMessage
    );
    // 释放sqlite3_mprintf申请的动态内存
    sqlite3_free(realInsertSql);

    if (result != SQLITE_OK) {
        emit logSignal(tr("数据库：插入预约记录失败，错误码为：") + QString::number(result) + tr("错误提示：") + QString(errorMessage));
        // 释放errorMessage指向的异常信息占用的动态内存
        sqlite3_free(errorMessage);
        return false;
    }
    // 获取自增长生成的预约记录号
    int appointmentRecordId = sqlite3_last_insert_rowid(database);
    emit logSignal(tr("数据库：插入预约记录成功，预约ID为：") + QString::number(appointmentRecordId));
    return true;
}

// 获取学号为studentNum的学生的所有预约记录
AliasName::Appointments Database::getAllAppointmentsOf(const std::string studentNum, bool* success) {
    emit (tr("数据库：开始查找学号为") + QString::fromStdString(studentNum) + tr("的学生的所有预约记录"));
    // 读出的结果
    AliasName::Appointments appointments;
    // 构造select语句
    char* realSelectSql = sqlite3_mprintf(
                ConstValue::selectAllAppointmentsOfStudentSql.c_str(),
                studentNum.c_str()
    );
    emit logSignal(tr("数据库：待执行的SQL语句") + QString(realSelectSql));

    // 未进行预编译的SQL语句起始指针
    const char* unpreparedSqlPointer = NULL;
    // 预编译出的SQL语句
    sqlite3_stmt* preparedSql = NULL;
    // 预编译SQL语句
    int prepareResult = sqlite3_prepare_v2(
                database,
                realSelectSql,
                // -1表示编译传入的整个字符串
                -1,
                &preparedSql,
                &unpreparedSqlPointer
    );
    // 释放sqlite3_mprintf占用的动态内存
    sqlite3_free(realSelectSql);

    // 预处理失败
    if (prepareResult != SQLITE_OK) {
        emit logSignal(tr("数据库：预编译SQL语句失败，错误码：") + QString::number(prepareResult));
        *success = false;
        return appointments;
    }

    // 预处理成功，循环拿出数据
    int selectResult;
    // 临时存储读出的数据
    int id;
    int seatNum;
    std::string time;
    std::string status;
    while(true) {
        selectResult = sqlite3_step(preparedSql);
        // 如果没有后续数据，则退出循环
        if (selectResult != SQLITE_ROW) {
            // 销毁预编译的SQL语句
            sqlite3_finalize(preparedSql);
            emit logSignal(tr("数据库：读取结束，销毁预编译出的SQL语句"));
            break;
        }
        // 取出本行的数据
        id = sqlite3_column_int(preparedSql, 0);
        seatNum = sqlite3_column_int(preparedSql, 1);
        // 这里的强转可能是不安全的
        // 使用这个语句不能编译通过，主要是该接口返回了const unsigned char*
//        time.assign(sqlite3_column_text(preparedSql, 2));
        time.assign((const char*)sqlite3_column_text(preparedSql, 2));
        status = intToStatusString(sqlite3_column_int(preparedSql, 3));
        qDebug() << id << seatNum << QString::fromStdString(time) << intStatus << QString::fromStdString(status);
        appointments.push_back(AliasName::Appointment(id, seatNum, time, status));
    }

    emit logSignal(tr("数据库：获取预约记录成功"));
    *success = true;
    return appointments;
}

// 获取所有的座位号（这里可以同时获取座位的可用时间段，实现对座位不同状态不同颜色的演示）
AliasName::SeatInfos Database::getAllSeats(bool* success) {
    // 获取的结果
    AliasName::SeatInfos seatInfos;

    emit logSignal(tr("数据库：获取所有座位号"));
    emit logSignal(tr("数据库：待执行的SQL语句：") + QString::fromStdString(ConstValue::selectAllSeatsSql));
    // 预编译结果
    int preparedResult;
    // 预编译得到的SQL语句
    sqlite3_stmt* preparedSql = NULL;
    // 未被预编译的SQL语句的起始指针
    const char* unpreparedSqlPointer = NULL;
    preparedResult = sqlite3_prepare_v2(
                database,
                ConstValue::selectAllSeatsSql.c_str(),
                -1,
                &preparedSql,
                &unpreparedSqlPointer
    );

    // 预编译失败，结束执行
    if (preparedResult != SQLITE_OK) {
        emit logSignal(tr("数据库：预编译SQL语句失败，错误码：") + QString::number(preparedResult));
        *success = false;
        return seatInfos;
    }

    // 预编译成功，循环获取结果
    int seatNum;
    // 查询结果
    int selectResult;
    while(true) {
        selectResult = sqlite3_step(preparedSql);
        // 没有后续记录，结束执行
        if (selectResult != SQLITE_ROW) {
            // 销毁预编译得到的SQL语句
            sqlite3_finalize(preparedSql);
            emit logSignal(tr("数据库：读取结束，销毁预编译得到的SQL语句"));
            break;
        }
        seatNum = sqlite3_column_int(preparedSql, 0);
        seatInfos.push_back(AliasName::SeatInfo(seatNum));
    }

    emit logSignal(tr("数据库：获取所有座位号成功"));
    *success = true;
    return seatInfos;
}

// 更新某一预约的占用时间段
// TODO：考虑在内部添加检验是否合法的逻辑
bool Database::continueAppointment(int appointmentId, const std::string newTime) {
    emit logSignal(tr("数据库：续约编号为") + QString::number(appointmentId) + tr("的预约记录"));
    // 构造更新SQL语句
    char* realUpdateSql = sqlite3_mprintf(
                ConstValue::updateSeatOccupiedTimeSql.c_str(),
                newTime.c_str(),
                appointmentId
    );
    emit logSignal(tr("数据库：待执行的SQL语句：") + QString(realUpdateSql));
    // 保存异常信息字符串的首地址
    char* errorMessage;
    int result = sqlite3_exec(
                database,
                realUpdateSql,
                NULL,
                NULL,
                &errorMessage
    );
    // 释放sqlite3_mprintf内部申请的动态内存
    sqlite3_free(realUpdateSql);

    if (result != SQLITE_OK) {
        emit logSignal(tr("数据库：更新失败，错误码为：") + QString::number(result) + tr("，错误提示：") + QString(errorMessage));
        // 释放错误信息占用的动态内存
        sqlite3_free(errorMessage);
        return false;
    }

    emit logSignal(tr("数据库：更新预约记录成功"));
    return true;
}

// 取消预约
bool Database::cancelAppointment(int appointmentId) {
    emit logSignal(tr("数据库：取消预约ID为") + QString::number(appointmentId) + tr("的预约"));

    // 构造更新SQL语句
    char* realUpdateSql = sqlite3_mprintf(
                ConstValue::updateAppointmentStatusSql.c_str(),
                statusStringToInt(ConstValue::UsedSeat),
                appointmentId
    );
    emit logSignal(tr("数据库：待执行的SQL语句：") + QString(realUpdateSql));

    // 存储异常字符串的首地址
    char* errorMessage;
    // 执行结果
    int result = sqlite3_exec(
                database,
                realUpdateSql,
                NULL,
                NULL,
                &errorMessage
    );
    // 释放sqlite3_mprintf内部申请的动态内存
    sqlite3_free(realUpdateSql);

    // 执行失败
    if (result != SQLITE_OK) {
        emit logSignal(tr("数据库：取消预约失败，错误码为：") + QString::number(result) + tr("，错误提示：") + QString(errorMessage));
        // 释放异常信息占用的动态内存
        sqlite3_free(errorMessage);
        return false;
    }

    emit logSignal(tr("数据库：取消预约成功"));
    return true;
}

// 获取某一座位的不可用时间段
AliasName::TimeScopes Database::getUnavailableTimeScopesOf(int seatNum, bool* success) {
    emit logSignal(tr("数据库：获取座位号为") + QString::number(seatNum) + tr("的已占用时间"));

    // 查询结果
    AliasName::TimeScopes unavailableTimeScopes;

    // 构造选择SQL语句
    char* realSelectSql = sqlite3_mprintf(
                ConstValue::selectOccupiedTimeOfSeatSql.c_str(),
                seatNum
    );
    emit logSignal(tr("数据库：待执行的SQL语句：") + QString(realSelectSql));

    // 预编译的结果
    int prepareResult;
    // 预编译得到的SQL语句
    sqlite3_stmt* preparedSql = NULL;
    // 未被预编译到的SQL语句起始地址
    const char* unpreparedSqlPointer = NULL;
    // 执行预编译
    prepareResult = sqlite3_prepare_v2(
                database,
                realSelectSql,
                -1,
                &preparedSql,
                &unpreparedSqlPointer
    );
    // 释放sqlite3_mprintf内部申请的动态内存
    sqlite3_free(realSelectSql);

    // 预编译失败
    if (prepareResult != SQLITE_OK) {
        *success = false;
        emit logSignal(tr("数据库：预编译SQL语句失败，结束查询，错误码为：") + QString::number(prepareResult));
        return unavailableTimeScopes;
    }

    // 临时存储读出的time
    std::string time;
    // 读取结果
    int selectResult;
    while(true) {
        selectResult = sqlite3_step(preparedSql);
        if (selectResult != SQLITE_ROW) {
            emit logSignal(tr("数据库：读取结束，销毁预编译出的SQL语句"));
            // 销毁预编译得到的SQL语句
            sqlite3_finalize(preparedSql);
            break;
        }
        // TODO：这里的强转可能不安全
        time.assign((const char*)sqlite3_column_text(preparedSql, 0));
        unavailableTimeScopes.push_back(Tools::databaseTimeToTimeScope(time));
    }

    emit logSignal(tr("数据库：获取座位已占用时间段成功"));
    *success = true;
    return unavailableTimeScopes;
}

// 获取某一个座位的可用时间范围
AliasName::TimeScopes Database::getAvailableTimeScopesOf(int seatNum, bool* success) {
    emit logSignal(tr("数据库：开始获取编号为") + QString::number(seatNum) + tr("的座位的可用时间段"));

    // 获取不可用时间段
    AliasName::TimeScopes unavailableTimeScopes = getUnavailableTimeScopesOf(seatNum, success);

    // 未被占用的时间段
    AliasName::TimeScopes availableTimeScopes;

    // 获取占用时间段失败，结束计算
    if (*success == false) {
        emit logSignal(tr("数据库：获取不可用时间失败，结束计算可用时间范围"));
        return availableTimeScopes;
    }
    // 获取成功，开始计算
    // 对不可用时间段进行排序
    std::sort(unavailableTimeScopes.begin(), unavailableTimeScopes.end());
    // 临时存储选取的开始时间
    int start = ConstValue::maxTimeScope.first;
    // 存储最晚结束时间
    int end = ConstValue::maxTimeScope.second;
    size_t size = unavailableTimeScopes.size();
    for (size_t i = 0; i < size && start < end; i++) {
        // 如果上一轮的结束时间小于本轮的开始时间，说明上一轮和本轮中间有可用时间段
        if (start < unavailableTimeScopes[i].first) {
            availableTimeScopes.push_back(AliasName::TimeScope(start, unavailableTimeScopes[i].first));
        }
        start = unavailableTimeScopes[i].second;
    }
    // 在遍历完所有的不可用时间段之后，可能最后还会有一段可用时间段
    if (start < end) {
        availableTimeScopes.push_back(AliasName::TimeScope(start, end));
    }

    emit logSignal(tr("数据库：计算可用时间段成功"));
    *success = true;
    return availableTimeScopes;
}
