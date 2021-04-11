#ifndef MAKEAPPOINTMENT_H
#define MAKEAPPOINTMENT_H

#include <QWidget>
#include <QGridLayout>
#include <QScrollArea>
#include <QPushButton>
#include <QString>
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
    // 顶层部件
    QWidget* makeAppointment = new QWidget(this);
    // 部件布局
    QGridLayout* layout = new QGridLayout(this);
    // 使用一个可滚动的区域
    QScrollArea* scrollArea = new QScrollArea(this);
    TimeSelectionDialog* timeDialog = new TimeSelectionDialog(this);
    // 确认预约对话框
    ConfirmDialog* confirmDialog = new ConfirmDialog(this);

    // 预约人的学号
    QString studentNum;
    // 被选中的座位的编号
    int selectedSeatNum = -1;
    // 选择的可用时间段
    AliasName::TimeScope timeScope = AliasName::TimeScope(-1, -1);

    // 初始化座位
    void initSeats();
    // 初始化布局
    void initLayout();
    // 初始化时间选择对话框
    void initTimeDialog();
    // 初始化确认预约对话框
    void initConfirmDialog();
    // 呼出确认预约对话框，返回是否确认预约
    void callConfirmDialog();

signals:

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
