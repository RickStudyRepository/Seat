#ifndef TIMESELECTIONDIALOG_H
#define TIMESELECTIONDIALOG_H

#include <QWidget>
#include <QTime>
#include "../../Tools/Tools.h"
#include "../../Tools/AliasName.h"
#include "../../Tools/Dialog/AbstractTimeSelectionDialog.h"

class TimeSelectionDialog : public AbstractTimeSelectionDialog
{
    Q_OBJECT

public:
    TimeSelectionDialog(QWidget* parent = NULL);

private:
    // 初始化下拉列表
    void initComboBox();

    // 重置开始时间
    void resetStartTime();

    // 处理无可用时间的情况
    void dealUnavailable();

    bool isValid(AliasName::TimeScope selected, AliasName::TimeScope compare);

private slots:
    // 动态调成结束时间下拉列表
    void resetEndTime();
};

#endif // TIMESELECTIONDIALOG_H
