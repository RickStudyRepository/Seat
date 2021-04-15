#include "../rc632/rc632.h"
#include "../../tty/tty.h"
#include "iso14443.h"
#include <unistd.h>

ISO14443::ISO14443(QObject *parent) :
    QObject(parent)
{
}

int ISO14443::Sel14443Protocol(void)
{
    int cnt=5;
    while(cnt--)
    {
        if(RC632_OK == CloseAnt())
        {
            if(RC632_OK == SelISO14443A())
            {
                if(RC632_OK == OpenAnt())
                    break;
            }
        }
        usleep(10000);
    }

    return cnt;
}

int ISO14443::SelISO14443A(void)     // 设置RC632工作方式为ISO14443_A
{
    uint8 data[1];

    data[0] = 0x41;     //  iso14443 type
    tty[COM1_ID].txlen = RC632_BuildReqFrame(tty[COM1_ID].txbuf, RC632_CMD_CONFIG_ISOTYPE, data, 1);
    uart_write(COM1_ID);

    return ReceiveFrame(100000, RC632_CMD_CONFIG_ISOTYPE);
}
int ISO14443::OpenAnt(void)    // 打开天线
{
    uint8 data[1];

    data[0] = 0x01;     //  open ant
    tty[COM1_ID].txlen = RC632_BuildReqFrame(tty[COM1_ID].txbuf, RC632_CMD_ANT_CONTROL, data, 1);
    uart_write(COM1_ID);

    return ReceiveFrame(100000, RC632_CMD_ANT_CONTROL);
}
int ISO14443::CloseAnt(void)   // 关闭天线
{
    uint8 data[1];

    data[0] = 0x00;     //  close ant
    tty[COM1_ID].txlen = RC632_BuildReqFrame(tty[COM1_ID].txbuf, RC632_CMD_ANT_CONTROL, data, 1);
    uart_write(COM1_ID);

    return ReceiveFrame(100000, RC632_CMD_ANT_CONTROL);
}
int ISO14443::RequestA(uint8 mode)  // 寻感应区内所有符合14443A标准的卡
{
    uint8 data[1];

    data[0] = mode;
    tty[COM1_ID].txlen = RC632_BuildReqFrame(tty[COM1_ID].txbuf, RC632_CMD_REQUEST_A, data, 1);
    uart_write(COM1_ID);

    return ReceiveFrame(1000000, RC632_CMD_REQUEST_A);
}
int ISO14443::TagSleep(void)
{
    uint8 *data;

    tty[COM1_ID].txlen = RC632_BuildReqFrame(tty[COM1_ID].txbuf, RC632_CMD_SLEEP, data, 0);
    uart_write(COM1_ID);

    return ReceiveFrame(1000000, RC632_CMD_SLEEP);
}
int ISO14443::AnticollA(void)   // 发送A卡防冲撞命
{
    uint8 data[1];

    data[0] = 0x04;
    tty[COM1_ID].txlen = RC632_BuildReqFrame(tty[COM1_ID].txbuf, RC632_CMD_ANTICOLL, data, 1);
    uart_write(COM1_ID);

    return ReceiveFrame(1000000, RC632_CMD_ANTICOLL);
}
int ISO14443::SelectA(uint8 *tagId)    // 发送A卡锁定命令
{
    tty[COM1_ID].txlen = RC632_BuildReqFrame(tty[COM1_ID].txbuf, RC632_CMD_SELECT, tagId, 4);
    uart_write(COM1_ID);

    return ReceiveFrame(1000000, RC632_CMD_SELECT);
}
int ISO14443::Authentication(uint8 *password, uint8 sectorId)    // 发送A卡验证密钥命令
{
    uint8 data[8];

    data[0] = 0x60;
    data[1] = sectorId<<2;
    memcpy(data+2, password, 6);
    tty[COM1_ID].txlen = RC632_BuildReqFrame(tty[COM1_ID].txbuf, RC632_CMD_AUTHENTICATION, data, 8);
    uart_write(COM1_ID);

    return ReceiveFrame(1000000, RC632_CMD_AUTHENTICATION);
}
int ISO14443::M1Init(uint8 sectorId, uint8 blockId, uint32 val)    //  M1钱包初始化
{
    uint8 data[5];
    data[0] = (sectorId<<2) + blockId;
    data[1] = BREAK_UINT32(val, 3);
    data[2] = BREAK_UINT32(val, 2);
    data[3] = BREAK_UINT32(val, 1);
    data[4] = BREAK_UINT32(val, 0);
    tty[COM1_ID].txlen = RC632_BuildReqFrame(tty[COM1_ID].txbuf, RC632_CMD_M1INITVAL, data, 5);
    uart_write(COM1_ID);

    return ReceiveFrame(1000000, RC632_CMD_M1INITVAL);
}
int ISO14443::M1Increment(uint8 sectorId, uint8 blockId, uint32 val)   // M1钱包充值
{
    uint8 data[5];
    data[0] = (sectorId<<2) + blockId;
    data[1] = BREAK_UINT32(val, 3);
    data[2] = BREAK_UINT32(val, 2);
    data[3] = BREAK_UINT32(val, 1);
    data[4] = BREAK_UINT32(val, 0);
    tty[COM1_ID].txlen = RC632_BuildReqFrame(tty[COM1_ID].txbuf, RC632_CMD_M1INCREMENT, data, 5);
    uart_write(COM1_ID);

    return ReceiveFrame(1000000, RC632_CMD_M1INCREMENT);
}
int ISO14443::M1Decrement(uint8 sectorId, uint8 blockId, uint32 val)   // M1钱包扣费
{
    uint8 data[5];
    data[0] = (sectorId<<2) + blockId;
    data[1] = BREAK_UINT32(val, 3);
    data[2] = BREAK_UINT32(val, 2);
    data[3] = BREAK_UINT32(val, 1);
    data[4] = BREAK_UINT32(val, 0);

    tty[COM1_ID].txlen = RC632_BuildReqFrame(tty[COM1_ID].txbuf, RC632_CMD_M1DECREMENT, data, 5);
    uart_write(COM1_ID);

    return ReceiveFrame(1000000, RC632_CMD_M1DECREMENT);
}
int ISO14443::M1Balance(uint8 sectorId, uint8 blockId)   // M1钱包余额
{
    uint8 data[1];
    data[0] = (sectorId<<2) + blockId;

    tty[COM1_ID].txlen = RC632_BuildReqFrame(tty[COM1_ID].txbuf, RC632_CMD_M1READVAL, data, 1);
    uart_write(COM1_ID);

    return ReceiveFrame(1000000, RC632_CMD_M1READVAL);
}

