#include "TimeSelectionDialog.h"
#include <QMessageBox>
#include <QTime>
#include <QDebug>

TimeSelectionDialog::TimeSelectionDialog(QWidget* parent)
    : QDialog(parent)
{
    // 设置对话框大小
    setMinimumSize(290, 130);
    // 设置为只能当前窗口活动
    setWindowModality(Qt::ApplicationModal);
    // 绑定按钮槽函数
    initButton();
    // 初始化下拉列表
    initComboBox();
    // 初始化布局
    initLayout();
    // 若关闭窗口，则发送未选择时间段信号
    connect(this, SIGNAL(finished(int)), this, SLOT(cancel()));
}

void TimeSelectionDialog::initButton() {
    connect(okButton, SIGNAL(released()), this, SLOT(confirmTime()));
    connect(cancelButton, SIGNAL(released()), this, SLOT(cancel()));
}

void TimeSelectionDialog::initComboBox() {
    // 选择了开始时间之后，动态调整结束时间，避免选择非法时间
    connect(startTime, SIGNAL(currentIndexChanged(int)), this, SLOT(resetEndTime()));
}

void TimeSelectionDialog::initLayout() {
    layout->addWidget(startTimeLabel, 0, 0, 1, 2);
    layout->addWidget(startTime, 0, 2, 1, 2);
    layout->addWidget(endTimeLabel, 1, 0, 1, 2);
    layout->addWidget(endTime, 1, 2, 1, 2);
    layout->addWidget(okButton, 2, 0, 1, 2);
    layout->addWidget(cancelButton, 2, 2, 1, 2);
    setLayout(layout);
}

void TimeSelectionDialog::setTimeScope(AliasName::AvailableTimes availableTimes) {
    // 重置是否选择了时间段的标记
    isSelected = false;
    // 重置当前时间点是否有可用时间
    available = false;
    // 重置可用时间
    this->availableTimes = availableTimes;

    // 获取当前时间的小时值
    int currentHourInt = QTime::currentTime().toString("HH").toInt();

    // 首先清空下拉列表内容
    startTime->clear();
    endTime->clear();

    // 临时存储可用起始时间
    int start;
    int end;
    // 向开始时间下拉列表中添加可用时间
    size_t size = this->availableTimes.size();
    for (size_t i = 0; i < size; i++) {
        start = this->availableTimes[i].first;
        end = this->availableTimes[i].second;
        while (start < end) {
            // 只有当时间在当前时间或当前时间之后才能添加到下拉列表中
            if (start >= currentHourInt) {
                available = true;
                startTime->addItem(Tools::intToTimeString(start));
            }
            start += 1;
        }
    }
    if (available == false) {
        startTime->addItem(QString(tr("无可用时间")));
        startTime->setEnabled(false);
        endTime->addItem(QString(tr("无可用时间")));
        endTime->setEnabled(false);
        // 无可用时间时禁用确认按钮
        okButton->setEnabled(false);
    }
    else {
        // 设置结束时间
        resetEndTime();
    }
}

void TimeSelectionDialog::resetEndTime() {
    // 仅当有可用时间时更新
    if (available == false) {
        return;
    }
    int start;
    int end;
    // 获取选中的开始时间
    int selectedStartTime = Tools::timeStringToInt(startTime->currentText());
    size_t size = this->availableTimes.size();
    for (size_t i = 0; i < size; i++) {
        start = this->availableTimes[i].first;
        end = this->availableTimes[i].second;
        // 查找包含选中的开始时间的时间段
        if (selectedStartTime >= start && selectedStartTime < end) {
            // 定位到开始时间之后
            start = selectedStartTime + 1;
            // 清空结束时间下拉列表
            endTime->clear();
            // 设置结束时间下拉列表
            while (start <= end) {
                endTime->addItem(Tools::intToTimeString(start));
                start += 1;
            }
            // 设置完毕，直接退出循环
            break;
        }
    }
}

void TimeSelectionDialog::confirmTime() {
    bool valid = false;
    int start = Tools::timeStringToInt(startTime->currentText());
    int end = Tools::timeStringToInt(endTime->currentText());
    size_t size = this->availableTimes.size();
    for (size_t i = 0; i < size; i++) {
        // 检验选取的时间段是否是某一可用时间段的子区间
        if (start < end && start >= this->availableTimes[i].first && end <= this->availableTimes[i].second) {
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
        qDebug() << "Send time scope";
        close();
    }
    else {
        QMessageBox::warning(this, tr("非法时间段"), tr("请选择一个连续的时间段！"));
    }
}

void TimeSelectionDialog::cancel() {
    // 若没有选择时间段，则发送未选择时间段的信号
    if (isSelected == false) {
        emit notSelectedTime();
    }
    // 如果对话框可见，则关闭对话框
    if (this->isVisible()) {
        close();
    }
}
