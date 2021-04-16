#ifndef RFIDTHREAD_H
#define RFIDTHREAD_H

#include <QThread>
#include "../../Tools/Tools.h"
#include "../../Tools/MacroDefinition.h"

using namespace MacroDefinition;

class MainWidget;

class RfidThread : public QThread {
    Q_OBJECT
public:
    explicit RfidThread(QObject *parent = 0);

    void run();

signals:
    
public slots:

private:

public:
    int cpld_fd;
    bool runFlag;
    uint8 tagId[6];
};

#endif // RFIDTHREAD_H
