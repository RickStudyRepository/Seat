#ifndef RFIDTHREAD_H
#define RFIDTHREAD_H

#include <QThread>
#include "../public/public.h"

#define SYSTEM_NO         0
#define SYSTEM_125K      1
#define SYSTEM_14443    2
#define SYSTEM_15693    3
#define SYSTEM_900M     4
#define SYSTEM_2400M   5

#define UART_ADDR       _IOW(0x02,1,unsigned long)

#define _UART1_SEL_125K     0
#define _UART1_SEL_14443    1
#define _UART1_SEL_15693    2
#define _UART1_SEL_900M     3
#define _UART1_SEL_2P4G     4
#define _UART1_SEL_UART1    5

#define _UART2_SEL_LOCK     10
#define _UART2_SEL_GATE     11
#define _UART2_SEL_GATE2    12
#define _UART2_SEL_UART2    13

class MainWidget;

class RfidThread : public QThread
{
    Q_OBJECT
public:
    explicit RfidThread(QObject *parent = 0);

    void run();

signals:
    
public slots:

private:

public:
    int cpld_fd;
    bool runFlag;
    uint8 tagId[6];
};

#endif // RFIDTHREAD_H
