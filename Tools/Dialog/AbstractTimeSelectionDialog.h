#ifndef ABSTRACTTIMESELECTIONDIALOG_H
#define ABSTRACTTIMESELECTIONDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QWidget>
#include <QComboBox>
#include <QPushButton>
#include <QGridLayout>
#include <QMessageBox>
#include <QDebug>
#include "../Tools.h"
#include "../AliasName.h"
#include "AutoCloseMessageBox.h"

class AbstractTimeSelectionDialog : public QDialog
{
    Q_OBJECT

public:
    AbstractTimeSelectionDialog(QWidget* parent = NULL);

    // 设置时间范围，实现对话框的复用
    void setTimeScopeAndShow(AliasName::TimeScopes availableTimes);

protected:
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

private:
    // 自动关闭的消息提示框
    AutoCloseMessageBox* autoCloseMsgBox = new AutoCloseMessageBox(this);
    void initAutoCloseMessageBox();

    // 初始化按钮
    void initButton();
    // 初始化布局
    void initLayout();

    // 重置开始时间下拉列表
    virtual void resetStartTime() = 0;
    // 处理无可用时间的情况
    virtual void dealUnavailable() = 0;

    // 判断结果是否合法的方法
    virtual bool isValid(AliasName::TimeScope selected, AliasName::TimeScope compare) = 0;

private slots:
    // 重置结束时间下拉列表
    virtual void resetEndTime() = 0;
    // 检查选定的时间，合法则发送时间范围
    void verifyTimeScope();
    // 取消选定时间
    void cancel();

signals:
    // 若选择了一个合法的时间段
    void sendTimeScope(AliasName::TimeScope timeScope);
    // 没有选择一个时间段
    void notSelectedTime();
};

#endif // ABSTRACTTIMESELECTIONDIALOG_H
