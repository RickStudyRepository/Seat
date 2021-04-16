#ifndef MAKEAPPOINTMENT_H
#define MAKEAPPOINTMENT_H

#include <QWidget>
#include <QGridLayout>
#include <QScrollArea>
#include <QPushButton>
#include <QString>
#include <vector>
#include "SeatWidget.h"
#include "TimeSelectionDialog.h"
#include "../../Tools/Dialog/ConfirmDialog.h"

class MakeAppointment : public QWidget
{
    Q_OBJECT
public:
    explicit MakeAppointment(QWidget *parent = NULL);
    // 让上层部件调用该方法隐藏对话框
    void hideDialog();
    // 设置预约人学号
    void resetStudentNum(QString studentNum);

private:
    // 预约人的学号
    QString studentNum;
    // 被选中的座位的编号
    int selectedSeatNum;
    // 选择的可用时间段
    AliasName::TimeScope timeScope;

    // 初始化座位
    std::vector<SeatWidget*> seats;
    void initSeats();

    // 顶层部件，用于容纳所有的座位，以便将其设置到滚动区域中
    QWidget* makeAppointment;
    QGridLayout* layout;
    // 初始化布局
    void initLayout();

    // 因为座位比较多，使用一个滚动区域
    QScrollArea* scrollArea;
    void initScrollArea();

    // 初始化时间选择对话框
    TimeSelectionDialog* timeDialog;
    void initTimeDialog();

    // 初始化确认预约对话框
    ConfirmDialog* confirmDialog;
    void initConfirmDialog();

    // 呼出确认预约对话框，返回是否确认预约
    void callConfirmDialog();

    // 绑定日志信号槽
    void connectLogString();

signals:
    // 日志信息信号
    void logSignal(QString);

private slots:
    // 呼出选择时间对话框
    void callTimeDialog(int seatNum);

    // 接收选择的时间段
    void receiveTimeScope(AliasName::TimeScope timeScope);

    // 向数据库写入预约信息
    void writeAppointmentToDatabase();

    // 重置座位号和选择的时间段
    void resetSeatAndTimeScope();
};

#endif // MAKEAPPOINTMENT_H
