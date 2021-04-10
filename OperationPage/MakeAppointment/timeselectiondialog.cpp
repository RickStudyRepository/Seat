#include "TimeSelectionDialog.h"



TimeSelectionDialog::TimeSelectionDialog(QWidget *parent)
    : AbstractTimeSelectionDialog(parent)
{
    // 初始化下拉列表
    initComboBox();
}

void TimeSelectionDialog::initComboBox() {
    // 选择了开始时间之后，动态调整结束时间，避免选择非法时间
    connect(startTime, SIGNAL(currentIndexChanged(int)), this, SLOT(resetEndTime()));
}

void TimeSelectionDialog::resetStartTime() {
    // 获取当前时间的小时值
    int currentHourInt = QTime::currentTime().toString("HH").toInt();
    // 清空下拉列表
    startTime->clear();
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

void TimeSelectionDialog::dealUnavailable() {
    if (available == false) {
        startTime->addItem(QString(tr("无可用时间")));
        startTime->setEnabled(false);
        endTime->addItem(QString(tr("无可用时间")));
        endTime->setEnabled(false);
        // 无可用时间时禁用确认按钮
        okButton->setEnabled(false);
    }
}

bool TimeSelectionDialog::isValid(AliasName::TimeScope selected, AliasName::TimeScope compare) {
    return selected.first < selected.second
            && selected.first >= compare.first
            && selected.second <= compare.second;
}