int ISO14443::MemRead(uint8 sectorId, uint8 blockId)
{
    uint8 data[1];
    data[0] = (sectorId<<2)+blockId;

    tty[COM1_ID].txlen = RC632_BuildReqFrame(tty[COM1_ID].txbuf, RC632_CMD_M1READ, data, 1);
    uart_write(COM1_ID);

    return ReceiveFrame(1000000, RC632_CMD_M1READ);
}
int ISO14443::MemWrite(uint8 sectorId, uint8 blockId, uint8 *buf, uint8 len)
{
    uint8 data[17];
    data[0] = (sectorId<<2) + blockId;
    memcpy(data+1, buf, len);
    tty[COM1_ID].txlen = RC632_BuildReqFrame(tty[COM1_ID].txbuf, RC632_CMD_M1WRITE, data, len+1);
    uart_write(COM1_ID);

    return ReceiveFrame(1000000, RC632_CMD_M1WRITE);
}

int ISO14443::ReceiveFrame(long timeout, uint16 cmd)
{
    uint8 nsopl = 0;
    uint8 *data=NULL;
    uint8 *p = tty[COM1_ID].rxbuf;
    uint8 len, i=0;
    uint8 dataLen = 0, tempLen=0;
    int ret = -1, recSta = RC632_STA_SOP1;
    tty[COM1_ID].timeout = timeout;

    while(1)
    {
        uart_read(COM1_ID);
        len = tty[COM1_ID].rxlen;
        if(len == 0)   return ret;

        i = 0;
        do{
            switch(recSta)
            {
            case RC632_STA_SOP1:
                if(p[i++] == 0xAA)   recSta = RC632_STA_SOP2;
                break;
            case RC632_STA_SOP2:
                if(p[i++] == 0xBB)   recSta = RC632_STA_LEN1;
                else recSta = RC632_STA_SOP1;
                break;
            case RC632_STA_LEN1:
                dataLen = p[i++];
                recSta = RC632_STA_LEN2;
                break;
            case RC632_STA_LEN2:
                tempLen = 0;
                nsopl = 0;
                i++;
                data = new uint8[dataLen];
                recSta = RC632_STA_DAT;
                break;
            case RC632_STA_DAT:
                if(nsopl == 1){
                    nsopl = 0;
                    i++;
                    break;
                }
                if(p[i] == RC632_FRAME_SOPL)  nsopl=1;
                data[tempLen++] = p[i++];
                if(tempLen == dataLen)
                {
                    len++;
                    recSta = RC632_STA_FCS;
                }
                break;
            case RC632_STA_FCS:
                if(cmd != BUILD_UINT16(data[2], data[3]))
                {
                    delete [] data;
                    recSta = RC632_STA_SOP1;
                }
                else
                {
                       memcpy(p+4, data, dataLen);
                       ret = data[4];
                       delete [] data;
                       return ret;
                }
            }
        }while(--len);
    }

    return ret;
}

