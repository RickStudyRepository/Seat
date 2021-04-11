#include "AbstractTimeSelectionDialog.h"
#include "../FontFactory.h"

AbstractTimeSelectionDialog::AbstractTimeSelectionDialog(QWidget *parent)
    : QDialog(parent)
{
    // 设置对话框大小
    setMinimumSize(290, 130);
    // 设置为只能当前窗口活动
    setWindowModality(Qt::ApplicationModal);
    // 绑定按钮槽函数
    initButton();
    // 初始化布局
    initLayout();
    // 初始化消息提示框
    initAutoCloseMessageBox();
    // 设置字体
    setFont(FontFactory::dialogFont());
    // 若关闭窗口，则发送未选择时间段信号
    connect(this, SIGNAL(finished(int)), this, SLOT(cancel()));
}

void AbstractTimeSelectionDialog::initButton() {
    connect(okButton, SIGNAL(released()), this, SLOT(verifyTimeScope()));
    connect(cancelButton, SIGNAL(released()), this, SLOT(cancel()));
}

void AbstractTimeSelectionDialog::initLayout() {
    layout->addWidget(startTimeLabel, 0, 0, 1, 2);
    layout->addWidget(startTime, 0, 2, 1, 2);
    layout->addWidget(endTimeLabel, 1, 0, 1, 2);
    layout->addWidget(endTime, 1, 2, 1, 2);
    layout->addWidget(okButton, 2, 0, 1, 2);
    layout->addWidget(cancelButton, 2, 2, 1, 2);
    setLayout(layout);
}

void AbstractTimeSelectionDialog::initAutoCloseMessageBox() {
    // 设置为警告提示
    autoCloseMsgBox->setIcon(QMessageBox::Warning);
    autoCloseMsgBox->setDefaultButton(QMessageBox::Ok);
}

void AbstractTimeSelectionDialog::setTimeScopeAndShow(AliasName::TimeScopes availableTimes) {
    // 重置选择标记
    isSelected = false;
    isSelected = false;
    available = false;

    // 重置可用时间
    this->availableTimes = availableTimes;

    // 重置开始结束下拉框
    resetStartTime();
    resetEndTime();
    dealUnavailable();

    // 显示
    show();
}

void AbstractTimeSelectionDialog::verifyTimeScope() {
    bool valid = false;
    int start = Tools::timeStringToInt(startTime->currentText());
    int end = Tools::timeStringToInt(endTime->currentText());
    size_t size = this->availableTimes.size();
    for (size_t i = 0; i < size; i++) {
        // 检验选取的时间段是否是某一可用时间段的子区间
        //            if (start < end && start >= this->availableTimes[i].first && end <= this->availableTimes[i].second) {
        if (isValid(AliasName::TimeScope(start, end), availableTimes[i]) == true) {
            valid = true;
            break;
        }
    }
    // 若时间合法，则发送时间范围
    if (valid == true) {
        // 标记选中了时间段
        isSelected = true;
        // 发送时间范围
        emit sendTimeScope(AliasName::TimeScope(start, end));
        qDebug() << "Send time scope:" << start << "," << end;
        close();
    }
    else {
        autoCloseMsgBox->showAndClose(5, tr("非法时间段"), tr("请选择一个连续的时间段！"));
    }
}

void AbstractTimeSelectionDialog::cancel() {
    // 若没有选择时间段，则发送未选择时间段的信号
    if (isSelected == false) {
        emit notSelectedTime();
    }
    // 若对话框可见则关闭对话框
    if (this->isVisible()) {
        close();
    }
}
