#ifndef TTY_H
#define TTY_H

#include <termios.h>
#include "../../Tools/MacroDefinition.h"

using namespace MacroDefinition;

typedef struct tty_info {
    int fd;
    int baud;
    long timeout;  //usec
    char *name;
    uint8 txbuf[256];
    uint8 rxbuf[256];
    uint8 txlen;   // the length of txbuf
    uint8 rxlen;   // the length of rxbuf
    int status;              //  1:  uart open    0: uart close
    int update;             //  is buf updated?
} tty_info_t;

extern tty_info_t tty[MAX_COM_NUM];

extern int tty_init(const char* _name, int _baud);
extern int tty_end(int _fd);
extern int tty_read(int _fd, uint8* buf, int nbytes);
extern int tty_write(int _fd, uint8* buf, int nbytes);
extern int tty_writecmd(int _fd, uint8* buf, int nbytes);
extern void tty_fflush(int _fd);


extern void PrintUartTxBuf(int id);
extern void PrintUartRxBuf(int id);

extern int uart_open(int id);
//extern int uart_close(tty_info_t *tty);
extern int uart_write(int id);
extern int uart_read(int id);
extern int uart_setbaud(int id);

#endif // TTY_H
