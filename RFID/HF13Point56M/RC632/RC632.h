#ifndef RC632_H
#define RC632_H

#include "../../public/public.h"

/*******************************************/
#define RC632_STA_SOP1        0x01
#define RC632_STA_SOP2        0x02
#define RC632_STA_LEN1        0x03
#define RC632_STA_LEN2        0x04
#define RC632_STA_NC           0x05
#define RC632_STA_CMD         0x06
#define RC632_STA_DAT          0x07
#define RC632_STA_FCS          0x08

/*******************************************/
#define RC632_RPC_SOP     0
#define RC632_RPC_LEN      2
#define RC632_RPC_NC        4
#define RC632_RPC_CMD     6
#define RC632_RPC_DAT      8
/*******************************************/
#define RC632_FRAME_SOP                    0xBBAA
#define RC632_FRAME_SOPL                 LO_UINT16(RC632_FRAME_SOP)
#define RC632_FRAME_SOPH                 HI_UINT16(RC632_FRAME_SOP)

#define RC632_CMD_SET_BAUDRATE      0x0101
#define RC632_CMD_GET_HARDMODEL   0x0104
#define RC632_CMD_CONFIG_ISOTYPE    0x0108
#define RC632_CMD_ANT_CONTROL        0x010C

#define RC632_CMD_REQUEST_A               0x0201
#define RC632_CMD_ANTICOLL                  0x0202
#define RC632_CMD_SELECT                     0x0203
#define RC632_CMD_SLEEP                       0x0204
#define RC632_CMD_AUTHENTICATION       0x0207
#define RC632_CMD_M1READ                    0x0208
#define RC632_CMD_M1WRITE                   0x0209
#define RC632_CMD_M1INITVAL                 0x020A
#define RC632_CMD_M1READVAL              0x020B
#define RC632_CMD_M1DECREMENT        0x020C
#define RC632_CMD_M1INCREMENT         0x020D
#define RC632_CMD_M1RESTORE             0x020E
#define RC632_CMD_M1TRANSFER           0x020F
#define RC632_CMD_TYPEA_RST              0x0210
#define RC632_CMD_TYPEA_COS              0x0211
#define RC632_CMD_ULTRA_ANTICOLL      0x0212
#define RC632_CMD_ULTRA_WRITE           0x0213

#define RC632_CMD_TYPEB_RST               0x0301
#define RC632_CMD_HLTB                         0x0302
#define RC632_CMD_CL_DESELECT            0x0303

#define RC632_CMD_RF020CHECK      0x0401
#define RC632_CMD_RF020READ       0x0402
#define RC632_CMD_RF020WRITE      0x0403
#define RC632_CMD_RF020LOCK       0x0404
#define RC632_CMD_RF020COUNT      0x0405
#define RC632_CMD_RF020DESELECT   0x0406

#define RC632_CMD_SELECT_SR       0x0501
#define RC632_CMD_COMPLETION_SR   0x0502
#define RC632_CMD_READ_SR176      0x0503
#define RC632_CMD_WRITE_SR176     0x0504
#define RC632_CMD_PROTECT_SR176   0x0505
#define RC632_CMD_READ_SR4K       0x0506
#define RC632_CMD_WRITE_SR4K      0x0507
#define RC632_CMD_AUTH_SR4K       0x0508
#define RC632_CMD_GETUID_SR4K     0x0509
#define RC632_CMD_PROTECT_SR4K    0x050A

#define RC632_CMD_ISO15693_INVENTORY16               0x1000
#define RC632_CMD_ISO15693_INVENTORY                 0x1001
#define RC632_CMD_ISO15693_STAY_QUIET                0x1002
#define RC632_CMD_ISO15693_SELECT                    0x1003
#define RC632_CMD_ISO15693_RESET_TO_READY            0x1004
#define RC632_CMD_ISO15693_READ_SM                   0x1005
#define RC632_CMD_ISO15693_WRITE_SM                  0x1006
#define RC632_CMD_ISO15693_LOCK_BLOCK                0x1007
#define RC632_CMD_ISO15693_WRITE_AFI                 0x1008
#define RC632_CMD_ISO15693_LOCK_AFI                  0x1009
#define RC632_CMD_ISO15693_WRITE_DSFID               0x100A
#define RC632_CMD_ISO15693_LOCK_DSFID                0x100B
#define RC632_CMD_ISO15693_GET_SYSINFO               0x100C
#define RC632_CMD_ISO15693_GET_MULTIBLOCK_SECURITY   0x100D


/*******************************************************
 * 和上位机的通讯错误代码定义
*******************************************************/
#define  RC632_OK 0                          //请求操作成功
#define  RC632_FAULT255  255                 //帧校验失败
#define  RC632_FAULT254  254                //  错误

#define  RC632_FAULT10   10                  //通用错误
#define  RC632_FAULT11   11                  //不支持该命令
#define  RC632_FAULT12   12                  //命令参数错
#define  RC632_FAULT13   13                  //无卡
#define  RC632_FAULT14   14                  //射频基站损坏

#define  RC632_FAULT20   20                  //寻卡失败
#define  RC632_FAULT21   21                  //卡复位失败
#define  RC632_FAULT22   22                  //密码验证失败
#define  RC632_FAULT23   23                  //读卡失败
#define  RC632_FAULT24   24                  //写卡失败
/*******************************************************/

extern uint8 RC632_CalcFrameFCS(uint8 *msg_ptr, uint8 len);
extern uint8 RC632_AnalysisFrame(uint8 *frame);
extern uint16 RC632_BuildReqFrame(uint8 *pRet, uint16 cmd, uint8 *data, uint16 dataLen);

#endif // RC632_H
