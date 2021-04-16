#include "AbstractTimeSelectionDialog.h"
#include "../FontFactory.h"

AbstractTimeSelectionDialog::AbstractTimeSelectionDialog(QWidget *parent)
    : QDialog(parent)
{
    initDialog();
    // 初始化开始结束时间标签
    initTimeLabels();
    // 初始化开始结束时间下拉列表
    initTimeComboBoxes();
    // 绑定按钮槽函数
    initButtons();
    // 初始化布局
    initLayout();
    // 初始化消息提示框
    initAutoCloseMessageBox();
}

void AbstractTimeSelectionDialog::initDialog() {
    // 设置对话框大小
    setMinimumSize(290, 130);
    // 设置为只能当前窗口活动
    setWindowModality(Qt::ApplicationModal);
    // 设置字体
    setFont(FontFactory::dialogFont());
    // 若关闭窗口，则发送未选择时间段信号
    connect(this, SIGNAL(finished(int)), this, SLOT(cancel()));
}

void AbstractTimeSelectionDialog::initTimeLabels() {
    startTimeLabel = new QLabel(tr("开始时间"), this);
    endTimeLabel = new QLabel(tr("结束时间"), this);
}

void AbstractTimeSelectionDialog::initTimeComboBoxes() {
    startTime = new QComboBox(this);
    endTime = new QComboBox(this);
}

void AbstractTimeSelectionDialog::initButtons() {
    // 确认按钮
    okButton = new QPushButton(tr("确认"));
    connect(okButton, SIGNAL(released()), this, SLOT(verifyTimeScope()));

    // 取消按钮
    cancelButton = new QPushButton(tr("取消"));
    connect(cancelButton, SIGNAL(released()), this, SLOT(cancel()));
}

void AbstractTimeSelectionDialog::initLayout() {
    layout = new QGridLayout(this);
    layout->addWidget(startTimeLabel, 0, 0, 1, 2);
    layout->addWidget(startTime, 0, 2, 1, 2);
    layout->addWidget(endTimeLabel, 1, 0, 1, 2);
    layout->addWidget(endTime, 1, 2, 1, 2);
    layout->addWidget(okButton, 2, 0, 1, 2);
    layout->addWidget(cancelButton, 2, 2, 1, 2);
    setLayout(layout);
}

void AbstractTimeSelectionDialog::initAutoCloseMessageBox() {
    autoCloseMsgBox = new AutoCloseMessageBox(this);
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
    emit logSignal(tr("抽象的时间选择对话框：设置下拉列表并显示"));
}

void AbstractTimeSelectionDialog::verifyTimeScope() {
    bool valid = false;
    int start = Tools::timeStringToInt(startTime->currentText());
    int end = Tools::timeStringToInt(endTime->currentText());
    size_t size = this->availableTimes.size();
    for (size_t i = 0; i < size; i++) {
        // 检验选取的时间段是否是某一可用时间段的子区间
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
        emit logSignal(
                    tr("抽象的时间选择对话框：发送选择的时间段：") +
                    QString::number(start) + "-" +
                    QString::number(end)
        );
        close();
    }
    else {
        emit logSignal(tr("抽象的时间选择对话框：选择了非法的时间段，停止发送时间段"));
        autoCloseMsgBox->showAndClose(5, tr("非法时间段"), tr("请选择一个连续的时间段！"));
    }
}

void AbstractTimeSelectionDialog::cancel() {
    // 若没有选择时间段，则发送未选择时间段的信号
    if (isSelected == false) {
        emit logSignal(tr("抽象的时间选择对话框：取消选择时间段"));
        emit notSelectedTime();
    }
    // 若对话框可见则关闭对话框
    if (this->isVisible()) {
        close();
    }
}
