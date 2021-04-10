#include "ContinueTimeDialog.h"
#include <QDebug>
#include <QMessageBox>

ContinueTimeDialog::ContinueTimeDialog(QWidget* parent)
    : QDialog(parent)
{
    // 设置对话框大小
    setMinimumSize(290, 130);
    // 设置为只能当前窗口活动
    setWindowModality(Qt::ApplicationModal);
    initButton();
    initLayout();
    // 直接关闭窗口也要尝试发送未选择时间段的信号
    connect(this, SIGNAL(finished(int)), this, SLOT(cancel()));
}

void ContinueTimeDialog::initButton() {
    connect(okButton, SIGNAL(released()), this, SLOT(confirmTime()));
    connect(cancelButton, SIGNAL(released()), this, SLOT(cancel()));
}

void ContinueTimeDialog::initLayout() {
    layout->addWidget(startTimeLabel, 0, 0, 1, 2);
    layout->addWidget(startTime, 0, 2, 1, 2);
    layout->addWidget(endTimeLabel, 1, 0, 1, 2);
    layout->addWidget(endTime, 1, 2, 1, 2);
    layout->addWidget(okButton, 2, 0, 1, 2);
    layout->addWidget(cancelButton, 2, 2, 1, 2);
    setLayout(layout);
}

void ContinueTimeDialog::showContinueDialog(AliasName::TimeScope timeScope, AliasName::TimeScopes availableTimes) {
    // 重置当前选定的时间范围和可用时间范围
    currentTimeScope = timeScope;
    this->availableTimes = availableTimes;

    // 重置选择标记
    available = false;
    isSelected = false;

    resetStartTime();
    resetEndTime();
    show();
}

void ContinueTimeDialog::resetStartTime() {
    startTime->clear();
    startTime->addItem(Tools::intToTimeString(currentTimeScope.first));
    // 不让用户修改
    startTime->setEnabled(false);
}

void ContinueTimeDialog::resetEndTime() {
    endTime->clear();
    size_t size = this->availableTimes.size();
    for (size_t i = 0; i < size; i++) {
        if (availableTimes[i].first == currentTimeScope.second) {
            // 找到一个可用的连续时间段
            available = true;
            int start = currentTimeScope.second + 1;
            int end = availableTimes[i].second;
            while(start <= end) {
                endTime->addItem(Tools::intToTimeString(start));
                start += 1;
            }
            break;
        }
    }
    if (available == false) {
        endTime->addItem(tr("无可用时间"));
        endTime->setEnabled(false);
    }
}

void ContinueTimeDialog::confirmTime() {
    bool valid = false;
    int end = Tools::timeStringToInt(endTime->currentText());
    size_t size = availableTimes.size();
    for (size_t i = 0; i < size; i++) {
        // 选择的结束时间在当前预约之后的连续空闲时间段中
        if (currentTimeScope.second == availableTimes[i].first && end <= availableTimes[i].second) {
            valid = true;
            break;
        }
    }
    // 若时间合法，则发送时间范围
    if (valid == true) {
        // 标记选中了时间段
        isSelected = true;
        // 发送时间范围
        emit sendTimeScope(AliasName::TimeScope(currentTimeScope.first, end));
        qDebug() << "Send continue time scope";
        close();
    }
    else {
        QMessageBox::warning(this, tr("非法时间段"), tr("请选择一个连续的时间段！"));
    }
}

void ContinueTimeDialog::cancel() {
    // 若没有选择时间段，则发送未选择时间段的信号
    if (isSelected == false) {
        emit notSelectedTime();
    }
    // 如果对话框可见，则关闭对话框
    if (this->isVisible()) {
        close();
    }
}
