#ifndef TTY_H
#define TTY_H

#include <termios.h>

#define TTY_ON    0x01
#define TTY_OFF   0x00

#define MAX_COM_NUM  2
#define COM1_ID    0
#define COM2_ID    1
// 这里指明串口名称，如果使用USB转串口，需要设置为相应的USB设备名
#define COM1 "/dev/ttymxc4"
#define COM2 "/dev/ttyS3"

typedef struct tty_info
{
    int fd;
    int baud;
    long timeout;  //usec
    char *name;
    unsigned char txbuf[256];
    unsigned char rxbuf[256];
    unsigned char txlen;   // the length of txbuf
    unsigned char rxlen;   // the length of rxbuf
    int status;              //  1:  uart open    0: uart close
    int update;             //  is buf updated?
}tty_info_t;

extern tty_info_t  tty[MAX_COM_NUM];

extern int tty_init(const char* _name, int _baud);
extern int tty_end(int _fd);
extern int tty_read(int _fd,unsigned char *buf,int nbytes);
extern int tty_write(int _fd,unsigned char *buf,int nbytes);
extern int tty_writecmd(int _fd,unsigned char *buf,int nbytes);
extern void tty_fflush(int _fd);


extern void PrintUartTxBuf(int id);
extern void PrintUartRxBuf(int id);

extern int uart_open(int id);
//extern int uart_close(tty_info_t *tty);
extern int uart_write(int id);
extern int uart_read(int id);
extern int uart_setbaud(int id);

#endif // TTY_H
