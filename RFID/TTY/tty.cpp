#include "tty.h"
#include "../../Tools/Tools.h"
#include "../../Tools/MacroDefinition.h"

using namespace MacroDefinition;

#include <QDebug>
#include <QString>

#include <sys/types.h>  /* open() */
#include <sys/stat.h>  /* open() */
#include <fcntl.h>  /* open() */
#include <unistd.h>  /* close() */
#include <string.h>  /* bzero() */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

static struct termios oldtio, newtio;

tty_info_t tty[MAX_COM_NUM];


int tty_end(int _fd) {
    tcsetattr(_fd, TCSANOW, &oldtio);	 	/* restore old modem setings */
    close(_fd);
    return 0;
}

int tty_read(int _fd, uint8* buf, int nbytes) {
    return read(_fd, buf, nbytes);
}

int tty_write(int _fd, uint8* buf, int nbytes) {
    int i;
    for(i=0; i<nbytes; i++) {
        write(_fd, &buf[i], 1);
        usleep(100);
    }
    return tcdrain(_fd);
}

int tty_init(const char* _name, int _baud) {
    int fd = open(_name, O_RDWR | O_NOCTTY); //| O_NONBLOCK);//)
    if (fd <0) {
        qDebug("Err:%s open failed!", _name);
        return -1;
    }
    else {
        qDebug("%s open successful!", _name);
    }

    tcgetattr(fd, &oldtio); /* save current modem settings */
    bzero(&newtio, sizeof(newtio));

    newtio.c_cflag = /*BAUDRATE*/_baud | /*CRTSCTS |*/ CS8 | CLOCAL | CREAD ;
    newtio.c_iflag = IGNPAR/*| ICRNL*/;
    newtio.c_iflag &= ~(IXON | IXOFF | IXANY); // by sprife

    newtio.c_oflag = 0;
    newtio.c_lflag = 0;

    //newtio.c_oflag &= ~(OPOST);
    //newtio.c_lflag &= ~ICANON;

    newtio.c_cc[VINTR]    = 0;     /* Ctrl-c */
    newtio.c_cc[VQUIT]    = 0;     /* Ctrl-\ */
    newtio.c_cc[VERASE]   = 0;     /* del */
    newtio.c_cc[VKILL]    = 0;     /* @ */
    newtio.c_cc[VEOF]     = 4;     /* Ctrl-d */

    newtio.c_cc[VTIME]    = 0;     /* ��ʹ�÷ָ���Ԫ���ļ�ʱ�� */
    newtio.c_cc[VMIN]     = 1;     /* �ڶ�ȡ�� 1 ����Ԫǰ��ֹͣ */

    newtio.c_cc[VSWTC]    = 0;     /* '\0' */
    newtio.c_cc[VSTART]   = 0;     /* Ctrl-q */
    newtio.c_cc[VSTOP]    = 0;     /* Ctrl-s */
    newtio.c_cc[VSUSP]    = 0;     /* Ctrl-z */
    newtio.c_cc[VEOL]     = 0;     /* '\0' */
    newtio.c_cc[VREPRINT] = 0;     /* Ctrl-r */
    newtio.c_cc[VDISCARD] = 0;     /* Ctrl-u */
    newtio.c_cc[VWERASE]  = 0;     /* Ctrl-w */
    newtio.c_cc[VLNEXT]   = 0;     /* Ctrl-v */
    newtio.c_cc[VEOL2]    = 0;     /* '\0' */

    tcflush(fd, TCIFLUSH);
    tcsetattr(fd, TCSANOW, &newtio);/*set attrib	  */

    return fd;
}

void tty_fflush(int _fd) {
    tcflush(_fd, TCIFLUSH);//ˢ�����������
}

int uart_open(int id) {
    tty[id].timeout = 10000;
    tty[id].txlen = 0;
    tty[id].rxlen = 0;
    tty[id].update = 0;
    tty[id].status = TTY_OFF;

    // 选择输出串口并初始化设置波特率
    if(id == COM1_ID) {
        tty[id].baud = B9600;
        tty[id].fd = tty_init(COM1, tty[id].baud);
    }
    else if(id == COM2_ID) {
        tty[id].baud = B115200;
        tty[id].fd = tty_init(COM2, tty[id].baud);
    }
    else {
        qDebug("COM_ID error!");
    }

    return 0;
}

int uart_write(int id) {
    PrintUartTxBuf(id);
    tty_write(tty[id].fd, tty[id].txbuf, tty[id].txlen);
    return 0;
}

int uart_read(int id) {
    struct timeval tv;
    fd_set fds;
    int ret;

    tty[id].rxlen = 0;
    tv.tv_sec = 0;
    tv.tv_usec = tty[id].timeout;

    FD_ZERO(&fds);
    FD_SET(tty[id].fd, &fds);

    ret = select(1 + tty[id].fd, &fds, NULL, NULL, &tv);
    if(ret > 0) {
        if(FD_ISSET(tty[id].fd, &fds)) {
            tty[id].rxlen = tty_read(tty[id].fd, tty[id].rxbuf, 256);
           // PrintUartRxBuf(id);
        }
    }

    return 0;
}

int uart_setbaud(int id) {
    tcgetattr(tty[id].fd, &newtio); /* get current modem settings */
    newtio.c_cflag &= ~0010017;
    newtio.c_cflag |= tty[id].baud;

    tcflush(tty[id].fd, TCIFLUSH);
    tcsetattr(tty[id].fd, TCSANOW, &newtio);/*set attrib	  */

    return 0;
}

void PrintUartTxBuf(int id) {
    QString str;
    Tools::HexToQString(str, tty[id].txbuf, tty[id].txlen);
    qDebug() << "UART_TX:" << str;
}
void PrintUartRxBuf(int id) {
    QString str;
    Tools::HexToQString(str, tty[id].rxbuf, tty[id].rxlen);
    qDebug() << "UART_RX" << str;
}
