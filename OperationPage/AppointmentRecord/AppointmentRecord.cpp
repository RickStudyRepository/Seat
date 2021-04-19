#include "AppointmentRecord.h"
#include <QTableWidgetItem>
#include <QChar>
#include <QMessageBox>
#include "OperationAndStatus.h"
#include "../../Tools/ConstValue.h"
#include "../../Tools/FontFactory.h"
#include "../../Database/Database.h"
#include <QDebug>

AppointmentRecord::AppointmentRecord(QWidget *parent) : QWidget(parent) {
    initAppointmentRecord();
    initConfirmDialog();
    initContinueDialog();
    initWarning();
    initLayout();
    initDatabase();
    connectLogString();
    qDebug() << "AppointmentRecord page init successfully!";
}

void AppointmentRecord::initDatabase() {
    database = Database::getSingleDatabase();
}

void AppointmentRecord::initLayout() {
    layout  = new QVBoxLayout(this);
    layout->addWidget(appointmentRecord);
    layout->setMargin(0);
    setLayout(layout);
}

void AppointmentRecord::initAppointmentRecord() {
    appointmentRecord = new QTableWidget(this);
    // 设置表格行属性
    // 隐藏垂直表头
    appointmentRecord->verticalHeader()->setVisible(false);
    // 行交替换颜色
    appointmentRecord->setAlternatingRowColors(true);
    // 设置行高为50
    appointmentRecord->verticalHeader()->setDefaultSectionSize(50);

    // 设置表格列属性
    // 设置表格列数
    appointmentRecord->setColumnCount(ConstValue::appointmentRecordTableColumn);
    // 设置列宽度
    // 座位号列宽
    appointmentRecord->setColumnWidth(0, 100);
    // 时间列宽
    appointmentRecord->setColumnWidth(1, 450);
    // 操作或状态列宽
    appointmentRecord->setColumnWidth(2, 200);
    // 设置列宽为固定列宽
    // TODO:这里移植时可能需要修改
    appointmentRecord->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    // 设置表头字体
    appointmentRecord->horizontalHeader()->setFont(FontFactory::tableHeaderFont());

    // 调整表头格式
    QHeaderView* head = appointmentRecord->horizontalHeader();
    // 设置对齐方式为水平垂直居中
    head->setDefaultAlignment(Qt::AlignCenter);
    // 点击表头，表头不高亮
    head->setHighlightSections(false);
    // 不允许表头移动
    head->setSectionsMovable(false);
    // 设置修改后的表头格式
    appointmentRecord->setHorizontalHeader(head);
    // 表头文本
    headStringList << tr("座位号") << tr("时间") << tr("状态  |  操作");
    // 设置水平表头
    appointmentRecord->setHorizontalHeaderLabels(headStringList);

    // 设置表格的其他属性
    // 设置表格不可编辑
    appointmentRecord->setEditTriggers(QAbstractItemView::NoEditTriggers);
    // 设置单元格不可被选中
    appointmentRecord->setSelectionMode(QAbstractItemView::NoSelection);
}

void AppointmentRecord::initConfirmDialog() {
    confirmDialog = new ConfirmDialog(this);
    confirmDialog->setWindowTitle(tr("确认取消预约"));
    confirmDialog->setConfirmButtonText(tr("我要取消"));
    connect(confirmDialog, SIGNAL(confirmed()), this, SLOT(cancelAppointment()));
}

void AppointmentRecord::initContinueDialog() {
    continueTimeDialog = new ContinueTimeDialog(this);
    connect(
        continueTimeDialog,
        SIGNAL(sendTimeScope(AliasName::TimeScope)),
        this,
        SLOT(continueAppointment(AliasName::TimeScope))
    );
}

void AppointmentRecord::initWarning() {
    warning = new AutoCloseMessageBox(this);
    warning->setIcon(QMessageBox::Warning);
    warning->setDefaultButton(QMessageBox::Ok);
}

void AppointmentRecord::connectLogString() {
    connect(confirmDialog, SIGNAL(logSignal(QString)), this, SIGNAL(logSignal(QString)));
    connect(continueTimeDialog, SIGNAL(logSignal(QString)), this, SIGNAL(logSignal(QString)));
}

void AppointmentRecord::hideDialog() {
    // 按需关闭对话框
    if (confirmDialog->isVisible()) {
        emit logSignal(tr("预约记录：自动关闭确认对话框"));
        confirmDialog->close();
    }
    if (continueTimeDialog->isVisible()) {
        emit logSignal(tr("预约记录：自动关闭选择续约时间对话框"));
        continueTimeDialog->close();
    }
}

void AppointmentRecord::resetStudentNum(QString studentNum) {
    this->studentNum = studentNum;
    emit logSignal(tr("预约记录：重置登录的学生的学号为：") + studentNum);
}

void AppointmentRecord::clearAppointmentRecord() {
    // 释放上一次进入构造的操作和状态部件占用的动态内存
    size_t size = osList.size();
    for (size_t i = 0; i < size; i++) {
        delete osList[i];
    }
    // 清空上一次进入构造的操作和状态部件指针
    osList.clear();
    // 清空表格内容
    appointmentRecord->clearContents();
}