uint8 ISO14443::RFID_Set14443Type()
{
    uint8 ret =RC632_FAULT254;
    if(RC632_OK == CloseAnt()) {
        if(RC632_OK == SelISO14443A()){
            if(RC632_OK == OpenAnt())
                ret = RC632_OK;
        }
    }

    return ret;
}

uint8 ISO14443::RFID_Authentication(uint8 *password, uint8 sectorId)
{
    uint8 ret = RC632_FAULT254;
    if(password == NULL)  memset(password, 0xFF, 6);
    if(RC632_OK == RFID_InventoryALL()){
        if(RC632_OK == Authentication(password, sectorId)){
            ret = RC632_OK;
        }
    }

    return ret;
}

uint8 ISO14443::RFID_InventoryALL(void)
{
    uint8 ret = RC632_FAULT254;
    if(RC632_OK == RequestA(0x52)){
        if(RC632_OK == AnticollA()){
            uint8 *p = tty[COM1_ID].rxbuf;
            memcpy(tagId, p+9, 4);
            if(RC632_OK == SelectA(tagId)){
                ret = RC632_OK;
            }
        }
    }
    return ret;
}
uint8 ISO14443::RFID_InventoryIDL(void)
{
    uint8 ret = RC632_FAULT254;
    if(RC632_OK == RequestA(0x26)){
        if(RC632_OK == AnticollA()){
            uint8 *p = tty[COM1_ID].rxbuf;
            memcpy(tagId, p+9, 4);
            if(RC632_OK == SelectA(tagId)){
                ret = RC632_OK;
            }
        }
    }
    return ret;
}
uint8 ISO14443::RFID_Sleep(void)
{
    uint8 ret = RC632_FAULT254;
    if(RC632_OK == TagSleep()){
        ret = RC632_OK;
    }
    return ret;
}

uint8 ISO14443::RFID_M1Init(const int sectorId, const int blockId, const uint32 money, uint8 *password)
{
    uint8 ret = RC632_FAULT254;
    if(RC632_OK == RFID_Authentication(password, sectorId)){
        if(RC632_OK == M1Init(sectorId, blockId, money)){
            ret  = RC632_OK;
        }
    }
    return ret;
}
uint8 ISO14443::RFID_M1Increment(const int sectorId, const int blockId, const uint32 money, uint8 *password)
{
    uint8 ret = RC632_FAULT254;
    if(RC632_OK == RFID_Authentication(password, sectorId)){
        if(RC632_OK == M1Increment(sectorId, blockId, money)){
            ret  = RC632_OK;
        }
    }
    return ret;
}
uint8 ISO14443::RFID_M1Decrement(const int sectorId, const int blockId, const uint32 money, uint8 *password)
{
    uint8 ret = RC632_FAULT254;
    if(RC632_OK == RFID_Authentication(password, sectorId)){
        if(RC632_OK == M1Decrement(sectorId, blockId, money)){
            ret  = RC632_OK;
        }
    }
    return ret;
}
uint8 ISO14443::RFID_M1Balance(const int sectorId, const int blockId, uint8 *password)
{
    uint8 ret = RC632_FAULT254;
    if(RC632_OK == RFID_Authentication(password, sectorId)){
        if(RC632_OK == M1Balance(sectorId, blockId)){
            uint8 *p = tty[COM1_ID].rxbuf;
            p += 9;
            balance = BUILD_UINT32(p[0], p[1], p[2], p[3]);
            ret  = RC632_OK;
        }
    }
    return ret;
}
uint8 ISO14443::RFID_MemRead(const int sectorId, const int blockId, uint8 *password)
{
    uint8 ret = RC632_FAULT254;
    if(RC632_OK == RFID_Authentication(password, sectorId)){
        if(RC632_OK == MemRead(sectorId, blockId)){
            uint8 *p = tty[COM1_ID].rxbuf;
            p += 9;
            memcpy(mem[blockId], p, 16);
            ret  = RC632_OK;
        }
    }
    return ret;
}
uint8 ISO14443::RFID_MemWrite(const int sectorId, const int blockId, uint8 *buf, uint8 len, uint8 *password)
{
    uint8 ret = RC632_FAULT254;
    if(RC632_OK == RFID_Authentication(password, sectorId)){
        if(RC632_OK == MemWrite(sectorId, blockId, buf, len)){
            ret  = RC632_OK;
        }
    }
    return ret;
}
