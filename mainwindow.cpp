#include "MainWindow.h"
#include <QDebug>
#include <QPoint>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    // 默认设置是否正在前往管理员界面为false
    isGoingAdminPage = false;
    setWindowTitle(appName);
    setWindowIcon(appIcon);
    setFixedSize(fixedSize);
    initAdminPage();
    initHomePage();
    initOperationPage();
    initInputDialog();
    initDigitKeyBoard();
    initWarning();
    initLayout();
    connectLogString();
}

// 初始化管理员界面
void MainWindow::initAdminPage() {
    adminPage = new AdminPage(this);
    connect(adminPage, SIGNAL(returnHomePage()), this, SLOT(returnHomePageFromAdminPage()));
    // 默认隐藏
    adminPage->gotoBack();
    emit logSignal(tr("主窗口：初始化管理员界面"));
}

// 初始化主页布局
void MainWindow::initHomePage() {
    homePage = new HomePage(this);
    connect(homePage, SIGNAL(gotoOperationPageSignal()), this, SLOT(gotoOperationPage()));
    connect(homePage, SIGNAL(gotoAdminPageSignal()), this, SLOT(inputPassword()));
    // 默认显示
    homePage->gotoFront();
    emit logSignal(tr("主窗口：初始化首页"));
}

// 初始化操作界面
void MainWindow::initOperationPage() {
    operationPage = new OperationPage(this);
    connect(operationPage, SIGNAL(returnHomePage()), this, SLOT(returnHomePageFromOperationPage()));
    // 默认隐藏
    operationPage->gotoBack();
    emit logSignal(tr("主窗口：初始化操作界面"));
}

void MainWindow::connectLogString() {
    connect(homePage, SIGNAL(logSignal(QString)), adminPage, SLOT(appendLog(QString)));
    connect(operationPage, SIGNAL(logSignal(QString)), adminPage, SLOT(appendLog(QString)));
    connect(inputDialog, SIGNAL(logSignal(QString)), adminPage, SLOT(appendLog(QString)));
    connect(digitKeyBoard, SIGNAL(logSignal(QString)), adminPage, SLOT(appendLog(QString)));
    connect(this, SIGNAL(logSignal(QString)), adminPage, SLOT(appendLog(QString)));
}

void MainWindow::closeEvent(QCloseEvent *closeEvent) {
    closeInputDialogAndDigitKeyBoard();
}

// 初始化布局
void MainWindow::initLayout() {
    layout->addWidget(adminPage);
    layout->addWidget(homePage);
    layout->addWidget(operationPage);
    // 将边框空白设置为0
    layout->setMargin(0);
    setLayout(layout);
    emit logSignal(tr("主窗口：初始化主界面布局"));
}

void MainWindow::initInputDialog() {
    inputDialog = new InputDialog(this);
    inputDialog->setWindowTitle(tr("验证密码"));
    inputDialog->setLineEditPlaceholderText(tr("请输入密码"));
    // 设置回显模式为密码形式
    inputDialog->setLineEditEchoMode(QLineEdit::Password);
    inputDialog->setTip(tr("密码："));

    connect(inputDialog, SIGNAL(confirmed(QString)), this, SLOT(verifyPassword(QString)));
    // 停止前往管理员界面，也就是允许前往
    connect(inputDialog, SIGNAL(cancel()), this, SLOT(stopGoingAdminPage()));
    connect(inputDialog, SIGNAL(finished(int)), this, SLOT(stopGoingAdminPage()));
    // 当关闭对话框时，同时关闭数字键盘
    connect(inputDialog, SIGNAL(finished(int)), this, SLOT(closeInputDialogAndDigitKeyBoard()));
    // 呼出数字键盘
    connect(inputDialog, SIGNAL(showKeyBoardSignal(QLineEdit*)), this, SLOT(callDigitKeyBoard(QLineEdit*)));
    emit logSignal(tr("主窗口：初始化密码输入对话框"));
}

void MainWindow::initDigitKeyBoard() {
    digitKeyBoard = DigitKeyBoard::getKeyBoard();
    digitKeyBoard->hide();
    emit logSignal(tr("主窗口：初始化数字键盘"));
}

void MainWindow::initWarning() {
    warning = new AutoCloseMessageBox(this);
    emit logSignal(tr("主窗口：初始化错误信息提示对话框"));
}

void MainWindow::gotoAdminPage() {
    homePage->gotoBack();
    adminPage->gotoFront(pos());
    emit logSignal(tr("主窗口：进入管理员界面"));
}

void MainWindow::inputPassword() {
    isGoingAdminPage = true;
    inputDialog->show();
    // 清空输入对话框内容
    inputDialog->clearLineEdit();
    emit logSignal(tr("主窗口：呼出输入密码对话框"));
    // 呼出键盘
    callDigitKeyBoard(inputDialog->getLineEdit());
}

void MainWindow::callDigitKeyBoard(QLineEdit* output) {
    digitKeyBoard->setGeometry(
                pos().x() + 190,
                pos().y() + 350,
                digitKeyBoard->width(),
                digitKeyBoard->height()
    );
    if (digitKeyBoard->isHidden() == false) {
        // 如果键盘没有被隐藏，那么就将数字键盘窗口设置为活动窗口
        digitKeyBoard->activateWindow();
        emit logSignal(tr("主窗口：将数字键盘设置为活动窗口"));
    }
    else {
        digitKeyBoard->startInput(output);
        emit logSignal(tr("主窗口：呼出数字键盘，开始输入"));
    }
}

void MainWindow::verifyPassword(QString input) {
    if (adminPage->verifyPassword(input) == true) {
        // 密码正确，关闭对话框并隐藏数字键盘
        inputDialog->close();
        digitKeyBoard->hide();
        emit logSignal(tr("主窗口：校验管理员密码成功"));
        gotoAdminPage();
    }
    else {
        emit logSignal(tr("主窗口：校验管理员密码失败"));
        warning->showAndClose(5, tr("密码错误"), tr("请检查密码"));
    }
}

// 停止前往管理员界面
void MainWindow::stopGoingAdminPage() {
    isGoingAdminPage = false;
    emit logSignal(tr("主窗口：停止前往管理员界面"));
}

// 切换到操作界面
void MainWindow::gotoOperationPage() {
    // 如果正在前往管理员界面，则直接返回
    if (isGoingAdminPage == true) {
        return;
    }
    homePage->gotoBack();
    // TODO:这里要通过RFID标签获取学号
    operationPage->gotoFront(tr("181110305"));
    emit logSignal(tr("主窗口：进入操作界面"));
}

// 从管理员界面返回首页
void MainWindow::returnHomePageFromAdminPage() {
    adminPage->gotoBack();
    homePage->gotoFront();
    isGoingAdminPage = false;
    emit logSignal(tr("主窗口：从管理员界面返回主窗口"));
}

// 返回到首页
void MainWindow::returnHomePageFromOperationPage() {
    operationPage->gotoBack();
    homePage->gotoFront();
    emit logSignal(tr("主窗口：从操作界面返回主窗口"));
}

void MainWindow::closeInputDialogAndDigitKeyBoard() {
    inputDialog->close();
    digitKeyBoard->hide();
    emit logSignal(tr("主窗口：自动关闭管理员密码输入对话框并隐藏数字键盘"));
}
