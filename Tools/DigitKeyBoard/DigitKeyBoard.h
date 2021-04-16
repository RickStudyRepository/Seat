#ifndef DIGITKEYBOARD_H
#define DIGITKEYBOARD_H

#include <QWidget>
#include <QString>
#include <QPushButton>
#include <QSignalMapper>
#include <QLineEdit>
#include <QGridLayout>
#include <vector>

// 采用单例模式
class DigitKeyBoard : public QWidget {
    Q_OBJECT

public:
    // 获取单例变量
    static DigitKeyBoard*& getKeyBoard(QWidget* parent = NULL);
    // 定位输出文本框
    void startInput(QLineEdit* output);

    ~DigitKeyBoard();

private:
    DigitKeyBoard(QWidget* parent = NULL);
    // 单例变量
    static DigitKeyBoard* singleKeyBoard;

    // 输出到的文本框指针
    QLineEdit* output;
    // 初始化按钮
    std::vector<QPushButton*> numberButtons;
    // 回退按钮
    QPushButton* backspaceButton;
    // 确认按钮
    QPushButton* confirmButton;
    void initButtons();

    // 信号映射
    QSignalMapper* signalMapper;
    void initSignalMapper();

    // 布局
    QGridLayout* layout;
    void initLayout();

private slots:
    // 响应按键
    void actionButton(QString buttonText);

signals:
    void logSignal(QString);

};

#endif // DIGITKEYBOARD_H
