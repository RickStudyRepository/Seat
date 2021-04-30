#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QString>
#include "../Database/Database.h"
#include "../Tools/AliasName.h"

class tcp_server : public QObject
{
    Q_OBJECT
public:
    explicit tcp_server(QObject *parent = NULL);

private slots:
    // 建立一个新的连接
    void connectNewClient();
    // 读取客户端发送来的请求
    void getClientRequest();
    // 在客户端断开连接时，作出相应的响应
    void dealUnconnectWithClient();
    // 向客户端发送请求的数据
    void sendDataToClient(QTcpSocket*, QString);
    // 处理客户端的请求
    int dealRequest(QTcpSocket*, QString);

private:
    QTcpServer mServer;
    QTcpSocket *mSocket;

    // 初始化服务器监听的端口和IP
    void initServer();

    // 操作数据库的类
    Database* database;
    // 获取数据库实例
    void initDatabase();

    /**
     * @brief constructNewAppointment, 基于客户端发来的数据创建一个预约条目的结构体
     * @param studentNum: QString, 预约人学号
     * @param seatNum: int, 被预约的座位号
     * @param timeScopeString: QString, 预约的时间段，不包含日期
     * @param status: std::string, 本条预约的状态
     * @return 新构建出的预约信息结构体
     */
    AliasName::Appointment constructNewAppointment(QString studentNum, QString seatNumString, QString timeScopeString);

    /**
     * @brief getSeatInfos, 获取所有的座位信息
     * @param success: bool*, 用于说明是否获取成功
     * @return QString, 获取的所有座位信息，这里简化了获取结果，仅返回当前是否有可用时间
     *         例如："1 true 2 false"
     */
    QString getSeatInfos(bool* success);

    /**
     * @brief getAllAppointmentsOf, 获取相应学号的学生的所有预约记录，并返回一个指定格式的字符串
     * @param studentNum: QString, 学号
     * @param success: bool*, 标志数据库操作是否成功
     * @return QString, 基于预约记录构造出来的预约记录结果
     */
    QString getAllAppointmentsOf(QString studentNum, bool* success);

signals:
    // 日志信号
    void logSignal(QString);
};

#endif // TCP_SERVER_H
