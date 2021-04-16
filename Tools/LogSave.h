#ifndef LOGSAVE_H
#define LOGSAVE_H

#include <QString>
#include <QFile>
#include <QTextStream>
#include "Tools.h"

class LogSave {
public:
    LogSave() : filename(Tools::getCurrentDatetime() + ".log") {

    }

    void writeLog(QString log) {
        QFile logFile(filename);
        // 以写入文件的方式打开文件，并让文件自行处理换行符的问题
        logFile.open(QFile::WriteOnly | QFile::Text);
        QTextStream out(&logFile);
        out.setCodec("utf-8");
        out << log;
        logFile.close();
    }

private:
    // 文件名
    const QString filename;
};

#endif // LOGSAVE_H
