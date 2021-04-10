#ifndef OPERATIONANDSTATUS_H
#define OPERATIONANDSTATUS_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QString>
#include <QFrame>
#include <string>
#include "../../Tools/ConstValue.h"
#include "../../Tools/FontFactory.h"

class OperationAndStatus : public QWidget {
    Q_OBJECT
private:
    // 在表格中的行号
    int rowNum;
    // 布局
    QHBoxLayout* layout = new QHBoxLayout(this);
    // 操作标签
    QLabel* cancelLabel = NULL;
    QLabel* continueLabel = NULL;
    // 操作标签之间的竖直线
    QFrame* verticalLine = new QFrame(this);
    // 状态标签
    QLabel* statusLabel = NULL;

    // 初始化布局，根据预约状态适配内容
    void initLayout(std::string status) {
        // 履约中或未签到，要放入两个可点击的标签
        if (status == ConstValue::UsingSeat || status == ConstValue::NotSigned) {
            cancelLabel = new QLabel(tr("<a href = '#'>取消预约</a>"), this);
            connect(cancelLabel, SIGNAL(linkActivated(QString)), this, SLOT(emitCancelSignal()));
            continueLabel = new QLabel(tr("<a href = '#'>续约</a>"), this);
            connect(continueLabel, SIGNAL(linkActivated(QString)), this, SLOT(emitContinueSignal()));
            // 操作标签之间的竖直线
            verticalLine->setFrameShape(QFrame::VLine);
            verticalLine->setLineWidth(2);

            layout->addWidget(cancelLabel);
            layout->addWidget(verticalLine);
            layout->addWidget(continueLabel);
        }
        // 已履约或已失约，则直接放入状态对应的文本
        else {
            statusLabel = new QLabel(tr(status.c_str()), this);
            layout->addWidget(statusLabel);
        }
        layout->setSpacing(5);
        layout->setAlignment(Qt::AlignCenter);
        setLayout(layout);
    }

public:
    OperationAndStatus(int rowNum, std::string status, QWidget* parent = NULL)
        : QWidget(parent)
    {
        this->rowNum = rowNum;
        initLayout(status);
        setFont(FontFactory::tableContentFont());
    }

    // 在用户通过操作进行了修改之后，变更操作或状态中的内容
    void resetOperationAndStatus(std::string status) {
        // 如果用户确认了取消预约，那么状态会变更为已履约，也就是：usedSeat
        if (status == ConstValue::UsedSeat) {
            if (cancelLabel == NULL || continueLabel == NULL) {
                return;
            }
            // 从布局中移除操作标签
            layout->removeWidget(cancelLabel);
            layout->removeWidget(verticalLine);
            layout->removeWidget(continueLabel);
            cancelLabel->deleteLater();
            continueLabel->deleteLater();
            verticalLine->deleteLater();
            // 置零，避免野指针
            cancelLabel = NULL;
            verticalLine = NULL;
            continueLabel = NULL;
            // 设置状态标签
            statusLabel = new QLabel(tr(status.c_str()), this);
            layout->addWidget(statusLabel);
        }
    }

signals:
    // 取消预约信号
    void cancelSignal(int);
    // 续约信号
    void continueSignal(int);

private slots:
    void emitCancelSignal() {
        // 取消某一个预约
        emit cancelSignal(rowNum);
    }

    void emitContinueSignal() {
        emit continueSignal(rowNum);
    }
};

#endif // OPERATIONANDSTATUS_H
