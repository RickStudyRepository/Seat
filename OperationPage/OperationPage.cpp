#include "OperationPage.h"

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
    show();
}

void OperationPage::gotoBack() {
    makeAppoinment->hideDialog();
    headWidget->stopCountDown();
    hide();
}

void OperationPage::resetStudentNum(QString studentNum) {
    headWidget->resetLogInLabelText(studentNum);
    makeAppoinment->resetStudentNum(studentNum);
}

void OperationPage::emitReturnHomePage() {
    emit returnHomePage();
}
