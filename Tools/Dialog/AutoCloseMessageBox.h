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
        connect(timer, SIGNAL(timeout()), this, SLOT(autoClose()));
        connect(this, SIGNAL(finished(int)), timer, SLOT(stop()));
        setFont(FontFactory::dialogFont());
    }

    void showAndClose(int seconds, QString title, QString text) {
        setWindowTitle(title);
        setText(text);
        show();
        timer->start(seconds * 1000);
    }

private:
    QTimer* timer = new QTimer(this);

public slots:
    void autoClose() {
        timer->stop();
        if (this->isVisible()) {
            close();
        }
    }

};

#endif // AUTOCLOSEMESSAGEBOX_H
