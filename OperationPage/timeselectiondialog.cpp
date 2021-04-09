#include "timeselectiondialog.h"
#include "tools.h"
#include <QMessageBox>
#include <QTime>

TimeSelectionDialog::TimeSelectionDialog(QWidget* parent)
    : QDialog(parent)
{
    // 设置对话框大小
    setFixedSize(290, 130);
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

void TimeSelectionDialog::setTimeScope(AvailableTimes availableTimes) {
    // 获取当前时间的小时值
    int currentHourInt = QTime::currentTime().toString("HH").toInt();
    // 首先清空下拉列表内容
    startTime->clear();
    endTime->clear();
    // 重置可用时间
    this->availableTimes = availableTimes;
    // 当前时间点是否有可用时间
    available = false;
    // 临时存储可用起始时间
    int start;
    int end;
    // 向开始时间下拉列表中添加可用时间
    for (auto availableTime : availableTimes) {
        start = availableTime.first;
        end = availableTime.second;
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
        endTime->addItem(QString(tr("无可用时间")));
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
    for (auto availableTime : availableTimes) {
        start = availableTime.first;
        end = availableTime.second;
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
    for (auto availableTime : availableTimes) {
        // 检验选取的时间段是否是某一可用时间段的子区间
        if (start < end && start >= availableTime.first && end <= availableTime.second) {
            valid = true;
            break;
        }
    }
    // 若时间合法，则提示用户选择的时间
    if (valid == true) {
        QMessageBox::information(
             this,
             tr("选择成功"),
             tr("你选择了时间段：\n") +
             Tools::intToTimeString(start) +
             tr("-") +
             Tools::intToTimeString(end)
        );
        emit sendTimeScope(TimeScope(start, end));
        close();
    }
    else {
        QMessageBox::warning(this, tr("非法时间段"), tr("请选择一个连续的时间段！"));
    }
}

void TimeSelectionDialog::cancel() {
    emit notSelectedTime();
    if (this->isVisible()) {
        close();
    }
}
