#ifndef AUTOCLOSEMESSAGEBOX_H
#define AUTOCLOSEMESSAGEBOX_H

#include <QWidget>
#include <QTimer>
#include <QCloseEvent>
#include <QMessageBox>
#include "../FontFactory.h"

class AutoCloseMessageBox : public QMessageBox {
    Q_OBJECT

public:
    AutoCloseMessageBox(QWidget* parent = NULL)
        : QMessageBox(parent) {
        // 计时器初始化
        timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(autoClose()));

        // 如果用户主动关闭了对话框，那么也要停止计时
        connect(this, SIGNAL(finished(int)), timer, SLOT(stop()));

        // 设置字体
        setFont(FontFactory::dialogFont());
    }

    void showAndClose(int seconds, QString title, QString text) {
        setWindowTitle(title);
        setText(text);
        show();
        timer->start(seconds * 1000);
    }

private:
    QTimer* timer;

public slots:
    void autoClose() {
        timer->stop();
        if (this->isVisible()) {
            close();
        }
    }

};

#endif // AUTOCLOSEMESSAGEBOX_H
