#ifndef CONFIRMDIALOG_H
#define CONFIRMDIALOG_H

#include <QWidget>
#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include "../Tools.h"
#include "../FontFactory.h"

class ConfirmDialog : public QDialog {
    Q_OBJECT
private:
    QGridLayout* layout;
    QLabel* confirmStringLabel;
    QPushButton* confirmButton;
    QPushButton* cancelButton;

public:
    ConfirmDialog(QWidget* parent = NULL)
        : QDialog(parent) {
        // 初始化确认消息标签
        confirmStringLabel = new QLabel(this);

        // 确认按钮初始化
        confirmButton = new QPushButton(this);
        // 取消按钮初始化
        cancelButton = new QPushButton(tr("我再看看"), this);

        // 按钮的信号和槽绑定
        connect(confirmButton, SIGNAL(released()), this , SLOT(confirm()));
        connect(cancelButton, SIGNAL(released()), this, SLOT(close()));
        connect(cancelButton, SIGNAL(released()), this, SIGNAL(cancel()));

        // 窗体属性设置
        setMinimumSize(290, 130);
        // 设置为只能当前窗口活动
        setWindowModality(Qt::ApplicationModal);
        // 设置字体为对话框字体
        setFont(FontFactory::dialogFont());

        // 初始化布局
        layout = new QGridLayout(this);
        layout->addWidget(confirmStringLabel, 0, 0, 1, 2);
        layout->addWidget(confirmButton, 1, 0, 1, 1);
        layout->addWidget(cancelButton, 1, 1, 1, 1);
        setLayout(layout);
    }

    void setConfirmButtonText(QString text) {
        confirmButton->setText(text);
    }

    void setTextAndShow(QString text) {
        confirmStringLabel->setText(text);
        emit logSignal(tr("确认对话框：将确认文本设置为：") + text + "，并显示");
        show();
    }

signals:
    // 确认
    void confirmed();
    // 取消
    void cancel();
    // 日志信息信号
    void logSignal(QString);

private slots:
    void confirm() {
        // 发送确认预约的信号
        emit confirmed();
        emit logSignal(tr("确认对话框：确认按钮被按下"));
        close();
    }
};

#endif // CONFIRMDIALOG_H