void AppointmentRecord::resetAppointments() {
    // 查询结果
    bool success = false;
    // 获取相应学号的学生的所有预约记录
    AliasName::Appointments appointments = database->getAllAppointmentsOf(studentNum.toStdString(), &success);

    // 更新预约列表
    this->appointments = appointments;
    // 清空表格中的内容
    clearAppointmentRecord();

    // 获取失败，结束执行
    if (success == false) {
        warning->showAndClose(5, tr("错误提示"), tr("获取预约记录失败！\n请重试！"));
        emit logSignal(tr("预约记录：获取预约记录失败"));
        return;
    }

    // 设置表格行数
    appointmentRecord->setRowCount(this->appointments.size());
    QTableWidgetItem* temp = NULL;
    OperationAndStatus* tempOS = NULL;
    size_t appointmentsNum = this->appointments.size();
    for (size_t i = 0; i < appointmentsNum; i++) {
        // 座位号
        temp = new QTableWidgetItem(QString("%1").arg(this->appointments[i].seatNum, 3, 10, QChar('0')));
        formatTableItem(temp);
        appointmentRecord->setItem(i, 0, temp);

        // 时间
        temp = new QTableWidgetItem(QString::fromStdString(this->appointments[i].time));
        formatTableItem(temp);
        appointmentRecord->setItem(i, 1, temp);

        // 操作或状态
        tempOS = new OperationAndStatus(i, this->appointments[i].status, this);
        connect(tempOS, SIGNAL(cancelSignal(int)), this, SLOT(callConfirmDialog(int)));
        connect(tempOS, SIGNAL(continueSignal(int)), this, SLOT(callContinueDialog(int)));
        // 绑定日志信号
        connect(tempOS, SIGNAL(logSignal(QString)), this, SIGNAL(logSignal(QString)));
        osList.push_back(tempOS);
        appointmentRecord->setCellWidget(i, 2, tempOS);
    }
    emit logSignal(tr("预约记录：重置预约记录表格内容"));
}

void AppointmentRecord::formatTableItem(QTableWidgetItem *item) {
    // 设置对齐方式为水平垂直居中
    item->setTextAlignment(Qt::AlignCenter);
    // 设置字体
    item->setFont(FontFactory::tableContentFont());
}

void AppointmentRecord::callConfirmDialog(int rowNum) {
    // 记录要取消预约的行号
    cancelRowNum = rowNum;
    std::string text =
            "你确认要取消如下预约吗？"
            "\n座位号：" +
            QString::number(this->appointments[rowNum].seatNum).toStdString() +
            "\n时间段：" + this->appointments[rowNum].time;
    confirmDialog->setTextAndShow(tr(text.c_str()));
    emit logSignal(
                tr("预约记录：呼出确认对话框，询问是否确认取消预约\n") +
                tr("确认信息：\n") +
                QString::fromStdString(text)
    );
}

void AppointmentRecord::cancelAppointment() {
    emit logSignal(
                tr("预约记录：取消预约ID为：") +
                QString::number(appointments[cancelRowNum].id) +
                tr("预约")
    );
    // 更新数据库及内存中保存的数据
    bool result = database->cancelAppointment(appointments[cancelRowNum].id);
    if (result == false) {
        emit logSignal(tr("预约记录：取消预约失败"));
        warning->showAndClose(5, tr("错误提示"), tr("取消预约失败！\n请重试！"));
        return;
    }
    emit logSignal(tr("预约记录：取消预约成功"));

    // 更新界面
    // 获取相应单元格的部件并强转为OperationAndStatus类型，进而重置标签内容
    QWidget* temp = appointmentRecord->cellWidget(cancelRowNum, 2);
    qobject_cast<OperationAndStatus*>(temp)->resetOperationAndStatus(ConstValue::UsedSeat);
    emit logSignal(tr("预约记录：重置表格中的预约状态"));
}

void AppointmentRecord::callContinueDialog(int rowNum) {
    emit logSignal(tr("预约记录：呼出选择续约时间对话框"));
    continueRowNum = rowNum;
    // 获取对应预约的预约时间范围
    AliasName::TimeScope currentTimeScope = Tools::databaseTimeToTimeScope(appointments[continueRowNum].time);
    // 获取可用时间结果
    bool success = false;
    // 从数据库中获取该座位的可用时间段
    AliasName::TimeScopes availableTimes = database->getAvailableTimeScopesOf(appointments[continueRowNum].seatNum, &success);
    // 获取可用时间失败
    if (success == false) {
        warning->showAndClose(5, tr("错误提示"), tr("获取可用时间失败！\n请重试"));
        emit logSignal(tr("预约记录：获取可用时间失败，停止呼出选择续约时间对话框"));
        return;
    }
    emit logSignal(tr("预约记录：获取可用时间成功，呼出选择预约时间对话框"));

    // 设置当前选定的时间
    continueTimeDialog->setCurrentTimeScope(currentTimeScope);
    // 设置可用时间范围并呼出对话框
    continueTimeDialog->setTimeScopeAndShow(availableTimes);
}

void AppointmentRecord::continueAppointment(AliasName::TimeScope continueTimeScope) {
    std::string oldTime = appointments[continueRowNum].time;
    std::string newTime = appointments[continueRowNum].time;
    // 更新内存中的结束时间
    newTime.replace(17, 5, Tools::intToTimeString(continueTimeScope.second).toStdString());
    appointments[continueRowNum].time = newTime;
    // 更新表格内容
    // TODO:这里为什么不能使用itemAt方法获取相应的单元格内容呢
    appointmentRecord->item(continueRowNum, 1)->setText(tr(newTime.c_str()));
    // 更新数据库的结束时间
    bool result = database->continueAppointment(appointments[continueRowNum].id, newTime);
    if (result == false) {
        emit logSignal(tr("预约记录：续约失败"));
        warning->showAndClose(5, tr("错误提示"), tr("续约失败！\n请重试！"));
    }
    emit logSignal(
                tr("预约记录：续约成功，续约ID号为") +
                QString::number(appointments[continueRowNum].id) +
                tr("的预约\n") +
                tr("旧的时间：") + QString::fromStdString(oldTime) + tr("\n") +
                tr("新的时间：") + QString::fromStdString(newTime)
    );
}
