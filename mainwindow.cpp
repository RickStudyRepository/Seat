#include "MainWindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle(appName);
    setWindowIcon(appIcon);
    setFixedSize(fixedSize);
    initHomePage();
    initOperationPage();
    initLayout();
}

// 初始化主页布局
void MainWindow::initHomePage() {
    connect(homePage, SIGNAL(gotoOperationPageSignal()), this, SLOT(gotoOperationPage()));
    // 默认显示
    homePage->gotoFront();
}

// 初始化操作界面
void MainWindow::initOperationPage() {
    connect(operationPage, SIGNAL(returnHomePage()), this, SLOT(returnHomePage()));
    // 默认隐藏
    operationPage->gotoBack();
}

// 初始化布局
void MainWindow::initLayout() {
    layout->addWidget(homePage);
    layout->addWidget(operationPage);
    // 将边框空白设置为0
    layout->setMargin(0);
    setLayout(layout);
}

// 切换到操作界面
void MainWindow::gotoOperationPage() {
    homePage->gotoBack();
    // TODO:这里要通过RFID标签获取学号
    operationPage->gotoFront(tr("181110305"));
}

// 返回到首页
void MainWindow::returnHomePage() {
    operationPage->gotoBack();
    homePage->gotoFront();
}
