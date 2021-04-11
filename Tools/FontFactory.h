#ifndef FONTFACTORY_H
#define FONTFACTORY_H

#include <QFont>
#include "ConstValue.h"

class FontFactory {
public:
    // 构造表头字体
    static QFont tableHeaderFont() {
        QFont font;
        font.setPointSize(ConstValue::TableHeaderFontSize);
        return font;
    }

    // 构造表格内容字体
    static QFont tableContentFont() {
        QFont font;
        font.setPointSize(ConstValue::TableContentFontSize);
        return font;
    }

    // 表格操作或状态字体
    static QFont tableOperationAndStatusFont(std::string status) {
        QFont font;
        font.setPointSize(ConstValue::TableContentFontSize);
        // 已失约或已履约都加粗强调一下
        if (status == ConstValue::UnusedSeat || status == ConstValue::UsedSeat) {
            font.setBold(true);
        }
        return font;
    }

    // 构造对话框字体
    static QFont dialogFont() {
        QFont font;
        font.setPointSize(ConstValue::DialogFontSize);
        return font;
    }

    // 座位号字体
    static QFont seatNumFont() {
        QFont font;
        font.setPointSize(ConstValue::SeatNumFontSize);
        return font;
    }

    // 构造其他说明性信息字体
    static QFont describeFont() {
        QFont font;
        font.setPointSize(ConstValue::DescribeFontSize);
        return font;
    }

};

#endif // FONTFACTORY_H
