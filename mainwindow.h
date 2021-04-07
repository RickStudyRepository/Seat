#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QString>
#include <QPixmap>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QPushButton>
#include <QLineEdit>

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    // 临时使用鼠标左键释放事件作为从首页切换到操作界面的触发条件
    void mouseReleaseEvent(QMouseEvent* mouseEvent);

private:
    // 整体布局
    QVBoxLayout* layout = new QVBoxLayout();
    // 标志是否处于操作界面，默认不处于
    bool state = false;
    // 首页
    QLabel* homePage = new QLabel();
    // 初始化首页
    void initHomePage();

    // 操作界面
    QWidget* operationPage = new QWidget();
    // 操作界面布局
    QVBoxLayout* operationPageLayout = new QVBoxLayout();

    // 操作界面顶部布局
    QHBoxLayout* headLayout = new QHBoxLayout();
    // 学号标签
    QLabel* studentNumLabel = new QLabel();
    QString studentNumString = tr("学号：");
    // 学号文本框
    QString studentNum = tr("181110305");
    // 返回按钮
    QPushButton* returnButton = new QPushButton(tr("返回"));
    void initOperationPageHead();

    // 具体的操作
    // 进行预约
    QWidget* makeAppointment = new QWidget();
    QString makeAppointmentString = tr("开始预约");
    void initMakeAppointment();
    // 历史记录
    QWidget* appointmentRecord = new QWidget();
    QString appointmentRecordString = tr("我的预约");
    void initAppointmentRecord();

    // 容纳具体操作的Tab
    QTabWidget* operationTab = new QTabWidget();
    void initOperationTab();

    // 初始化操作界面
    void initOperationPage();

private slots:
    // 切换到操作界面
    void gotoOperationPage();
    // 返回到首页界面
    void returnHomePage();
};

#endif // MAINWINDOW_H
