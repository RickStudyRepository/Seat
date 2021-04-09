#ifndef COUNTDOWN_H
#define COUNTDOWN_H

#include <QLCDNumber>
#include <QTimer>
#include <QWidget>

class CountDown : public QLCDNumber
{
    Q_OBJECT

private:
    QTimer* timer = new QTimer(this);
    int time;
    const int maxTime;
    const int oneSecond = 1000;

public:
    CountDown(QWidget *parent = 0, int maxTime = 0)
        : QLCDNumber(parent), maxTime(maxTime)
    {
        // 设置显示的数字个数
        setDigitCount(std::to_string(maxTime).length());
        // 默认设置，字母样式
        setSegmentStyle(QLCDNumber::Filled);
        // 设置成无边框
        setFrameStyle(QFrame::NoFrame);
        setFixedWidth(40);
        setStyleSheet("color:black");

        // 绑定槽函数
        connect(timer, SIGNAL(timeout()), this, SLOT(decreaseTime()));
    }
    // 开始倒计时
    void startCountDown() {
        time = maxTime;
        timer->start(oneSecond);
        display(time);
    }
    // 停止倒计时
    void stopCountDown() {
        timer->stop();
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
            emit countZero();
        }
    }

    // 每当用户有操作时，重置倒计时
    void resetTime() {
        time = maxTime;
        display(time);
        timer->stop();
        timer->start(oneSecond);
    }

signals:
    // 发送倒计时结束的信号
    void countZero();
};

#endif // COUNTDOWN_H
