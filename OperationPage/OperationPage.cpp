#include "OperationPage.h"
#include "../Tools/ConstValue.h"
#include "../Tools/FontFactory.h"
#include <QPalette>
#include <QColor>

OperationPage::OperationPage(QWidget *parent) : QWidget(parent) {
    initHeadWidget();
    initHorizontalLine();
    initOperationTab();
    initLayout();
    connectLogString();
    qDebug() << "Operation page init successfully!";
}

void OperationPage::initHeadWidget() {
    headWidget = new HeadWidget(this);
    connect(headWidget, SIGNAL(returnHomePageSignal()), this, SLOT(emitReturnHomePage()));
}

void OperationPage::initHorizontalLine() {
    horizontalLine = new QFrame(this);
    horizontalLine->setFrameStyle(QFrame::HLine);
    horizontalLine->setLineWidth(2);
    horizontalLine->setFixedWidth(494);
    QPalette temp = horizontalLine->palette();
    temp.setColor(QPalette::WindowText, QColor(63, 81, 181));
    horizontalLine->setPalette(temp);
}

void OperationPage::initOperationTab() {
    operationTab = new QTabWidget(this);
    // 设置Tab的样式为圆角
    operationTab->setTabShape(QTabWidget::Rounded);
    // 将Tab设置为不能调整顺序
    operationTab->setMovable(false);
    // 添加现场预约到Tab中
    makeAppoinment = new MakeAppointment(this);
    operationTab->addTab(makeAppoinment, ConstValue::makeAppoinmentString);
    // 添加我的预约到Tab中
    appointmentRecord = new AppointmentRecord(this);
    operationTab->addTab(appointmentRecord, ConstValue::appointmentRecordString);
    // 默认展示“开始预约”操作界面
    operationTab->setCurrentIndex(0);
    operationTab->setFont(FontFactory::describeFont());
    // 仅当进入我的预约界面时，加载我的预约记录
    connect(operationTab, SIGNAL(currentChanged(int)), this, SLOT(loadAppointments(int)));
}

void OperationPage::initLayout() {
    layout = new QVBoxLayout(this);
    layout->addWidget(headWidget);
    layout->addWidget(horizontalLine);
    layout->addWidget(operationTab);
    layout->setMargin(0);
    layout->setAlignment(Qt::AlignCenter);
    setLayout(layout);
}

void OperationPage::connectLogString() {
    connect(headWidget, SIGNAL(logSignal(QString)), this, SIGNAL(logSignal(QString)));
    connect(makeAppoinment, SIGNAL(logSignal(QString)), this, SIGNAL(logSignal(QString)));
    connect(appointmentRecord, SIGNAL(logSignal(QString)), this, SIGNAL(logSignal(QString)));
}

void OperationPage::gotoFront(QString studentNum) {
    resetStudentNum(studentNum);
    operationTab->setCurrentIndex(0);
    headWidget->startCountDown();
    emit logSignal(tr("操作界面：切入前台"));
    show();
}

void OperationPage::gotoBack() {
    makeAppoinment->hideDialog();
    appointmentRecord->hideDialog();
    headWidget->stopCountDown();
    emit logSignal(tr("操作界面：切入后台"));
    hide();
}

void OperationPage::resetStudentNum(QString studentNum) {
    headWidget->resetLogInLabelText(studentNum);
    makeAppoinment->resetStudentNum(studentNum);
    appointmentRecord->resetStudentNum(studentNum);
    emit logSignal(tr("操作界面：重置登录学生的学号为：") + studentNum);
}

void OperationPage::loadAppointments(int index) {
    // 如果切到了我的预约页面
    if (index == 1) {
        emit logSignal(tr("操作界面：载入当前用户的预约记录"));
        appointmentRecord->resetAppointments();
    }
}

void OperationPage::emitReturnHomePage() {
    emit logSignal(tr("操作界面：返回首页"));
    emit returnHomePage();
}
