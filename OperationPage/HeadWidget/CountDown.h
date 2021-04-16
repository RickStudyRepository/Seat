#ifndef COUNTDOWN_H
#define COUNTDOWN_H

#include <QLCDNumber>
#include <QTimer>
#include <QString>
#include <QWidget>

class CountDown : public QLCDNumber
{
    Q_OBJECT

private:
    QTimer* timer;
    int time;
    const int maxTime;
    const int oneSecond;

public:
    CountDown(QWidget *parent = 0, int maxTime = 0)
        : QLCDNumber(parent), maxTime(maxTime), oneSecond(1000)
    {
        // 设置显示的数字个数
        setDigitCount(QString::number(maxTime).length());
        // 默认设置，字母样式
        setSegmentStyle(QLCDNumber::Filled);
        // 设置成无边框
        setFrameStyle(QFrame::NoFrame);
        setFixedWidth(40);
        setStyleSheet("color:black");

        timer = new QTimer(this);
        // 绑定槽函数
        connect(timer, SIGNAL(timeout()), this, SLOT(decreaseTime()));
    }

    // 开始倒计时
    void startCountDown() {
        time = maxTime;
        timer->start(oneSecond);
        display(time);
        emit logSignal(tr("倒计时：开始计时，时长为：") + QString::number(maxTime));
    }

    // 停止倒计时
    void stopCountDown() {
        timer->stop();
        emit logSignal(tr("倒计时：停止计时，剩余时长为：") + QString::number(time));
    }

private slots:
    // 逐秒减少时间并展示时间
    void decreaseTime() {
        time -= 1;
        display(time);
        // 计时结束，发送返回首页信号
        if (time < 1) {
            // 停止计时器
            timer->stop();
            emit logSignal(tr("倒计时：计时结束"));
            emit countZero();
        }
    }

    // 每当用户有操作时，重置倒计时，暂未用到
    void resetTime() {
        time = maxTime;
        display(time);
        timer->stop();
        timer->start(oneSecond);
    }

signals:
    // 发送倒计时结束的信号
    void countZero();
    // 日志信息信号
    void logSignal(QString);
};

#endif // COUNTDOWN_H
