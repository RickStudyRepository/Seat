#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QString>
#include "Database/Database.h"

class tcp_server : public QObject
{
    Q_OBJECT
public:
    explicit tcp_server(QObject *parent = NULL);
    void init_server(int);
    int solve_data(QTcpSocket*,QString);

public slots:
    void new_client();
    void read_client_data();
    void client_dis();
    void data_back(QTcpSocket*,QString);

private:
    QTcpServer mServer;
    QTcpSocket *mSocket;

    // request key
    const QString LOGIN = "login";
    const QString RESERVE = "reserve";
    const QString CANCEL = "cancel";
    const QString STATE = "state";
    const QString RECORD = "record";

    // database
    Database* database;
    void initDatabase();

    // get seat info
    QString getSeatInfos(bool* success);

    // get all appointments
    QString getAllAppointmentsOf(QString id, bool* success);
};

#endif // TCP_SERVER_H
