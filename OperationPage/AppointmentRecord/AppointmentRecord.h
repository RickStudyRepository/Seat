#ifndef APPOINTMENTRECORD_H
#define APPOINTMENTRECORD_H

#include <QWidget>
#include <QHeaderView>
#include <QTableWidget>
#include <QStringList>
#include <QVBoxLayout>
#include <QString>
#include "../../Database/Database.h"
#include "OperationAndStatus.h"
#include "ContinueTimeDialog.h"
#include "../../Tools/AliasName.h"
#include "../../Tools/Dialog/ConfirmDialog.h"
#include "../../Tools/Dialog/AutoCloseMessageBox.h"

class AppointmentRecord : public QWidget
{
    Q_OBJECT

// 数据库相关
private:
    Database* database;
    void initDatabase();

// 界面相关
private:
    // 布局
    QVBoxLayout* layout;
    void initLayout();
    // 预约人学号
    QString studentNum;
    // 预约列表
    AliasName::Appointments appointments;
    // 水平表头内容
    QStringList headStringList;
    // 表格
    QTableWidget* appointmentRecord;
    // 当前操作和状态部件的指针
    std::vector<OperationAndStatus*> osList;
    // 初始化表格格式
    void initAppointmentRecord();
    // 清空表格内容，包含操作和状态部件
    void clearAppointmentRecord();
    // 调整单元格格式
    void formatTableItem(QTableWidgetItem* item);

    // 确认取消预约对话框
    ConfirmDialog* confirmDialog;
    void initConfirmDialog();
    int cancelRowNum;

    // 续约对话框
    ContinueTimeDialog* continueTimeDialog;
    void initContinueDialog();
    int continueRowNum;

    // 错误提示对话框
    AutoCloseMessageBox* warning;
    void initWarning();

    // 绑定日志信号槽
    void connectLogString();

public:
    explicit AppointmentRecord(QWidget *parent = NULL);
    // 重置预约人学号
    void resetStudentNum(QString studentNum);
    // 重置预约列表及表格内容
    void resetAppointments();
    // 隐藏可能打开的对话框
    void hideDialog();

signals:
    void resetOperationAndStatus(std::string status);
    // 日志信息信号
    void logSignal(QString);

public slots:
    // 向用户确认取消预约
    void callConfirmDialog(int rowNum);
    // 取消预约，更新数据库和内存
    void cancelAppointment();
    // 让用户选择续约结束时间
    void callContinueDialog(int rowNum);
    // 续约，延长预约时间
    void continueAppointment(AliasName::TimeScope continueTimeScope);
};

#endif // APPOINTMENTRECORD_H
