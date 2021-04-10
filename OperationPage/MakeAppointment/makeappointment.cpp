#include "MakeAppointment.h"
#include "../../Tools/AliasName.h"
#include "../../Tools/Tools.h"
#include <QDebug>

MakeAppointment::MakeAppointment(QWidget *parent) : QWidget(parent) {
    initSeats();
    initLayout();
    initTimeDialog();
    initConfirmDialog();

    // 设置滚动区域
    scrollArea->setWidget(makeAppointment);
    // 设置最大的大小
    scrollArea->setFixedSize(795, 387);
}

void MakeAppointment::initSeats() {
    // TODO:call database here
    // 要在这里载入所有座位的可用时间段
    SeatWidget* temp;
    for (int i = 0; i < 100; i++) {
        temp = new SeatWidget(QPixmap(":/images/Seat.ico"), i + 1);
        // 绑定选择时间对话框槽函数
        connect(temp, SIGNAL(sendSeatNum(int)), this, SLOT(callTimeDialog(int)));
        layout->addWidget(temp, i / 7, i % 7, 1, 1);
    }
}

void MakeAppointment::initLayout() {
    layout->setSpacing(10);
    layout->setMargin(2);
    makeAppointment->setLayout(layout);
}

void MakeAppointment::initTimeDialog() {
    timeDialog->setWindowTitle(selectTimeString);
    // 接收选定的时间范围
    connect(timeDialog, SIGNAL(sendTimeScope(AliasName::TimeScope)), this, SLOT(receiveTimeScope(AliasName::TimeScope)));
    // 接收是否选择了时间段
    connect(timeDialog, SIGNAL(notSelectedTime()), this, SLOT(resetSeatAndTimeScope()));
}

void MakeAppointment::callTimeDialog(int seatNum) {
    // 记录被选中的座位编号
    selectedSeatNum = seatNum;
    // TODO: call database here
    // 这里需要使用数据库获取该座位的可用时间段
    AliasName::TimeScopes test;
    test.push_back(AliasName::TimeScope(8, 12));
    test.push_back(AliasName::TimeScope(14, 22));

    // 设置对话框的可用时间段
    timeDialog->setTimeScopeAndShow(test);
}

void MakeAppointment::initConfirmDialog() {
    // 确认预约之后向数据库写入预约信息
    connect(confirmDialog, SIGNAL(confirmed()), this, SLOT(writeAppointmentToDatabase()));
    confirmDialog->setWindowTitle(tr("确认预约"));
    confirmDialog->setConfirmButtonText(tr("我要预约"));
}

void MakeAppointment::callConfirmDialog() {
    QString text =
            tr("座位号：") + QString::number(selectedSeatNum) +
            tr("\n时间：") +
            Tools::intToTimeString(timeScope.first) +
            tr("-") +
            Tools::intToTimeString(timeScope.second) +
            tr("\n请确认你的预约信息！");
    confirmDialog->setTextAndShow(text);
}

void MakeAppointment::hideDialog() {
    // 关闭确认对话框
    if (confirmDialog->isVisible()) {
        confirmDialog->close();
    }
    // 关闭时间选择对话框
    if (timeDialog->isVisible()) {
        timeDialog->close();
    }
    // 以防万一，重置座位号和时间段
    resetSeatAndTimeScope();
}

void MakeAppointment::receiveTimeScope(AliasName::TimeScope timeScope) {
    // 接收时间段
    this->timeScope = timeScope;
    // 询问用户是否确认预约
    callConfirmDialog();
}

void MakeAppointment::resetStudentNum(QString studentNum) {
    this->studentNum = studentNum;
}

void MakeAppointment::writeAppointmentToDatabase() {
    qDebug() << "向数据库写入预约信息：\n"
             << "预约人：" << studentNum << "\n"
             << "座位号：" << selectedSeatNum << "\n"
             << "时间段："
             << Tools::intToTimeString(timeScope.first)
             << "-"
             << Tools::intToTimeString(timeScope.second);
    // 重置座位号和时间段
    resetSeatAndTimeScope();
}

void MakeAppointment::resetSeatAndTimeScope() {
    // 若呼出选择时间对话框而没有选择时间段，则重置选中的编号和选中的时间段
    selectedSeatNum = -1;
    timeScope = AliasName::TimeScope(-1, -1);
}
