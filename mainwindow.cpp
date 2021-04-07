#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    setFixedSize(800, 480);
    initHomePage();
    initOperationPage();
    // 将边框宽度设置为0
    layout->setMargin(0);
    setLayout(layout);
}

// 初始化主页布局
void MainWindow::initHomePage() {
    homePage->setPixmap(QPixmap(QString(":/images/HomePage.jpg")));
    // 默认显示
    homePage->show();

    // 添加到布局中
    layout->addWidget(homePage);
}

// 初始化操作界面顶部布局
void MainWindow::initOperationPageHead() {
    studentNumLabel->setText(studentNumString + studentNum);
    connect(returnButton, SIGNAL(released()), this, SLOT(returnHomePage()));
    headLayout->addWidget(studentNumLabel);
    headLayout->addWidget(returnButton);
    headLayout->setMargin(0);
}

// 初始化开始预约界面
void MainWindow::initMakeAppointment() {

}

// 初始化预约记录界面
void MainWindow::initAppointmentRecord() {

}

void MainWindow::initOperationTab() {
    // 设置Tab的基本属性
    // 设置Tab的样式为圆角
    operationTab->setTabShape(QTabWidget::Rounded);
    // 将Tab设置为不能调整顺序
    operationTab->setMovable(false);
    // 添加操作到Tab中
    operationTab->addTab(makeAppointment, makeAppointmentString);
    operationTab->addTab(appointmentRecord, appointmentRecordString);
    // 默认展示“开始预约”操作界面
    operationTab->setCurrentIndex(0);
}

// 初始化操作布局
void MainWindow::initOperationPage() {
    // 初始化操作界面顶部布局
    initOperationPageHead();
    // 初始化开始预约界面
    initMakeAppointment();
    // 初始化我的预约界面
    initAppointmentRecord();
    // 初始化标签页界面
    initOperationTab();

    // 设置操作界面布局
    operationPageLayout->addLayout(headLayout);
    operationPageLayout->addWidget(operationTab);
    operationPageLayout->setMargin(0);
    operationPage->setLayout(operationPageLayout);

    // 默认隐藏操作界面
    operationPage->hide();
    // 添加到布局中
    layout->addWidget(operationPage);
}

// 切换到操作界面
void MainWindow::gotoOperationPage() {
    state = true;
    homePage->hide();
    operationPage->show();
    // 默认进入到“开始预约”Tab
    operationTab->setCurrentIndex(0);
}

// 返回到主界面
void MainWindow::returnHomePage() {
    state = false;
    operationPage->hide();
    homePage->show();
}

// 暂时用来演示切换效果
void MainWindow::mouseReleaseEvent(QMouseEvent *mouseEvent) {
    // 鼠标左键释放切换到操作界面
    if (mouseEvent->button() == Qt::LeftButton) {
        if (state == false) {
            gotoOperationPage();
        }
    }
}
