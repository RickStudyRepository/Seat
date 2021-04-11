#ifndef TOOLS_H
#define TOOLS_H

#include <QString>
#include <cstdlib>
#include "AliasName.h"

class Tools {
public:
    /**
     * @brief intToTimeString, 16 --> "16:00"
     * @param time: int, 整点小时值
     * @return QString, "{time}:00"
     */
    static QString intToTimeString(int time) {
        return QString::number(time) + QString(":00");
    }

    /**
     * @brief timeStringToInt, "16:00" --> 16
     * @param time: QString, 整点时间字符串
     * @return int, 整点小时值
     */
    static int timeStringToInt(QString time) {
        return time.split(":").at(0).toInt();
    }
    /**
     * @brief databaseTimeToTimeScope, 从数据库中保存的字符串提取出整数时间范围
     * @param time: std::string, 格式为：`YYYY-MM-DD HH:MM-HH:MM`
     * @return TimeScope, 提取出的是HH对应的一对整数
     */
    static AliasName::TimeScope databaseTimeToTimeScope(std::string time) {
        int start = Tools::timeStringToInt(QString::fromStdString(time.substr(11, 2)));
        int end = Tools::timeStringToInt(QString::fromStdString(time.substr(17, 2)));
        return AliasName::TimeScope(start, end);
    }

    /**
     * @brief stringToInt, 将一个std::string类型的字符串转换为一个int整数，
     *                     因为目标机不支持C++11，故没有使用stoi
     * @param str: std::string, 一个整数字符串
     * @return 整数字符串对应的整数值
     */
    static int stringToInt(std::string str) {
        return atoi(str.c_str());
    }
};

#endif // TOOLS_H
