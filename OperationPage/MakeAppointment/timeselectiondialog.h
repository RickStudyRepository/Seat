#ifndef TIMESELECTIONDIALOG_H
#define TIMESELECTIONDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QWidget>
#include <QComboBox>
#include <QPushButton>
#include <QGridLayout>
#include "../../Tools/Tools.h"
#include "../../Tools/AliasName.h"

class TimeSelectionDialog : public QDialog
{
    Q_OBJECT

public:
    TimeSelectionDialog(QWidget* parent = nullptr);

    // 设置时间范围，实现对话框的复用
    void setTimeScope(AvailableTimes availableTimes);

private:
    // 所有的可用时间
    AvailableTimes availableTimes;
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
    // 初始化下拉列表
    void initComboBox();
    // 初始化布局
    void initLayout();
private slots:
    // 动态调成结束时间下拉列表
    void resetEndTime();
    // 检查选定的时间，合法则发送时间范围
    void confirmTime();
    // 取消选定时间
    void cancel();

signals:
    // 若选择了一个合法的时间段
    void sendTimeScope(TimeScope timeScope);
    // 没有选择一个时间段
    void notSelectedTime();
};

#endif // TIMESELECTIONDIALOG_H
