#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QIcon>
#include <QSize>
#include <QString>
#include <QVBoxLayout>
#include <QCloseEvent>
#include "AdminPage/AdminPage.h"
#include "HomePage/HomePage.h"
#include "OperationPage/OperationPage.h"
#include "Tools/DigitKeyBoard/DigitKeyBoard.h"
#include "Tools/Dialog/InputDialog.h"
#include "Tools/Dialog/AutoCloseMessageBox.h"
#include "Tools/ConstValue.h"
#include "Database/Database.h"
#include "Socket/tcp_server.h"

// RFID相关
#include "RFID/RFID.h"

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = NULL);

// RFID相关
private:
    RFID* rfid;
    void initRFID();

// 数据库相关
private:
    Database* database;
    void initDatabase();

// Socket相关
private:
    tcp_server* server;
    void initTcpServer();

// 界面相关
private:
    // 初始化窗口的基本属性
    void initWindowBasicProperty();

    // 整体布局
    QVBoxLayout* layout;
    void initLayout();

    // 密码输入对话框
    InputDialog* inputDialog;
    void initInputDialog();

    // 数字键盘
    DigitKeyBoard* digitKeyBoard;
    void initDigitKeyBoard();

    // 密码错误提示框
    AutoCloseMessageBox* warning;
    void initWarning();

    // 刷卡提示框
    AutoCloseMessageBox* info;
    void initInfoMessageBox();

    bool isGoingAdminPage;
    // 超级管理员界面
    AdminPage* adminPage;
    void initAdminPage();

    // 首页
    HomePage* homePage;
    // 初始化首页
    void initHomePage();

    // 操作界面
    OperationPage* operationPage;
    // 初始化操作界面
    void initOperationPage();

    // 绑定日志信息槽
    void connectLogString();

    void closeEvent(QCloseEvent* closeEvent);

signals:
    // 日志信息信号
    void logSignal(QString);

private slots:
    // 切换到管理员界面
    void gotoAdminPage();
    // 输入密码
    void inputPassword();
    // 核验密码
    void verifyPassword(QString input);
    // 呼出键盘
    void callDigitKeyBoard(QLineEdit* output);
    // 停止进入管理员界面
    void stopGoingAdminPage();

    // 进入操作界面前，读取卡内学号
    void readStudentNum();
    // 切换到操作界面
    void gotoOperationPage(QString studentNum);
    // 从操作界面返回到首页
    void returnHomePageFromOperationPage();
    // 从管理员界面返回首页
    void returnHomePageFromAdminPage();

    // 关闭输入密码对话框和数字键盘
    void closeInputDialogAndDigitKeyBoard();

    // 处理结束程序的信号
    void endProgram();
};

#endif // MAINWINDOW_H
