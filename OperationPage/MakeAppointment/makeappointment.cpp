#include "MakeAppointment.h"
#include "../../Tools/AliasName.h"
#include "../../Tools/Tools.h"
#include "../../Database/Database.h"
#include <QDebug>

MakeAppointment::MakeAppointment(QWidget *parent)
    : QWidget(parent), selectedSeatNum(-1), timeScope(AliasName::TimeScope(-1, -1))
{
    initSeats();
    initLayout();
    initScrollArea();

    initTimeDialog();
    initConfirmDialog();
    connectLogString();
}

void MakeAppointment::initSeats() {
    // TODO:call database here
    // 要在这里载入所有座位的可用时间段
    SeatWidget* temp;
    for (int i = 0; i < 100; i++) {
        temp = new SeatWidget(QPixmap(":/images/Seat.png"), i + 1);
        // 绑定选择时间对话框槽函数
        connect(temp, SIGNAL(sendSeatNum(int)), this, SLOT(callTimeDialog(int)));
        seats.push_back(temp);
    }
}

void MakeAppointment::initLayout() {
    layout = new QGridLayout(this);
    layout->setSpacing(10);
    layout->setMargin(2);

    // 将所有的座位添加到布局中
    size_t seatNum = seats.size();
    for (size_t i = 0; i < seatNum; i++) {
        layout->addWidget(seats[i], i / 7, i % 7, 1, 1);
    }

    makeAppointment = new QWidget(this);
    makeAppointment->setLayout(layout);
}

void MakeAppointment::initScrollArea() {
    scrollArea = new QScrollArea(this);
    // 设置滚动区域
    scrollArea->setWidget(makeAppointment);
    // 设置最大的大小
    scrollArea->setFixedSize(795, 387);
}

void MakeAppointment::initTimeDialog() {
    timeDialog = new TimeSelectionDialog(this);
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
    AliasName::TimeScopes availableTimes = Database::getAvailableTimesOf(seatNum);

    // 设置对话框的可用时间段
    timeDialog->setTimeScopeAndShow(availableTimes);
    emit logSignal(tr("现场预约：呼出选择时间对话框"));
}

void MakeAppointment::initConfirmDialog() {
    confirmDialog = new ConfirmDialog(this);
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
    emit logSignal(
                tr("现场预约：呼出确认预约对话框") + tr("\n") +
                tr("确认信息内容为：\n") +
                text
    );
}

void MakeAppointment::connectLogString() {
    connect(timeDialog, SIGNAL(logSignal(QString)), this, SIGNAL(logSignal(QString)));
    connect(confirmDialog, SIGNAL(logSignal(QString)), this, SIGNAL(logSignal(QString)));
}

void MakeAppointment::hideDialog() {
    // 关闭确认对话框
    if (confirmDialog->isVisible()) {
        confirmDialog->close();
        emit logSignal(tr("现场预约：自动关闭确认预约对话框"));
    }
    // 关闭时间选择对话框
    if (timeDialog->isVisible()) {
        timeDialog->close();
        emit logSignal(tr("现场预约：自动关闭时间选择对话框"));
    }
    // 以防万一，重置座位号和时间段
    resetSeatAndTimeScope();
}

void MakeAppointment::receiveTimeScope(AliasName::TimeScope timeScope) {
    // 接收时间段
    this->timeScope = timeScope;
    emit logSignal(
                tr("现场预约：接收时间范围：") +
                QString::number(timeScope.first) + "-" +
                QString::number(timeScope.second)
    );
    // 询问用户是否确认预约
    callConfirmDialog();
}

void MakeAppointment::resetStudentNum(QString studentNum) {
    this->studentNum = studentNum;
    emit logSignal(tr("现场预约：重置登录的学生学号为：") + studentNum);
}

void MakeAppointment::writeAppointmentToDatabase() {
    // 构造新的预约记录
    AliasName::Appointment newAppointment(
        studentNum.toStdString(),
        selectedSeatNum,
        Tools::getCurrentDate() +
        " " +
        Tools::intToTimeString(timeScope.first).toStdString() +
        "-" +
        Tools::intToTimeString(timeScope.second).toStdString(),
        ConstValue::UsingSeat
    );
    emit logSignal(
                tr("现场预约：向数据库写入新的预约信息\n") +
                tr("预约信息：\n") +
                tr("学号：") + QString::fromStdString(newAppointment.studentNum) + "\n" +
                tr("座位号：") + QString::number(newAppointment.seatNum) + "\n" +
                tr("时间段：") + QString::fromStdString(newAppointment.time) + "\n" +
                tr("状态：") + QString::fromStdString(newAppointment.status)
    );
    // TODO:call database here
    Database::makeNewAppoinment(newAppointment);
    // 重置座位号和时间段
    resetSeatAndTimeScope();
}

void MakeAppointment::resetSeatAndTimeScope() {
    // 若呼出选择时间对话框而没有选择时间段，则重置选中的编号和选中的时间段
    selectedSeatNum = -1;
    timeScope = AliasName::TimeScope(-1, -1);
}
