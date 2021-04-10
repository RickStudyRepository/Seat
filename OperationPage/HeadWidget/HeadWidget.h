#ifndef HEADWIDGET_H
#define HEADWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QString>
#include <QPushButton>
#include <QGridLayout>
#include <QIcon>
#include "CountDown.h"

class HeadWidget : public QWidget
{
    Q_OBJECT
private:
    // 操作界面顶部部件
    QLabel* logInLabel = new QLabel(this);
    // 登录提示字符串
    QString logInString = tr("欢迎您%1！今天又是美好的一天呐！");
    // 返回倒计时
    const int maxTime = 180;
    CountDown* countDown = new CountDown(this, maxTime);
    // 返回首页按钮
    QPushButton* returnHomePage = new QPushButton(tr("返回"), this);
    // 布局
    QGridLayout* layout = new QGridLayout(this);

public:
    explicit HeadWidget(QWidget *parent = NULL)
        : QWidget(parent)
    {
        returnHomePage->setIcon(QIcon(":/images/ReturnHomePage.svg"));
        connect(returnHomePage, SIGNAL(released()), this, SLOT(emitReturnHomePage()));
        connect(countDown, SIGNAL(countZero()), this, SLOT(emitReturnHomePage()));
        layout->addWidget(logInLabel, 0, 0, 1, 6);
        layout->addWidget(countDown, 0, 7, 1, 1);
        layout->addWidget(returnHomePage, 0, 8, 1, 1);
        layout->setMargin(0);
        setLayout(layout);
    }

    void resetLogInLabelText(QString studentNum) {
        logInLabel->setText(logInString.arg(studentNum));
    }

    void startCountDown() {
        countDown->startCountDown();
    }

    void stopCountDown() {
        countDown->stopCountDown();
    }

signals:
    void returnHomePageSignal();

public slots:
    void emitReturnHomePage() {
        emit returnHomePageSignal();
    }
};

#endif // HEADWIDGET_H
