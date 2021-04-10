#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QWidget>
#include "Tools/ClickedableLabel.h"

class HomePage : public QWidget {
    Q_OBJECT
private:
    ClickedableLabel* label = new ClickedableLabel(this);

public:
    HomePage(QWidget* parent = NULL)
        : QWidget(parent)
    {
        label->setPixmap(QPixmap(QString(":/images/HomePage.jpg")));
        connect(label, SIGNAL(leftButtonRelease()), this, SLOT(gotoOperationPage()));
    }

    void gotoBack() {
        hide();
    }

    void gotoFront() {
        show();
    }

signals:
    void gotoOperationPageSignal();

private slots:
    void gotoOperationPage() {
        emit gotoOperationPageSignal();
    }
};

#endif // HOMEPAGE_H
