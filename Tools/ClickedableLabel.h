#ifndef MYQLABEL_H
#define MYQLABEL_H

#include <QLabel>
#include <QWidget>
#include <QMouseEvent>

class MyQLabel : public QLabel
{
    Q_OBJECT

public:
    MyQLabel(QWidget* parent = nullptr)
        : QLabel(parent)
    {

    }

private:
    // 用于捕获鼠标释放的信号
    void mouseReleaseEvent(QMouseEvent* mouseEvent) {
        if (mouseEvent->button() == Qt::LeftButton) {
            // 鼠标左键释放信号
            emit leftButtonRelease();
        }
    }

signals:
    // 鼠标左键释放的信号
    void leftButtonRelease();
};

#endif // MYQLABEL_H
