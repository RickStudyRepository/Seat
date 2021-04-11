#include "ContinueTimeDialog.h"

ContinueTimeDialog::ContinueTimeDialog(QWidget *parent)
    : AbstractTimeSelectionDialog(parent)
{
    setWindowTitle(tr("选择续约结束时间"));
}

void ContinueTimeDialog::setCurrentTimeScope(AliasName::TimeScope currentTimeScope) {
    this->currentTimeScope = currentTimeScope;
}

void ContinueTimeDialog::resetStartTime() {
    startTime->clear();
    startTime->addItem(Tools::intToTimeString(currentTimeScope.first));
    // 不让用户修改
    startTime->setEnabled(false);
}

void ContinueTimeDialog::dealUnavailable() {
    if (available == false) {
        endTime->addItem(tr("无可用时间"));
        endTime->setEnabled(false);
        okButton->setEnabled(false);
    }
    // 如果有可用时间，则进行复位
    else {
        endTime->setEnabled(true);
        okButton->setEnabled(true);
    }
}

void ContinueTimeDialog::resetEndTime() {
    endTime->clear();
    size_t size = this->availableTimes.size();
    for (size_t i = 0; i < size; i++) {
        if (availableTimes[i].first == currentTimeScope.second) {
            // 找到一个可用的连续时间段
            available = true;
            // 后移一位开始时间，略过原本的结束时间
            int start = currentTimeScope.second + 1;
            int end = availableTimes[i].second;
            while(start <= end) {
                endTime->addItem(Tools::intToTimeString(start));
                start += 1;
            }
            break;
        }
    }
}

bool ContinueTimeDialog::isValid(AliasName::TimeScope selected, AliasName::TimeScope compare) {
    return currentTimeScope.second == compare.first
            && selected.second <= compare.second;
}
