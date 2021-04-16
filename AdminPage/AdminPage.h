#ifndef ADMINPAGE_H
#define ADMINPAGE_H

#include <QWidget>
#include <QLabel>
#include <QString>
#include <QPushButton>
#include <QTextEdit>
#include <QGridLayout>
#include <QFrame>
#include <QPoint>
#include "../Tools/DigitKeyBoard/DigitKeyBoard.h"
#include "../Tools/Dialog/InputDialog.h"
#include "../Tools/Dialog/ConfirmDialog.h"
#include "../Tools/Dialog/AutoCloseMessageBox.h"
#include "../Tools/LogSave.h"
#include "../RFID/RFID.h"

class AdminPage : public QWidget
{
    Q_OBJECT
// RFID相关
private:
    RFID* rfid;
    void initRFID();

// 界面相关
private:
    // 管理员密码
    std::string password;

    // 主窗口的坐标
    QPoint parentPos;

    // 部件
    // 系统名称
    QLabel* systemName = NULL;
    void initSystemName();
    // 作者名称
    QLabel* authorName = NULL;
    void initAuthorName();
    // 一个美化的水平线
    QFrame* horizontalLine = NULL   ;
    void initHorizontalLine();
    // 返回首页的按钮
    QPushButton* returnButton = NULL;
    void initReturnButton();
    // 初始化卡片信息的按钮
    QPushButton* initCardContent = NULL;
    void initCardContentButton();
    // 日志文本框，常量指针指向常量数据
    QTextEdit* const logTextEdit = NULL;
    void initLogTextEdit();

    // 输入对话框
    InputDialog* inputDialog;
    void initInputDialog();
    // 数字键盘
    DigitKeyBoard* digitKeyBoard;
    void initDigitKeyBoard();
    // 将要写入卡内的学号
    QString studentNum;
    // 确认写入对话框
    ConfirmDialog* confirmDialog;
    void initConfirmDialog();
    // 自动关闭的错误信息提示对话框
    AutoCloseMessageBox* warning;
    void initWarning();
    // 自动关闭的信息提示对话框
    AutoCloseMessageBox* info;
    void initInfoMessageBox();

    // 结束程序
    QPushButton* exitButton;
    void initExitButton();

    // 布局
    QGridLayout* layout = NULL;
    void initLayout();

    // 上次写入日志的时间
    QString previousLogTime;

// 日志相关
private:
    LogSave logSave;
    QPushButton* saveLogButton;
    // 初始化日志保存按钮
    void initSaveLogButton();

public:
    explicit AdminPage(QWidget *parent = NULL);

    ~AdminPage();

    // 校验密码
    bool verifyPassword(QString password);

signals:
    void returnHomePage();

    void endProgram();

public slots:
    // 前往前台
    void gotoFront(QPoint parentPos);
    // 隐藏到后台
    void gotoBack();

private slots:
    // 呼出填写对话框
    void inputStudentNum();
    // 呼出数字键盘
    void callDigitKeyBoard(QLineEdit* output);
    // 确认录入学号
    void confirmWriteStudentNum(QString studentNum);
    // 向卡片内写入学号
    void writeStudentNum();
    // 关闭输入对话框
    void closeInputDialog();
    // 隐藏数字键盘
    void hideDigitKeyBoard();

    // 追加日志
    void appendLog(QString logString);

    // 将初始化卡片信息的按钮置为可用
    void enableInitCardContentButton();

    // 保存日志槽函数
    void saveLog();
};

#endif // ADMINPAGE_H
