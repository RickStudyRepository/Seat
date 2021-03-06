#ifndef INPUTDIALOG_H
#define INPUTDIALOG_H

#include <QWidget>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include "../Tools.h"
#include "../FontFactory.h"

class InputDialog : public QDialog {
    Q_OBJECT
private:
    // 布局
    QGridLayout* layout;
    // 提示信息
    QLabel* tipLabel;
    // 输入的文本框
    QLineEdit* inputLineEdit;
    // 确认按钮
    QPushButton* confirmButton;
    // 取消输入按钮
    QPushButton* cancelButton;
    // 呼出输入键盘
    QPushButton* callDigitKeyBoard;

public:
    InputDialog(QWidget* parent = NULL)
        : QDialog(parent) {
        // 窗体属性设置
        setMinimumSize(290, 130);
        setFont(FontFactory::dialogFont());

        initTipLabel();
        initButton();
        initLineEdit();
        initLayout();
    }

    void setLineEditPlaceholderText(QString placeHolderText) {
        inputLineEdit->setPlaceholderText(placeHolderText);
    }

    void setLineEditEchoMode(QLineEdit::EchoMode echoMode) {
        inputLineEdit->setEchoMode(echoMode);
    }

    void clearLineEdit() {
        inputLineEdit->clear();
        emit logSignal(tr("输入对话框：清空输入文本框"));
    }

    void setTip(QString text) {
        tipLabel->setText(text);
        emit logSignal(tr("输入对话框：设置输入提示为：") + text);
    }

    QLineEdit* getLineEdit() {
        return inputLineEdit;
    }

private:
    void initTipLabel() {
        tipLabel = new QLabel(this);
        tipLabel->setFont(FontFactory::describeFont());
    }

    void initLineEdit() {
        inputLineEdit = new QLineEdit(this);
        inputLineEdit->setFont(FontFactory::describeFont());
    }

    void initButton() {
        // 确认按钮
        confirmButton = new QPushButton(this);
        confirmButton->setText(tr("确认输入"));
        confirmButton->setFont(FontFactory::describeFont());
        connect(confirmButton, SIGNAL(released()), this, SLOT(confirm()));

        // 取消按钮
        cancelButton = new QPushButton(this);
        cancelButton->setText(tr("取消"));
        cancelButton->setFont(FontFactory::describeFont());
        connect(cancelButton, SIGNAL(released()), this, SLOT(close()));
        connect(cancelButton, SIGNAL(released()), this, SLOT(cancelInput()));

        // 呼出键盘按钮
        callDigitKeyBoard = new QPushButton(this);
        callDigitKeyBoard->setText(tr("打开键盘"));
        callDigitKeyBoard->setFont(FontFactory::describeFont());
        connect(callDigitKeyBoard, SIGNAL(released()), this, SLOT(emitShowKeyBoard()));
    }

    void initLayout() {
        layout = new QGridLayout(this);
        layout->addWidget(tipLabel, 0, 0, 1, 1);
        layout->addWidget(inputLineEdit, 0, 1, 1, 2);
        layout->addWidget(confirmButton, 1, 0, 1, 1);
        layout->addWidget(cancelButton, 1, 1, 1, 1);
        layout->addWidget(callDigitKeyBoard, 1, 2, 1, 1);
        setLayout(layout);
    }

signals:
    // 完成输入
    void confirmed(QString);
    // 呼出键盘的信号
    void showKeyBoardSignal(QLineEdit*);
    // 取消输入
    void cancel();
    // 日志信息信号
    void logSignal(QString);

private slots:
    // 确认输入
    void confirm() {
        emit logSignal(tr("输入对话框：确认输入"));
        emit confirmed(inputLineEdit->text());
    }

    // 取消输入
    void cancelInput() {
        emit logSignal(tr("输入对话框：取消输入"));
        emit cancel();
    }

    // 发送呼出键盘的信号
    void emitShowKeyBoard() {
        emit logSignal(tr("输入对话框：请求将数字键盘设置为活动窗口"));
        emit showKeyBoardSignal(inputLineEdit);
    }
};

#endif // INPUTDIALOG_H
