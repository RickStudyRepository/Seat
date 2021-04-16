#ifndef CONSTVALUE_H
#define CONSTVALUE_H

#include "MacroDefinition.h"

using namespace MacroDefinition;

namespace ConstValue {
    // 预约状态
    const std::string UsedSeat = "已履约";
    const std::string UnusedSeat = "已失约";
    const std::string UsingSeat = "履约中";
    const std::string NotSigned = "未签到";

    // 字体大小
    // 标题字体大小
    const int TitleFontSize = 15;
    // 表格表头字体大小
    const int TableHeaderFontSize = 13;
    // 表格内容字体大小
    const int TableContentFontSize = 12;
    // 对话框字体大小
    const int DialogFontSize = 13;
    // 座位号字体大小
    const int SeatNumFontSize = 12;
    // 其他说明性信息字体大小
    const int DescribeFontSize = 13;

    // 超级管理员密码
    const std::string password = "123456";

    // 操作界面自动返回首页的时间
    const int maxDuration = 120;

    // 读卡或写卡最大重试次数
    const unsigned short maxTryTimeOnCard = 10;
    // RFID卡片的默认密码
    const unsigned char rfidPassword[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    // RFID默认操作的扇区号
    const uint32 sectorNum = 0;
    // RFID默认操作的块号
    const uint32 blockNum = 1;
}

#endif // CONSTVALUE_H
