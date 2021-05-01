#ifndef TOOLS_H
#define TOOLS_H

#include <QString>
#include <QDate>
#include <QDateTime>
#include <QStringList>
#include <cstdlib>
#include <vector>
#include "AliasName.h"
#include "MacroDefinition.h"

using namespace MacroDefinition;

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
     * @brief getCurrentDate, 获取当前日期的"yyyy-MM-dd"字符串
     * @return
     */
    static std::string getCurrentDate() {
        return QDate::currentDate().toString(QString("yyyy-MM-dd")).toStdString();
    }

    /**
     * @brief getCurrentDatetime，获取当前的日期时间的"yyyy-MM-dd HH:mm:ss"字符串
     * @return QString, "yyyy-MM-dd HH:mm:ss"格式的日期字符串
     */
    static QString getCurrentDatetime() {
        return QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
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

    // 以下方法从示例程序public.h文件引入
    /**
     * 将十六进制的8位无符号整数转换为十六进制的字符串
     * @param id: QString&, 转换结果将会保存在这里
     * @param buf: uint8*, 注意到前面的宏定义，这里实际上是将字符数组当作8位无符号整数数组使用了
     * @param len: int, 原十六进制数字的长度
     */
    static void HexToQString(QString &id, uint8* buf, int len) {
        id.clear();
        for(int i=0; i<len; i++) {
            /**
             * %1类似于printf的%d参数，格式化字符串
             * arg方法的传入参数将替换格式化字符串中的%1
             * 这里arg方法的涵义是将一个8位无符号整数转换为字符串，
             * 这个字符串的长度是2，右对齐，因为宽度参数是正数，
             * 传入的整数是16进制的，空余的位置使用第四个参数指明的字符填充
             */
            id = id.append(QString("%1").arg(buf[i], 2, 16, QLatin1Char('0')).toUpper());
        }
    }

    /**
     * 将十六进制的字符串转换为十六进制的8位无符号整数
     * @param id: QString&, 转换结果将会保存在这里
     * @param buf: uint8*, 注意到前面的宏定义，这里实际上是将字符数组当作8位无符号整数数组使用了
     * @param len: int, 原十六进制数字的长度
     * @return 返回存储十六进制数组的长度
     */
    static uint8 QStringToHex(QString &id, uint8* buf, uint8 len) {
        int strLen = id.length();
        if((strLen + 1) / 2 < len) {
            len = (strLen + 1) / 2;
        }
        for(uint8 i = 0; i < len; i++) {
            /**
             * mid方法是返回一个从`i<<1(也就是2 * i)`开始长度为2的子字符串，
             * 这里使用2 * i的原因可能是十六进制数转换为字符串的时候每一位的前面都多一个0
             * toInt将该子字符串转化为一个16进制的整数，
             * 若转换时有异常发生，将会把第一个参数指向的一个bool变量置为false，
             * 这里传入了0，即Q_NULLPTR，也就是不关心转换结果
             */
            buf[i] = id.mid(i << 1, 2).toInt(0, 16);
        }
        return len;
    }
};

#endif // TOOLS_H
