#ifndef MACRODEFINITION_H
#define MACRODEFINITION_H

namespace MacroDefinition {
    // 从public.h中移出的宏定义
    #define uint8   unsigned char
    #define uint16  unsigned short
    #define uint32  unsigned int
    #define int8    signed char
    #define int16   signed short
    #define int32   signed int

    /* takes a byte out of a uint32 : var - uint32,  ByteNum - byte to take out (0 - 3) */
    // 从一个无符号整型数中拆分出一个字节，ByteNum用于指示拆分出的是第几个字节
    #define BREAK_UINT32(var, ByteNum)  (uint8)((uint32)(((var) >>((ByteNum) * 8)) & 0x00FF))

    // 拆分16位无符号整数的低八位和高八位，不影响原始数据
    #define HI_UINT16(a) (((a) >> 8) & 0xFF)
    #define LO_UINT16(a) ((a) & 0xFF)
    // 拆分8位无符号整数的低四位和高四位，不影响原始数据
    #define HI_UINT8(a) (((a) >> 4) & 0x0F)
    #define LO_UINT8(a) ((a) & 0x0F)

    // 使用几个数构造出一个32位的无符号整数
    #define BUILD_UINT32(Byte0, Byte1, Byte2, Byte3) \
              ((uint32)((uint32)((Byte0) & 0x00FF) \
              + ((uint32)((Byte1) & 0x00FF) << 8) \
              + ((uint32)((Byte2) & 0x00FF) << 16) \
              + ((uint32)((Byte3) & 0x00FF) << 24)))

    // 使用几个数构造出一个16位的无符号整数
    #define BUILD_UINT16(loByte, hiByte)    ((uint16)(((loByte) & 0x00FF) + (((hiByte) & 0x00FF) << 8)))
    // 使用几个数构造出一个8位的无符号整数
    #define BUILD_UINT8(hiByte, loByte)   ((uint8)(((loByte) & 0x0F) + (((hiByte) & 0x0F) << 4)))

    #define TIMEOUT_ACTUATOR    3000

    #define ON   1
    #define OFF  0


    // 从tty.h中移出的宏定义
    #define TTY_ON    0x01
    #define TTY_OFF   0x00

    #define MAX_COM_NUM  2
    #define COM1_ID    0
    #define COM2_ID    1
    // 这里指明串口名称，如果使用USB转串口，需要设置为相应的USB设备名
    #define COM1 "/dev/ttyUSB0"
    #define COM2 "/dev/ttyS3"


    // 从RfidThread中移出的宏定义
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
}

#endif // MACRODEFINITION_H
