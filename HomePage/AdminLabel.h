#ifndef ADMINLABEL_H
#define ADMINLABEL_H

#include <QWidget>
#include <QMouseEvent>
#include <QLabel>

class AdminLabel : public QLabel {
    Q_OBJECT
private:
    void mouseReleaseEvent(QMouseEvent* mouseEvent) {
        if (mouseEvent->button() == Qt::LeftButton) {
            if (mouseEvent->pos().x() <= 100 && mouseEvent->pos().y() <= 100) {
                emit leftUpCornerRelease();
            }
            else {
                emit otherPlaceRelease();
            }
        }
    }

public:
    AdminLabel(QWidget* parent = NULL) : QLabel(parent) {}

signals:
    // 鼠标左键在界面左上角释放
    void leftUpCornerRelease();
    // 鼠标左键在界面其他位置释放
    void otherPlaceRelease();
};

#endif // ADMINLABEL_H
