#ifndef SEATWIDGET_H
#define SEATWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QString>
#include <QChar>
#include <QVBoxLayout>
#include "../../Tools/ClickedableLabel.h"

class SeatWidget : public QWidget
{
    Q_OBJECT

public:
    SeatWidget(QPixmap picture, int num, QWidget* parent = nullptr)
        : QWidget(parent)
    {
        // 座位编号
        this->num = num;
        // 固定座位图片大小
        photoLabel->setFixedSize(100, 100);
        photoLabel->setPixmap(picture.copy().scaled(photoLabel->size()));
        // 图片被左键单击之后发送自己的座位编号
        connect(photoLabel, SIGNAL(leftButtonRelease()), this, SLOT(selected()));
        // 设置座位编号字符串："001", "020"等形式
        numLabel->setText(QString("%1").arg(num, 3, 10, QChar('0')));
        // 设置座位编号为水平垂直居中
        numLabel->setAlignment(Qt::AlignCenter);
        QVBoxLayout* layout = new QVBoxLayout(this);
        layout->addWidget(photoLabel);
        layout->addWidget(numLabel);
        layout->setMargin(0);
        layout->setSpacing(5);
        setLayout(layout);
        // 固定部件大小
        setFixedSize(100, 130);
        // 设置边框，宽度为2，颜色是黑色
        setStyleSheet("border: 2px solid #000000;");
    }

private:
    ClickedableLabel* photoLabel = new ClickedableLabel(this);
    QLabel* numLabel = new QLabel(this);
    int num;

signals:
    void sendSeatNum(int);

private slots:
    void selected() {
        emit sendSeatNum(num);
    }
};

#endif // SEATWIDGET_H
