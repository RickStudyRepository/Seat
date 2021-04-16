#ifndef ISO14443_H
#define ISO14443_H

#include <QObject>
#include "../RC632/RC632.h"
#include "../../../Tools/MacroDefinition.h"

using namespace MacroDefinition;

class ISO14443 : public QObject {
    Q_OBJECT
public:
    explicit ISO14443(QObject *parent = 0);
    
    int Sel14443Protocol(void);

    int SelISO14443A(void);     // 设置RC632工作方式为ISO14443_A
    int OpenAnt(void);    // 打开天线
    int CloseAnt(void);   // 关闭天线
    int RequestA(uint8 mode);  // 寻感应区内所有符合14443A标准的卡
    int TagSleep(void);
    int AnticollA(void);   // 发送A卡防冲撞命
    int SelectA(uint8 *tagId);    // 发送A卡锁定命令
    int Authentication(uint8 *password, uint8 sectorId);    // 发送A卡验证密钥命令

    int M1Init(uint8 sectorId, uint8 blockId, uint32 val);    //  M1钱包初始化
    int M1Increment(uint8 sectorId, uint8 blockId, uint32 val);   // M1钱包充值
    int M1Decrement(uint8 sectorId, uint8 blockId, uint32 val);   // M1钱包扣费
    int M1Balance(uint8 sectorId, uint8 blockId);   // M1钱包余额
    int MemRead(uint8 sectorId, uint8 blockId);
    int MemWrite(uint8 sectorId, uint8 blockId, uint8 *buf, uint8 len);

    int ReceiveFrame(long timeout, uint16 cmd);

public:
    uint8 RFID_Set14443Type();
    uint8 RFID_Authentication(uint8 *password, uint8 sectorId);

    uint8 RFID_InventoryALL(void);
    uint8 RFID_InventoryIDL(void);
    uint8 RFID_Sleep(void);

    uint8 RFID_M1Init(const int sectorId, const int blockId, const uint32 money, uint8 *password = NULL);
    uint8 RFID_M1Increment(const int sectorId, const int blockId, const uint32 money, uint8 *password = NULL);
    uint8 RFID_M1Decrement(const int sectorId, const int blockId, const uint32 money, uint8 *password = NULL);
    uint8 RFID_M1Balance(const int sectorId, const int blockId, uint8 *password = NULL);
    uint8 RFID_MemRead(const int sectorId, const int blockId, uint8 *password = NULL);
    uint8 RFID_MemWrite(const int sectorId, const int blockId, uint8 *buf, uint8 len, uint8 *password = NULL);

signals:
    
public slots:

public:
    uint8 tagId[4];
    uint32 balance;
    uint8 mem[4][16];
};

#endif // ISO14443_H
