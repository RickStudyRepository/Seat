#include "rfidthread.h"
#include "../tty/tty.h"
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <QDebug>
#include <QMessageBox>

//#define CPLD_DEV "/dev/cpld"

RfidThread::RfidThread(QObject *parent) :
    QThread(parent)
{
/*    cpld_fd = open(CPLD_DEV, O_RDWR);  // open cpld driver
    if(cpld_fd<0)
    {
        qDebug("Error opening %s device.", CPLD_DEV);
    }
    ioctl(cpld_fd, UART_ADDR, _UART1_SEL_14443);
*/
    uart_open(COM1_ID);    //  connect rfid modules
    tty[COM1_ID].baud = B19200;
    uart_setbaud(COM1_ID);
}

void RfidThread::run()
{
    int ret;
    qDebug()<<"rfid thread starting!";

    runFlag = false;

    while(runFlag == true)
    {

    }
}
