#include "OperationPage.h"
#include "../Tools/ConstValue.h"

OperationPage::OperationPage(QWidget *parent) : QWidget(parent)
{
    initOperationTab();
    initLayout();
    connect(headWidget, SIGNAL(returnHomePageSignal()), this, SLOT(emitReturnHomePage()));
}

void OperationPage::initLayout() {
    layout->addWidget(headWidget);
    layout->addWidget(operationTab);
    layout->setMargin(0);
    setLayout(layout);
}

void OperationPage::initOperationTab() {
    // 设置Tab的样式为圆角
    operationTab->setTabShape(QTabWidget::Rounded);
    // 将Tab设置为不能调整顺序
    operationTab->setMovable(false);
    // 添加页面到Tab中
    operationTab->addTab(makeAppoinment, makeAppoinmentString);
    operationTab->addTab(appointmentRecord, appointmentRecordString);
    // 默认展示“开始预约”操作界面
    operationTab->setCurrentIndex(0);
}

void OperationPage::gotoFront(QString studentNum) {
    resetStudentNum(studentNum);
    operationTab->setCurrentIndex(0);
    headWidget->startCountDown();

    // TODO:call database here
    // 获取相应学号的学生的所有预约记录
    AliasName::Appointments appointments;
    appointments.push_back(AliasName::Appointment("181110305", 23, "2020-04-09 08:00-09:00", ConstValue::UsedSeat));
    appointments.push_back(AliasName::Appointment("181110305", 34, "2020-04-09 18:00-21:00", ConstValue::UsingSeat));
    appointmentRecord->resetAppointments(appointments);

    show();
}

void OperationPage::gotoBack() {
    makeAppoinment->hideDialog();
    appointmentRecord->hideDialog();
    headWidget->stopCountDown();
    hide();
}

void OperationPage::resetStudentNum(QString studentNum) {
    headWidget->resetLogInLabelText(studentNum);
    makeAppoinment->resetStudentNum(studentNum);
    appointmentRecord->resetStudentNum(studentNum);
}

void OperationPage::emitReturnHomePage() {
    emit returnHomePage();
}
