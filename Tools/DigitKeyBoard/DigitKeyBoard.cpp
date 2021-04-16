#include "DigitKeyBoard.h"
#include "../FontFactory.h"
#include <QDebug>

// 单例变量
DigitKeyBoard* DigitKeyBoard::singleKeyBoard = NULL;

DigitKeyBoard*& DigitKeyBoard::getKeyBoard(QWidget *parent) {
    if (DigitKeyBoard::singleKeyBoard != NULL) {
        return DigitKeyBoard::singleKeyBoard;
    }
    else {
        singleKeyBoard = new DigitKeyBoard(parent);
        return singleKeyBoard;
    }
}

DigitKeyBoard::DigitKeyBoard(QWidget *parent)
    : QWidget(parent) {
    initButtons();
    initSignalMapper();
    initLayout();
    setMaximumSize(420, 130);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
}

DigitKeyBoard::~DigitKeyBoard() {
    delete DigitKeyBoard::singleKeyBoard;
}

void DigitKeyBoard::initButtons() {
    for (int i = 0; i < 10; i++) {
        numberButtons.push_back(new QPushButton(QString::number(i), this));
        numberButtons[i]->setFont(FontFactory::describeFont());
    }
    backspaceButton = new QPushButton(tr("回退"), this);
    backspaceButton->setFont(FontFactory::describeFont());
    confirmButton = new QPushButton(tr("确认"), this);
    confirmButton->setFont(FontFactory::describeFont());
}

void DigitKeyBoard::initSignalMapper() {
    signalMapper = new QSignalMapper(this);
    // 数字按键映射
    for (int i = 0; i < 10; i++) {
        connect(numberButtons[i], SIGNAL(released()), signalMapper, SLOT(map()));
    }
    connect(backspaceButton, SIGNAL(released()), signalMapper, SLOT(map()));
    connect(confirmButton, SIGNAL(released()), signalMapper, SLOT(map()));

    // 设置映射的字符串
    for (int i = 0; i < 10; i++) {
        signalMapper->setMapping(numberButtons[i], QString::number(i));
    }
    signalMapper->setMapping(backspaceButton, tr("backspace"));
    signalMapper->setMapping(confirmButton, tr("confirm"));

    // 绑定按键响应槽函数
    connect(signalMapper, SIGNAL(mapped(QString)), this, SLOT(actionButton(QString)));
}

void DigitKeyBoard::initLayout() {
    layout = new QGridLayout(this);
    for (int i = 0; i < 9; i++) {
        layout->addWidget(numberButtons[i + 1], i / 3, i % 3, 1, 1);
    }
    layout->addWidget(numberButtons[0], 0, 3, 1, 1);
    layout->addWidget(backspaceButton, 1, 3, 1, 1);
    layout->addWidget(confirmButton, 2, 3, 1, 1);
    setLayout(layout);
}

void DigitKeyBoard::startInput(QLineEdit* output) {
    emit logSignal(tr("数字键盘：开始向") + QString::number((int)output) + tr("指向的文本框输入信息"));
    this->output = output;
    show();
}

void DigitKeyBoard::actionButton(QString buttonText) {
    // 确认输入，隐藏数字键盘
    if (buttonText == tr("confirm")) {
        hide();
        // 本次输入结束，重置输出文本框
        output = NULL;
        emit logSignal(tr("数字键盘：输入结束，隐藏数字键盘"));
    }
    // 删除光标前的一个字符
    else if (buttonText == "backspace") {
        output->backspace();
        emit logSignal(tr("数字键盘：删除目标文本框光标前的一个字符"));
    }
    // 在光标前插入一个字符串
    else {
        output->insert(buttonText);
        emit logSignal(tr("数字键盘：向文本框添加文本：") + buttonText);
    }
}
