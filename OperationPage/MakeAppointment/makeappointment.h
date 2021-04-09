#ifndef MAKEAPPOINTMENT_H
#define MAKEAPPOINTMENT_H

#include <QWidget>
#include <QGridLayout>
#include <QScrollArea>
#include "seatwidget.h"
#include "timeselectiondialog.h"

class MakeAppointment : public QWidget
{
    Q_OBJECT
public:
    explicit MakeAppointment(QWidget *parent = nullptr);
    // 让上层部件调用该方法隐藏对话框
    void hideTimeDialog();

private:
    // 顶层部件
    QWidget* makeAppointment = new QWidget(this);
    // 部件布局
    QGridLayout* layout = new QGridLayout(this);
    // 使用一个可滚动的区域
    QScrollArea* scrollArea = new QScrollArea(this);
    // 时间选择对话框
    QString selectTimeString = tr("选择时间");
    TimeSelectionDialog* timeDialog = new TimeSelectionDialog(this);

    // 初始化座位
    void initSeats();
    // 初始化布局
    void initLayout();
    // 初始化时间选择对话框
    void initTimeDialog();

    // 被选中的座位的编号
    int selectedSeatNum = -1;
    // 选择的可用时间段
    TimeScope timeScope = TimeScope(-1, -1);

signals:

private slots:
    // 呼出选择时间对话框
    void callTimeDialog(int seatNum);
    // 接收选择的时间段
    void receiveTimeScope(TimeScope timeScope);
    // 接收选择状态
    void notSelected();
};

#endif // MAKEAPPOINTMENT_H
