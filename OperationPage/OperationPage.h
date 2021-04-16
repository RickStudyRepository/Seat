#ifndef OPERATIONPAGE_H
#define OPERATIONPAGE_H

#include <QWidget>
#include <QLabel>
#include <QString>
#include <QVBoxLayout>
#include <QTabWidget>
#include <QFrame>
#include "MakeAppointment/MakeAppointment.h"
#include "AppointmentRecord/AppointmentRecord.h"
#include "HeadWidget/HeadWidget.h"

class OperationPage : public QWidget
{
    Q_OBJECT
private:
    // 操作界面顶部部件
    HeadWidget* headWidget;
    void initHeadWidget();

    // 头部和操作之间的分割线
    QFrame* horizontalLine;
    void initHorizontalLine();

    // 操作部件容纳Tab
    QTabWidget* operationTab;
    // 现场预约部件
    MakeAppointment* makeAppoinment;
    // 我的预约部件
    AppointmentRecord* appointmentRecord;
    void initOperationTab();

    // 布局
    QVBoxLayout* layout;
    // 初始化布局
    void initLayout();

    // 绑定日志信号
    void connectLogString();

public:
    explicit OperationPage(QWidget *parent = NULL);

    void resetStudentNum(QString studentNum);

signals:
    void returnHomePage();
    // 日志信息信号
    void logSignal(QString);

public slots:
    // 展示操作界面
    void gotoFront(QString studentNum);
    // 隐藏操作界面
    void gotoBack();
    // 载入学号为studentNum的预约信息
    void loadAppointments(int index);
    // 释放返回主界面的信号
    void emitReturnHomePage();
};

#endif // OPERATIONPAGE_H
