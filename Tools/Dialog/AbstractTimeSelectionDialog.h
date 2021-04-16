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

// 把这些属性设置为protected是为了让具体的子类实现可以访问
protected:
    // 所有的可用时间
    AliasName::TimeScopes availableTimes;
    // 当前时间点是否有可用时间段
    bool available;
    // 标记用户是否选择了时间
    bool isSelected;
    // 整体布局
    QGridLayout* layout;
    // 开始时间下拉列表
    QLabel* startTimeLabel;
    QComboBox* startTime;
    // 结束时间下拉列表
    QLabel* endTimeLabel;
    QComboBox* endTime;
    // 确认按钮
    QPushButton* okButton;
    // 取消按钮
    QPushButton* cancelButton;

private:
    // 自动关闭的消息提示框
    AutoCloseMessageBox* autoCloseMsgBox;
    void initAutoCloseMessageBox();

    // 初始化开始结束时间标签
    void initTimeLabels();
    // 初始化开始结束时间下拉列表
    void initTimeComboBoxes();
    // 初始化按钮
    void initButtons();
    // 初始化布局
    void initLayout();
    // 初始化对话框风格
    void initDialog();

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
    // 日志信息信号
    void logSignal(QString);
};

#endif // ABSTRACTTIMESELECTIONDIALOG_H
