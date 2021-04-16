#ifndef RFID_H
#define RFID_H

#include <QWidget>
#include <QString>

#include "HF13Point56M/ISO14443/ISO14443.h"
#include "RFIDThread/RfidThread.h"

class RFID : public QWidget
{
    Q_OBJECT
private:
    RFID(QWidget* parent = NULL);

    // 单例变量
    static RFID* singleRFID;

public:
    // 获取单例变量
    static RFID*& getRFID(QWidget* parent = NULL);

    ~RFID();

private:
    RfidThread* rfidThread;
    ISO14443 rfidProtocol;

    // 是否被初始化了，也就是是否设置了协议为14443
    bool isInited;
    // 是否正在执行操作
    bool isOperating;

    // 初始化RFID协议
    bool initRFID();

    // 寻找感应区内的卡片并锁定
    bool selectCard();

public slots:

    // 初始化卡片内的学号
    bool writeStudentNum(QString studentNum);

    // 读取卡片内的内容，等待时间应该为1s
    bool readStudentNum(QString& studentNum);

signals:
    // 日志信号
    void logSignal(QString);
};

#endif // RFID_H
