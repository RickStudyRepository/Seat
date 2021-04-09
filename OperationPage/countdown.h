#ifndef COUNTDOWN_H
#define COUNTDOWN_H

#include <QLCDNumber>
#include <QTimer>

class CountDown : public QLCDNumber
{
    Q_OBJECT

public:
    CountDown(QWidget *parent = 0, int maxTime = 0);
    // 开始倒计时
    void startCountDown();
    // 停止倒计时
    void stopCountDown();

private:
    QTimer* timer = new QTimer(this);
    int time;
    const int maxTime;

private slots:
    // 逐秒减少时间并展示时间
    void decreaseTime();

signals:
    // 发送倒计时结束的信号
    void countZero();
};

#endif // COUNTDOWN_H
