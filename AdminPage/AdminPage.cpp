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

    initDatabase();
    initRFID();
    initSystemName();
    initHorizontalLine();
    initAuthorName();
    initReturnButton();
    initCardContentButton();
    initLogTextEdit();
    initSaveLogButton();
    initInputDialog();
    initConfirmDialog();
    initDigitKeyBoard();
    initWarning();
    initInfoMessageBox();
    initExitButton();
    initLayout();
}

AdminPage::~AdminPage() {
    saveLog();
}

void AdminPage::initRFID() {
    rfid = RFID::getRFID();
    connect(rfid, SIGNAL(logSignal(QString)), this, SLOT(appendLog(QString)));
}

void AdminPage::initDatabase() {
    database = Database::getSingleDatabase();
}

void AdminPage::initLayout() {
    layout = new QGridLayout(this);
    layout->addWidget(systemName, 0, 0, 1, 7);
    layout->addWidget(returnButton, 0, 7, 1, 1);
    layout->addWidget(authorName, 1, 0, 1, 4);
    layout->addWidget(exitButton, 1, 5, 1, 1);
    layout->addWidget(initCardContent, 1, 6, 1, 1);
    layout->addWidget(saveLogButton, 1, 7, 1, 1);
    layout->addWidget(horizontalLine, 2, 0, 1, 8);
    layout->addWidget(logTextEdit, 3, 0, 5, 8);
    layout->setMargin(3);
    layout->setAlignment(Qt::AlignCenter);
    for (int i = 0; i < 8; i++) {
        layout->setColumnMinimumWidth(i, 96);
    }
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
    returnButton->setText(tr("返回首页"));
    returnButton->setFont(FontFactory::describeFont());
    // 绑定返回的信号
    connect(returnButton, SIGNAL(released()), this, SIGNAL(returnHomePage()));
}

void AdminPage::initCardContentButton() {
    initCardContent = new QPushButton(this);
    initCardContent->setText(tr("录入学号"));
    initCardContent->setFont(FontFactory::describeFont());
    connect(initCardContent, SIGNAL(released()), this, SLOT(inputStudentNum()));
}

void AdminPage::initLogTextEdit() {
    // logTextEdit的初始化使用了类的初始化列表，因为是一个常量
    logTextEdit->setFont(FontFactory::describeFont());
    logTextEdit->setReadOnly(true);
    logTextEdit->setMinimumHeight(375);
    logTextEdit->append(tr("这里是所有的日志信息："));
}

void AdminPage::initSaveLogButton() {
    saveLogButton = new QPushButton(this);
    saveLogButton->setText(tr("保存日志"));
    saveLogButton->setFont(FontFactory::describeFont());
    connect(saveLogButton, SIGNAL(released()), this, SLOT(saveLog()));
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
    connect(inputDialog, SIGNAL(finished(int)), this, SLOT(enableInitCardContentButton()));
    connect(inputDialog, SIGNAL(cancel()), this, SLOT(enableInitCardContentButton()));
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
    connect(confirmDialog,  SIGNAL(cancel()), this, SLOT(enableInitCardContentButton()));
    connect(confirmDialog, SIGNAL(finished(int)), this, SLOT(enableInitCardContentButton()));
}

void AdminPage::initWarning() {
    warning = new AutoCloseMessageBox(this);
    warning->setIcon(QMessageBox::Warning);
    warning->setDefaultButton(QMessageBox::Ok);
}

void AdminPage::initInfoMessageBox() {
    info = new AutoCloseMessageBox(this);
    info->setIcon(QMessageBox::Information);
    info->setDefaultButton(QMessageBox::Ok);
}

void AdminPage::initExitButton() {
    exitButton = new QPushButton(this);
    exitButton->setText(tr("退出程序"));
    exitButton->setFont(FontFactory::describeFont());
    connect(exitButton, SIGNAL(released()), this, SIGNAL(endProgram()));
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

void AdminPage::saveLog() {
    appendLog(tr("管理员界面：保存日志到文件"));
    logSave.writeLog(logTextEdit->toPlainText());
}

void AdminPage::enableInitCardContentButton() {
    initCardContent->setEnabled(true);
}

bool AdminPage::verifyPassword(QString password) {
    return this->password == password.toStdString();
}

void AdminPage::inputStudentNum() {
    // 将初始化卡片信息按钮置为不可用，阻止用户重复操作
    initCardContent->setEnabled(false);

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
    if (studentNum.length() == ConstValue::studentNumLength) {
        warning->showAndClose(5, tr("非法学号"), tr("学号的长度只能是9位数字"));
        appendLog(tr("管理员界面：输入的学号非法，停止写入"));
        return;
    }
    // 检查学号是否已保存在数据库中
    bool success = false;
    bool result = database->isStudentExists(studentNum, &success);
    // 查询失败
    if (*success == false) {
        warning->showAndClose(5, tr("查询失败"), tr("检验学号失败，请重试！"));
        appendLog(tr("管理员界面：检验学号是否已存在失败，停止写入"));
        return;
    }
    // 学号已存在
    if (result != false) {
        warning->showAndClose(5, tr("非法学号"), tr("学号已存在！"));
        appendLog(tr("管理员界面：学号已存在，停止写入"));
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
    // 提示用户刷卡
    info->showAndClose(5, tr("请刷卡"), tr("请把ID卡放到感应区"));

    // 最大尝试次数
    unsigned short maxTryTime = ConstValue::maxTryTimeOnCard;
    // 写卡是否成功
    bool success = false;
    do {
        success = rfid->writeStudentNum(studentNum);
        maxTryTime -= 1;
    } while (success == false && maxTryTime > 0);

    // 如果提示信息对话框仍然可见，则关闭
    if (info->isVisible()) {
        info->close();
    }

    // 写入失败，发出错误提示
    if (success == false) {
        warning->showAndClose(5, tr("写入失败"), tr("请重新录入学号并刷卡重试"));
        appendLog(tr("管理员界面：写入学号失败"));
    }
    // 写入成功，写入日志
    else {
        appendLog(tr("管理员界面：向卡片内写入学号：") + studentNum + tr("成功"));
        // 写卡成功，那么将学号写入到数据库
        bool result = database->insertNewStudent(studentNum);
        if (result == false) {
            appendLog(tr("管理员界面：将学号写入数据库失败"));
            warning->showAndClose(5, tr("写入失败"), tr("将学号写入数据库时失败，请重试！"));
        }
    }

    // 写入完成，将初始化卡片信息按钮置为可用
    enableInitCardContentButton();
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
