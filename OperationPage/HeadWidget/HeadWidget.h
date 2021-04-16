#ifndef HEADWIDGET_H
#define HEADWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QString>
#include <QPushButton>
#include <QGridLayout>
#include <QIcon>
#include "CountDown.h"
#include "../../Tools/FontFactory.h"
#include "../../Tools/ConstValue.h"

class HeadWidget : public QWidget
{
    Q_OBJECT
private:
    // 操作界面顶部部件
    QLabel* logInLabel;
    // 登录提示字符串
    const QString logInString;
    CountDown* countDown;
    // 返回首页按钮
    QPushButton* returnHomePage;
    // 布局
    QGridLayout* layout;

    // 绑定日志信号槽
    void connectLogString() {
        connect(countDown, SIGNAL(logSignal(QString)), this, SIGNAL(logSignal(QString)));
    }

public:
    explicit HeadWidget(QWidget *parent = NULL)
        : QWidget(parent), logInString(tr("欢迎您%1！今天又是美好的一天呐！"))
    {
        // 返回首页按钮
        returnHomePage = new QPushButton(tr("返回"), this);
        returnHomePage->setIcon(QIcon(":/images/ReturnHomePage.svg"));
        connect(returnHomePage, SIGNAL(released()), this, SLOT(emitReturnHomePage()));

        // 倒计时部件
        countDown = new CountDown(this, ConstValue::maxDuration);
        connect(countDown, SIGNAL(countZero()), this, SLOT(emitReturnHomePage()));

        // 针对登录人的欢迎标签
        logInLabel = new QLabel(this);

        // 初始化布局
        layout = new QGridLayout(this);
        layout->addWidget(logInLabel, 0, 0, 1, 6);
        layout->addWidget(countDown, 0, 7, 1, 1);
        layout->addWidget(returnHomePage, 0, 8, 1, 1);
        layout->setMargin(4);
        setLayout(layout);
        setFont(FontFactory::describeFont());
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
    // 日志信息信号
    void logSignal(QString);

public slots:
    void emitReturnHomePage() {
        emit logSignal(tr("操作界面顶部部件：倒计时结束或返回按钮按下，返回首页"));
        emit returnHomePageSignal();
    }
};

#endif // HEADWIDGET_H
