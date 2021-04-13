#ifndef CONSTVALUE_H
#define CONSTVALUE_H

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
}

#endif // CONSTVALUE_H
