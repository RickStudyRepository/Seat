#include "makeappointment.h"
#include "../tools.h"
#include <QMessageBox>
#include <QDebug>

MakeAppointment::MakeAppointment(QWidget *parent) : QWidget(parent) {
    initSeats();
    initLayout();
    initTimeDialog();

    // 设置滚动区域
    scrollArea->setWidget(makeAppointment);
    scrollArea->setFixedSize(795, 417);
}

void MakeAppointment::initSeats() {
    SeatWidget* temp;
    for (int i = 0; i < 100; i++) {
        temp = new SeatWidget(QPixmap(":/images/Seat.ico"), i + 1);
        // 绑定选择时间对话框槽函数
        connect(temp, SIGNAL(sendSeatNum(int)), this, SLOT(callTimeDialog(int)));
        layout->addWidget(temp, i / 7, i % 7, 1, 1);
    }
}

void MakeAppointment::initLayout() {
    layout->setSpacing(11);
    layout->setMargin(2);
    makeAppointment->setLayout(layout);
}

void MakeAppointment::initTimeDialog() {
    timeDialog->setWindowTitle(selectTimeString);
    // 接收选定的时间范围
    connect(timeDialog, SIGNAL(sendTimeScope(TimeScope)), this, SLOT(receiveTimeScope(TimeScope)));
    // 接收是否选择了时间段
    connect(timeDialog, SIGNAL(notSelectedTime()), this, SLOT(notSelected()));
}

void MakeAppointment::callTimeDialog(int seatNum) {
    // 记录被选中的座位编号
    selectedSeatNum = seatNum;
    // TODO: call database here
    // 这里需要使用数据库获取该座位的可用时间段
    AvailableTimes test;
    test.push_back(TimeScope(8, 12));
    test.push_back(TimeScope(14, 22));

    // 设置对话框的可用时间段
    timeDialog->setTimeScope(test);
    timeDialog->show();
}

void MakeAppointment::hideTimeDialog() {
    if (timeDialog->isVisible()) {
        timeDialog->close();
    }
}

void MakeAppointment::receiveTimeScope(TimeScope timeScope) {
    this->timeScope = timeScope;
    QString timeScopeString =
            Tools::intToTimeString(timeScope.first) +
            tr("-") +
            Tools::intToTimeString(timeScope.second);
    // 询问用户是否确认预约
    if (QMessageBox::question(
                             this,
                             tr("预约确认"),
                             tr("座位号：") + QString::number(selectedSeatNum) +
                             tr("\n时间：") + timeScopeString +
                             tr("\n请确认！")
                         ) == QMessageBox::Yes) {
        // write database here
        qDebug() << "确认预约";
    }
}

void MakeAppointment::notSelected() {
    // 若呼出选择时间对话框而没有选择时间段，则重置选中的编号和选中的时间段
    selectedSeatNum = -1;
    timeScope = TimeScope(-1, -1);
}
