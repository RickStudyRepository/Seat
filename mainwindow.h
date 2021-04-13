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

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = NULL);

private:
    // 窗口标题
    const QString appName = tr("Seat");
    // 窗口图标
    const QIcon appIcon = QIcon(QString(":/images/Seat.ico"));
    // 窗口大小
    const QSize fixedSize = QSize(800, 480);
    // 整体布局
    QVBoxLayout* layout = new QVBoxLayout(this);
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
    // 切换到操作界面
    void gotoOperationPage();
    // 返回到首页界面
    void returnHomePageFromOperationPage();
    void returnHomePageFromAdminPage();

    // 关闭输入密码对话框和数字键盘
    void closeInputDialogAndDigitKeyBoard();
};

#endif // MAINWINDOW_H
