#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QWidget>
#include <QDebug>
#include "AdminLabel.h"

class HomePage : public QWidget {
    Q_OBJECT
private:
    AdminLabel* label = new AdminLabel(this);

public:
    HomePage(QWidget* parent = NULL)
        : QWidget(parent)
    {
        label->setPixmap(QPixmap(QString(":/images/HomePage.jpg")));
        connect(label, SIGNAL(otherPlaceRelease()), this, SLOT(gotoOperationPage()));
        connect(label, SIGNAL(leftUpCornerRelease()), this, SLOT(gotoAdminPage()));
    }

    void gotoBack() {
        hide();
    }

    void gotoFront() {
        show();
    }

signals:
    // 前往操作界面
    void gotoOperationPageSignal();
    // 前往超级管理员界面
    void gotoAdminPageSignal();

private slots:
    void gotoOperationPage() {
        emit gotoOperationPageSignal();
    }

    void gotoAdminPage() {
        qDebug() << tr("前往超级管理员界面");
        emit gotoAdminPageSignal();
    }
};

#endif // HOMEPAGE_H
