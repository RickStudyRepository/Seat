#ifndef TOOLS_H
#define TOOLS_H

#include <QString>

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
};

#endif // TOOLS_H
