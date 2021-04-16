#ifndef SEATWIDGET_H
#define SEATWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QString>
#include <QChar>
#include <QVBoxLayout>
#include "../../Tools/ClickedableLabel.h"
#include "../../Tools/FontFactory.h"

class SeatWidget : public QWidget
{
    Q_OBJECT

public:
    SeatWidget(QPixmap picture, int num, QWidget* parent = NULL)
        : QWidget(parent)
    {
        // 座位编号
        this->num = num;

        photoLabel = new ClickedableLabel(this);
        // 固定座位图片大小
        photoLabel->setFixedSize(100, 100);
        photoLabel->setPixmap(picture.copy().scaled(photoLabel->size()));
        photoLabel->setFrameStyle(QFrame::Box);
        // 图片被左键单击之后发送自己的座位编号
        connect(photoLabel, SIGNAL(leftButtonRelease()), this, SLOT(selected()));

        numLabel = new QLabel(this);
        // 设置座位编号字符串："001", "020"等形式
        numLabel->setText(QString("%1").arg(num, 3, 10, QChar('0')));
        // 设置座位编号为水平垂直居中
        numLabel->setAlignment(Qt::AlignCenter);
        numLabel->setFont(FontFactory::seatNumFont());

        QVBoxLayout* layout = new QVBoxLayout(this);
        layout->addWidget(photoLabel);
        layout->addWidget(numLabel);
        layout->setMargin(0);
        layout->setSpacing(0);
        setLayout(layout);
        // 固定部件大小
        setFixedSize(100, 130);
    }

private:
    ClickedableLabel* photoLabel;
    QLabel* numLabel;
    int num;

signals:
    void sendSeatNum(int);
    // 日志信息信号
    void logSignal(QString);

private slots:
    void selected() {
        emit logSignal(tr("座位部件：被选中，座位号：") + QString::number(num));
        emit sendSeatNum(num);
    }
};

#endif // SEATWIDGET_H
