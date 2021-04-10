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
    // 顶层部件
    QWidget* operationPage = new QWidget(this);
    // 布局
    QVBoxLayout* layout = new QVBoxLayout(this);

    // 操作界面顶部部件
    HeadWidget* headWidget = new HeadWidget(this);

    // 头部和操作之间的分割线
    QFrame* horizontalLine = new QFrame(this);
    void initHorizontalLine();

    // 操作部件容纳Tab
    QTabWidget* operationTab = new QTabWidget(this);
    void initOperationTab();

    // 现场预约部件
    MakeAppointment* makeAppoinment = new MakeAppointment(this);
    QString makeAppoinmentString = tr("现场预约");

    // 我的预约部件
    AppointmentRecord* appointmentRecord = new AppointmentRecord(this);
    QString appointmentRecordString = tr("我的预约");

    // 初始化布局
    void initLayout();

public:
    explicit OperationPage(QWidget *parent = NULL);

    void resetStudentNum(QString studentNum);

signals:
    void returnHomePage();

public slots:
    // 展示操作界面
    void gotoFront(QString studentNum);
    // 隐藏操作界面
    void gotoBack();
    // 释放返回主界面的信号
    void emitReturnHomePage();
};

#endif // OPERATIONPAGE_H
