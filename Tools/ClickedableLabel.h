#ifndef CLICKEDABLELABEL_H
#define CLICKEDABLELABEL_H

#include <QLabel>
#include <QWidget>
#include <QMouseEvent>

class ClickedableLabel : public QLabel
{
    Q_OBJECT

public:
    ClickedableLabel(QWidget* parent = nullptr)
        : QLabel(parent)
    {

    }

private:
    // 用于捕获鼠标释放的信号
    void mouseReleaseEvent(QMouseEvent* mouseEvent) {
        // 鼠标左键释放
        if (mouseEvent->button() == Qt::LeftButton) {
            emit leftButtonRelease();
        }
        // 鼠标右键释放
        if (mouseEvent->button() == Qt::RightButton) {
            emit rightButtonRelease();
        }
    }

signals:
    // 鼠标左键释放的信号
    void leftButtonRelease();
    // 鼠标右键释放的信号
    void rightButtonRelease();
};

#endif // CLICKEDABLELABEL_H
