#include "AppointmentRecord.h"
#include <QTableWidgetItem>
#include <QChar>
#include "OperationAndStatus.h"
#include "../../Tools/ConstValue.h"
#include "../../Tools/FontFactory.h"
#include <QDebug>

AppointmentRecord::AppointmentRecord(QWidget *parent) : QWidget(parent)
{
    initAppointmentRecord();
    initConfirmDialog();
    initLayout();
}

void AppointmentRecord::initLayout() {
    layout->addWidget(appointmentRecord);
    layout->setMargin(0);
    setLayout(layout);
}

void AppointmentRecord::initAppointmentRecord() {
    // 设置表格行属性
    // 隐藏垂直表头
    appointmentRecord->verticalHeader()->setVisible(false);
    // 行交替换颜色
    appointmentRecord->setAlternatingRowColors(true);
    // 设置行高为50
    appointmentRecord->verticalHeader()->setDefaultSectionSize(50);

    // 设置表格列属性
    // 设置表格列数
    appointmentRecord->setColumnCount(columnCount);
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
    confirmDialog->setWindowTitle(tr("确认取消预约"));
    confirmDialog->setConfirmButtonText(tr("我要取消"));
    connect(confirmDialog, SIGNAL(confirmed()), this, SLOT(cancelAppointment()));
}

void AppointmentRecord::hideDialog() {
    // 按需关闭对话框
    if (confirmDialog->isVisible()) {
        confirmDialog->close();
    }
    if (continueTimeDialog->isVisible()) {
        continueTimeDialog->close();
    }
}

void AppointmentRecord::resetStudentNum(QString studentNum) {
    this->studentNum = studentNum;
}

void AppointmentRecord::resetAppointments(AliasName::Appointments appointments) {
    // 更新预约列表
    this->appointments = appointments;
    // 清空表格内容
    appointmentRecord->clearContents();
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
        appointmentRecord->setCellWidget(i, 2, tempOS);
    }
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
}

void AppointmentRecord::callContinueDialog(int rowNum) {
    continueNum = rowNum;
    // 获取对应预约的预约时间范围
    AliasName::TimeScope currentTimeScope = Tools::databaseTimeToTimeScope(appointments[rowNum].time);
    // TODO:call database here
    // 从数据库中获取该座位的可用时间段
    int seatNum = appointments[rowNum].seatNum;
    AliasName::TimeScopes availableTimes;
    availableTimes.push_back(AliasName::TimeScope(21, 22));
    // 设置当前选定的时间
    continueTimeDialog->setCurrentTimeScope(currentTimeScope);
    // 设置可用时间范围并呼出对话框
    continueTimeDialog->setTimeScopeAndShow(availableTimes);
}

void AppointmentRecord::cancelAppointment() {
    qDebug() << "Cancel appointment";
    // 更新数据库及内存中保存的数据
    // call database here

    // 更新界面
    // 获取相应单元格的部件并强转为OperationAndStatus类型，进而重置标签内容
    QWidget* temp = appointmentRecord->cellWidget(cancelRowNum, 2);
    qobject_cast<OperationAndStatus*>(temp)->resetOperationAndStatus(ConstValue::UsedSeat);
}
