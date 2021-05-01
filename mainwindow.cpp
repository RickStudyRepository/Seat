#include "MainWindow.h"
#include <QDebug>
#include <QPoint>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    // 默认设置是否正在前往管理员界面为false
    isGoingAdminPage = false;
    initRFID();
    initTcpServer();
    initDatabase();
    initWindowBasicProperty();
    initAdminPage();
    initHomePage();
    initOperationPage();
    initInputDialog();
    initDigitKeyBoard();
    initWarning();
    initInfoMessageBox();
    initLayout();
    connectLogString();
}

void MainWindow::initRFID() {
    this->rfid = RFID::getRFID();
}

void MainWindow::initDatabase() {
    database = Database::getSingleDatabase();
}

void MainWindow::initTcpServer() {
    server = new tcp_server(this);
    connect(server, SIGNAL(logSignal(QString)), this, SIGNAL(logSignal(QString)));
}

void MainWindow::initWindowBasicProperty() {
    setWindowTitle(ConstValue::appName);
    setWindowIcon(QIcon(ConstValue::appIconLoction));
    setFixedSize(ConstValue::windowSize);
}

// 初始化管理员界面
void MainWindow::initAdminPage() {
    adminPage = new AdminPage(this);
    connect(adminPage, SIGNAL(returnHomePage()), this, SLOT(returnHomePageFromAdminPage()));
    connect(adminPage, SIGNAL(endProgram()), this, SLOT(endProgram()));
    // 默认隐藏
    adminPage->gotoBack();
    emit logSignal(tr("主窗口：初始化管理员界面"));
}

// 初始化主页布局
void MainWindow::initHomePage() {
    homePage = new HomePage(this);
    connect(homePage, SIGNAL(gotoOperationPageSignal()), this, SLOT(readStudentNum()));
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
    layout = new QVBoxLayout(this);
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
    // 停止前往管理员界面，也就是允许前往操作界面
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
    warning->setIcon(QMessageBox::Warning);
    warning->setDefaultButton(QMessageBox::Ok);
    emit logSignal(tr("主窗口：初始化错误信息提示对话框"));
}

void MainWindow::initInfoMessageBox() {
    info = new AutoCloseMessageBox(this);
    info->setIcon(QMessageBox::Information);
    info->setDefaultButton(QMessageBox::Ok);
    emit logSignal(tr("主窗口：初始化信息提示对话框"));
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
    emit logSignal(tr("主窗口：解开前往其他窗口的限制"));
}

// 读取学号
void MainWindow::readStudentNum() {
    // 如果正在进入管理员界面，则不响应
    if (isGoingAdminPage == true) {
        return;
    }

    // 让首页不再响应鼠标键盘事件，防止用户重复操作
    homePage->setEnabled(false);

    // 提示用户刷卡
    info->showAndClose(5, tr("请刷卡"), tr("请将你的ID卡放置在感应区"));

    // 通过RFID读取学号
    // 临时存储读取的学号
    QString studentNum;
    // 读取结果
    bool success = false;
    // 最大尝试次数，基于内部实现，每一次的等待时间大概是1s
    unsigned short maxTryTime = ConstValue::maxTryTimeOnCard;
    do {
        success = rfid->readStudentNum(studentNum);
        maxTryTime -= 1;
    } while(success == false && maxTryTime > 0);

    // 如果提示用户刷卡的对话框仍然可见，则关闭
    if (info->isVisible()) {
        info->close();
    }

    // 读取失败，发出错误提示
    if (success == false) {
        warning->showAndClose(5, tr("读卡失败"), tr("请重试进入并刷卡"));
        emit logSignal(tr("主窗口：进入操作界面时，获取ID卡内学号失败"));
    }
    // 读取成功，切到操作界面
    else {
        emit logSignal(tr("主窗口：进入操作界面时，获取ID卡内学号成功"));
        // 查询学号是否存在
        bool result = database->isStudentExists(studentNum.toStdString(), &success);
        // 检验学号成功
        if (success == true) {
            // 学号不存在，不能前往操作界面
            if (result == false) {
                warning->showAndClose(5, tr("非法用户"), tr("未识别的用户，请找管理员注册"));
                emit logSignal(tr("主窗口：数据库中没有学号为：") + studentNum + tr("的记录"));
            }
            // 学号存在，验证通过，前往操作界面
            else {
                emit logSignal(tr("主窗口：用户合法，前往操作界面"));
                gotoOperationPage(studentNum);
            }
        }
        // 检验学号失败
        else {
            warning->showAndClose(5, tr("错误提示"), tr("检验用户失败，请重试！"));
            emit logSignal(tr("主窗口：检验用户：") + studentNum + tr("失败，停止前往操作界面"));
        }
    }

    // 恢复首页对鼠标和键盘事件的响应
    homePage->setEnabled(true);
}

// 切换到操作界面
void MainWindow::gotoOperationPage(QString studentNum) {
    homePage->gotoBack();
    operationPage->gotoFront(studentNum);
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

void MainWindow::endProgram() {
    returnHomePageFromAdminPage();
    close();
}
