#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QIcon>
#include <QSize>
#include <QLabel>
#include <QString>
#include <QPixmap>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QMouseEvent>
#include <QPushButton>
#include <QLineEdit>
#include "Tools/ClickedableLabel.h"
#include "HomePage/HomePage.h"
#include "OperationPage/OperationPage.h"

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private:
    // 窗口标题
    const QString appName = tr("Seat");
    // 窗口图标
    const QIcon appIcon = QIcon(QString(":/images/Seat.ico"));
    // 窗口大小
    const QSize fixedSize = QSize(800, 480);
    // 整体布局
    QVBoxLayout* layout = new QVBoxLayout(this);
    void initLayout();

    // 首页
    HomePage* homePage = new HomePage(this);
    // 初始化首页
    void initHomePage();

    // 操作界面
    OperationPage* operationPage = new OperationPage(this);
    // 初始化操作界面
    void initOperationPage();

private slots:
    // 切换到操作界面
    void gotoOperationPage();
    // 返回到首页界面
    void returnHomePage();
};

#endif // MAINWINDOW_H
