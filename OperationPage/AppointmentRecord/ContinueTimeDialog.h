#ifndef CONTINUETIMEDIALOG_H
#define CONTINUETIMEDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QWidget>
#include <QComboBox>
#include <QPushButton>
#include <QGridLayout>
#include "../../Tools/Tools.h"
#include "../../Tools/AliasName.h"
#include "../../Tools/Dialog/AbstractTimeSelectionDialog.h"

class ContinueTimeDialog : public AbstractTimeSelectionDialog
{
    Q_OBJECT

public:
    ContinueTimeDialog(QWidget* parent = NULL);

    // 设置当前的预约范围
    void setCurrentTimeScope(AliasName::TimeScope currentTimeScope);

private:
    // 当前选定的时间范围
    AliasName::TimeScope currentTimeScope;
    // 重置开始时间下拉列表
    void resetStartTime();

    void dealUnavailable();

    bool isValid(AliasName::TimeScope selected, AliasName::TimeScope compare);

private slots:
    // 重置结束时间下拉列表
    void resetEndTime();
};

#endif // CONTINUETIMEDIALOG_H
