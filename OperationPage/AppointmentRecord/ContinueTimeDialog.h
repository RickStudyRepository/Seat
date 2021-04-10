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

class ContinueTimeDialog : public QDialog
{
    Q_OBJECT

public:
    ContinueTimeDialog(QWidget* parent = NULL);

    // 展示续约对话框
    void showContinueDialog(AliasName::TimeScope timeScope, AliasName::TimeScopes availableTimes);

private:
    // 当前选定的时间范围
    AliasName::TimeScope currentTimeScope;
    // 所有的可用时间
    AliasName::TimeScopes availableTimes;
    // 当前时间点是否有可用时间段
    bool available;
    // 标记用户是否选择了时间
    bool isSelected;
    // 整体布局
    QGridLayout* layout = new QGridLayout(this);
    // 开始时间下拉列表
    QLabel* startTimeLabel = new QLabel(tr("开始时间"), this);
    QComboBox* startTime = new QComboBox(this);
    // 结束时间下拉列表
    QLabel* endTimeLabel = new QLabel(tr("结束时间"), this);
    QComboBox* endTime = new QComboBox(this);
    // 确认按钮
    QPushButton* okButton = new QPushButton(tr("确认"));
    // 取消按钮
    QPushButton* cancelButton = new QPushButton(tr("取消"));

    // 初始化按钮
    void initButton();
    // 初始化布局
    void initLayout();
    // 设置开始时间
    void resetStartTime();
    // 动态调成结束时间下拉列表
    void resetEndTime();

private slots:
    // 检查选定的时间，合法则发送时间范围
    void confirmTime();
    // 取消选定时间
    void cancel();

signals:
    // 若选择了一个合法的时间段
    void sendTimeScope(AliasName::TimeScope timeScope);
    // 没有选择一个时间段
    void notSelectedTime();
};

#endif // CONTINUETIMEDIALOG_H
