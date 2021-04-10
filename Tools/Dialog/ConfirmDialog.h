#ifndef CONFIRMDIALOG_H
#define CONFIRMDIALOG_H

#include <QWidget>
#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include "../Tools.h"

class ConfirmDialog : public QDialog {
    Q_OBJECT
private:
    QGridLayout* layout = new QGridLayout(this);
    QLabel* confirmStringLabel = new QLabel(this);
    QPushButton* confirmButton = new QPushButton(this);
    QPushButton* cancelButton = new QPushButton(tr("我再看看"), this);

public:
    ConfirmDialog(QWidget* parent = NULL)
        : QDialog(parent) {
        // 信号和槽绑定
        connect(confirmButton, SIGNAL(released()), this , SLOT(confirm()));
        connect(cancelButton, SIGNAL(released()), this, SLOT(close()));

        // 窗体属性设置
        setFixedSize(290, 130);
        // 设置为只能当前窗口活动
        setWindowModality(Qt::ApplicationModal);

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
        show();
    }

signals:
    void confirmed();

private slots:
    void confirm() {
        // 发送确认预约的信号
        emit confirmed();
        close();
    }
};

#endif // CONFIRMDIALOG_H
