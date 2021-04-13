#include "AdminPage.h"
#include "../Tools/ConstValue.h"
#include "../Tools/FontFactory.h"
#include "../Tools/Tools.h"
#include <QPalette>
#include <QColor>
#include <QDebug>

AdminPage::AdminPage(QWidget *parent)
    : QWidget(parent), logTextEdit(new QTextEdit(this)) {
    password = ConstValue::password;
    initSystemName();
    initHorizontalLine();
    initAuthorName();
    initReturnButton();
    initCardContentButton();
    initLogTextEdit();
    initInputDialog();
    initConfirmDialog();
    initDigitKeyBoard();
    initWarning();
    initLayout();
}

void AdminPage::initLayout() {
    layout = new QGridLayout(this);
    layout->addWidget(systemName, 0, 0, 1, 7);
    layout->addWidget(returnButton, 0, 7, 1, 1);
    layout->addWidget(authorName, 1, 0, 1, 4);
    layout->addWidget(initCardContent, 1, 6, 1, 2);
    layout->addWidget(horizontalLine, 2, 0, 1, 8);
    layout->addWidget(logTextEdit, 3, 0, 9, 8);
    layout->setMargin(3);
    layout->setAlignment(Qt::AlignCenter);
    setLayout(layout);
}

void AdminPage::initSystemName() {
    systemName = new QLabel(tr("图书馆座位预约系统调试信息"), this);
    systemName->setFont(FontFactory::titleFont());
}

void AdminPage::initAuthorName() {
    authorName = new QLabel(tr("作者：181110305-董成相 和 181110308-吉张雄"), this);
    authorName->setFont(FontFactory::describeFont());
}

void AdminPage::initHorizontalLine() {
    horizontalLine = new QFrame(this);
    horizontalLine->setFrameStyle(QFrame::HLine);
    horizontalLine->setLineWidth(2);
    horizontalLine->setFixedWidth(494);
    QPalette temp = horizontalLine->palette();
    temp.setColor(QPalette::WindowText, QColor(63, 81, 181));
    horizontalLine->setPalette(temp);
}

void AdminPage::initReturnButton() {
    returnButton = new QPushButton(this);
    returnButton->setText(tr("返回主界面"));
    returnButton->setFont(FontFactory::describeFont());
    // 绑定返回的信号
    connect(returnButton, SIGNAL(released()), this, SIGNAL(returnHomePage()));
}

void AdminPage::initCardContentButton() {
    initCardContent = new QPushButton(this);
    initCardContent->setText(tr("初始化卡片信息"));
    initCardContent->setFont(FontFactory::describeFont());
    connect(initCardContent, SIGNAL(released()), this, SLOT(inputStudentNum()));
}

void AdminPage::initLogTextEdit() {
    logTextEdit->setFont(FontFactory::describeFont());
    logTextEdit->setReadOnly(true);
    logTextEdit->setMinimumHeight(375);
    logTextEdit->append(tr("这里是所有的日志信息："));
}

void AdminPage::initInputDialog() {
    inputDialog = new InputDialog(this);
    inputDialog->setWindowTitle(tr("输入学号"));
    inputDialog->setTip(tr("学号："));
    inputDialog->setLineEditPlaceholderText(tr("请输入学号"));
    inputDialog->setLineEditEchoMode(QLineEdit::Normal);
    connect(inputDialog, SIGNAL(confirmed(QString)), this, SLOT(confirmWriteStudentNum(QString)));
    connect(inputDialog, SIGNAL(showKeyBoardSignal(QLineEdit*)), this, SLOT(callDigitKeyBoard(QLineEdit*)));
    connect(inputDialog, SIGNAL(finished(int)), this, SLOT(hideDigitKeyBoard()));
}

void AdminPage::initDigitKeyBoard() {
    digitKeyBoard = DigitKeyBoard::getKeyBoard();
    digitKeyBoard->hide();
}

void AdminPage::initConfirmDialog() {
    confirmDialog = new ConfirmDialog(this);
    confirmDialog->setConfirmButtonText(tr("确认写入"));
    confirmDialog->setWindowTitle(tr("写入学号确认"));
    connect(confirmDialog, SIGNAL(confirmed()), this, SLOT(writeStudentNum()));
}

void AdminPage::initWarning() {
    warning = new AutoCloseMessageBox(this);
    warning->setIcon(QMessageBox::Warning);
    warning->setDefaultButton(QMessageBox::Ok);
}

void AdminPage::appendLog(QString logString) {
    QString nowTime = Tools::getCurrentDatetime();
    if (nowTime == previousLogTime) {
        logTextEdit->append(logString);
    }
    else {
        logTextEdit->append(nowTime + tr("\n") + logString);
        // 更新上次写入日志的时间
        previousLogTime = nowTime;
    }
}

bool AdminPage::verifyPassword(QString password) {
    return this->password == password.toStdString();
}

void AdminPage::inputStudentNum() {
    // 清空文本框内的内容
    inputDialog->clearLineEdit();
    inputDialog->show();
    appendLog(tr("管理员界面：呼出学号输入对话框"));
    callDigitKeyBoard(inputDialog->getLineEdit());
}

void AdminPage::callDigitKeyBoard(QLineEdit* output) {
    digitKeyBoard->setGeometry(
                parentPos.x() + 190,
                parentPos.y() + 350,
                digitKeyBoard->width(),
                digitKeyBoard->height()
    );
    if (digitKeyBoard->isHidden() == false) {
        // 如果键盘没有被隐藏，那么就将数字键盘窗口设置为活动窗口
        appendLog(tr("管理员界面：将数字键盘设置为活动窗口"));
        digitKeyBoard->activateWindow();
    }
    else {
        appendLog(tr("管理员界面：呼出数字键盘，开始输入学号"));
        digitKeyBoard->startInput(output);
    }
}

void AdminPage::confirmWriteStudentNum(QString studentNum) {
    // 检验输入是否为空
    if (studentNum == tr("")) {
        warning->showAndClose(3, tr("非法的学号"), tr("学号不能为空"));
        appendLog(tr("管理员界面：输入的学号为空，停止写入"));
        return;
    }
    // 关闭输入对话框及数字键盘
    closeInputDialog();
    hideDigitKeyBoard();
    // 缓存学号
    this->studentNum = studentNum;
    // 呼出确认对话框
    confirmDialog->setTextAndShow(
                tr("将向卡内写入如下学号：\n") +
                studentNum + "\n" +
                tr("请确认！")
    );
}

void AdminPage::writeStudentNum() {
    appendLog(tr("向卡片内写入学号：") + studentNum);
}

void AdminPage::gotoFront(QPoint parentPos) {
    this->parentPos = parentPos;
    show();
    appendLog(tr("管理员界面：切入前台"));
}

void AdminPage::gotoBack() {
    closeInputDialog();
    hideDigitKeyBoard();
    hide();
    appendLog(tr("管理员界面：切入后台"));
}

void AdminPage::closeInputDialog() {
    if (inputDialog->isVisible()) {
        inputDialog->close();
        appendLog(tr("管理员界面：关闭学号输入对话框"));
    }
}

void AdminPage::hideDigitKeyBoard() {
    if (digitKeyBoard->isVisible()) {
        digitKeyBoard->hide();
        appendLog(tr("管理员界面：隐藏数字键盘"));
    }
}
