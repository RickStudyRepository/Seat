#include "RfidThread.h"
#include "../TTY/tty.h"
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <QDebug>
#include <QMessageBox>

#include "../../Tools/MacroDefinition.h"

RfidThread::RfidThread(QObject *parent) :
    QThread(parent) {
    uart_open(COM1_ID);    //  connect rfid modules
    tty[COM1_ID].baud = B19200;
    uart_setbaud(COM1_ID);
}

void RfidThread::run() {
    int ret;
    qDebug()<<"rfid thread starting!";
    runFlag = false;
    while(runFlag == true)
    {

    }
}
