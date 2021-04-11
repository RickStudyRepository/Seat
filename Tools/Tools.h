#ifndef TOOLS_H
#define TOOLS_H

#include <QString>
#include <QDate>
#include <cstdlib>
#include <vector>
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
     * @brief getCurrentDate, 获取当前日期的"yyyy-MM-dd"表示字符串
     * @return
     */
    static std::string getCurrentDate() {
        return QDate::currentDate().toString(QString("yyyy-MM-dd")).toStdString();
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

    /**
     * @brief stringSplit, 将输入字符串str根据splitter进行拆分，不会产生空串
     * @param splitter: char, 分隔符
     * @param str: std::string, 待分隔的字符串
     * @return 分隔出的字符串数组
     */
    static std::vector<std::string> stringSplit(std::string str, char splitter = ',') {
        std::vector<std::string> result;
        std::string temp;
        // 搜索出的最新的分隔符位置
        size_t splitterIndex = 0;
        // 下一个子串的起始位置
        size_t startIndex = 0;
        do {
            // 从下一子串开始的位置查找
            splitterIndex = str.find(splitter, startIndex);
            if (splitterIndex != str.npos) {
                // 获取被分割出的字符串
                temp = str.substr(startIndex, splitterIndex - startIndex);
                // 不保留空串
                if (temp != "") {
                    result.push_back(temp);
                }
                // 更新下一子串的起始位置为本次分隔符位置的下一个字符
                startIndex = splitterIndex + 1;
            }
        } while (splitterIndex != str.npos);
        // 检查最后一个分隔符后剩余的子串
        temp = str.substr(startIndex);
        if (temp != "") {
            result.push_back(temp);
        }
        return result;
    }
};

#endif // TOOLS_H
