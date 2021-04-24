#include "tcp_server.h"
#include "../Tools/AliasName.h"
#include "../Tools/Tools.h"
#include "../Tools/ConstValue.h"

tcp_server::tcp_server(QObject *parent) : QObject(parent)
{
    connect(&mServer,SIGNAL(newConnection()),this,SLOT(new_client()));
    initDatabase();
}

void tcp_server::initDatabase() {
    database = Database::getSingleDatabase();
}

QString tcp_server::getSeatInfos(bool *success) {
    AliasName::SeatInfos seatInfos = database->getAllSeatsInfo(success);
    QString seatInfosString = "";
    if (*success == false) {
        *success = false;
    }
    else {
        //size_t size = seatInfos.size();
        size_t size = 2;
        for (size_t i = 0; i < size; i++) {
            seatInfosString += QString::number(seatInfos[i].num);
            seatInfosString += " ";
            qDebug() << seatInfos[i].availableTimes.size();
            if (seatInfos[i].hasAvailableTimeScope() == true) {
                seatInfosString += "true";
            }
            else {
                seatInfosString += "false";
            }
            if (i < size - 1) {
                seatInfosString += " ";
            }
        }
        *success = true;
    }
    return seatInfosString;
}

QString tcp_server::getAllAppointmentsOf(QString id, bool *success) {
    AliasName::Appointments appointments = database->getAllAppointmentsOf(id.toStdString(), success);
    QString result = "";
    if (*success == false) {
        *success = false;
    }
    // construct string result
    else {
        size_t size = appointments.size();
        for (size_t i = 0; i < size; i++) {
            result += QString::number(appointments[i].seatNum);
            result += " ";
            result += QString::fromStdString(appointments[i].time);
            result += " ";
            result += QString::fromStdString(appointments[i].status);
            if (i < size - 1) {
                result += " ";
            }
        }
        *success = true;
    }
    return result;
}

// init and start listening
void tcp_server::init_server(int port)
{
    if(mServer.isListening()){
        qDebug()<<"has Listening";
    }else{
        if( mServer.listen(QHostAddress::AnyIPv4,port)){
            qDebug()<< "listen "<<port << "successfully!";
        }
    }
}

// get a new connection
void tcp_server::new_client()
{
    qDebug()<<"enter new client";
    mSocket = mServer.nextPendingConnection();//get socket
    qDebug()<<"get a new connection from"
            << mSocket->peerAddress().toString();
    //listening data sending
    connect(mSocket,SIGNAL(readyRead()),this,SLOT(read_client_data()));
    //listening socket disconnect
    connect(mSocket,SIGNAL(disconnected()),this,SLOT(client_dis()));
}

void tcp_server::read_client_data()
{
     //read msg from all socket
     QTcpSocket *obj = (QTcpSocket*)sender();
     QString msg = obj->readAll();
     qDebug()<<"server getMsg:"<<msg;
     solve_data(obj,msg);

}

int tcp_server::solve_data(QTcpSocket *obj,QString msg)
{
    QStringList list = msg.split(" ");
    if(list.length() == 0)
    {
        return -1;
    }
    QString back_data = "";
    if(LOGIN == list[0])
    {
        QString id = list[1];
        QString psw = list[2];
        //op sql
        bool success = false;
        bool result = database->isStudentExists(id.toStdString(), &success);
        if (success == true) {
            if (result == true) {
                qDebug() << "Socket: Student login successfully!";
                back_data.append("200"); // login successfully
            }
            else {
                qDebug() << "Socket: Student not exists!";
                back_data.append("400");
            }
        }
        else {
            qDebug() << "Socket: database failed in login";
            back_data.append("400"); // login faild
        }
    }else if(RESERVE == list[0]){
        QString id = list[1];
        QString hour = list[2];
        hour[3] = '0';
        hour[4] = '0';
        hour[9] = '0';
        hour[10] = '0';
        QString time = QString::fromStdString(Tools::getCurrentDate()) + " " + hour;
        int seatNum = list[3].toInt();

        // reserve
        AliasName::Appointment newAppointment(
                    id.toStdString(),
                    seatNum,
                    time.toStdString(),
                    ConstValue::UsingSeat
        );

        bool success = database->makeNewAppoinment(newAppointment);
        if (success == true) {
            back_data.append("200"); // reserve successfully
        }
        else {
            back_data.append("400"); // reserve faild
        }
    }else if(STATE == list[0]){
        //op sql
        qDebug()<<"STATE.......";
        // get state successfully
        bool success = false;
        QString result = getSeatInfos(&success);
        if (success == true) {
            back_data.append("200 ");
            back_data.append(result);
        }
        // get state faild
        else {
            back_data.append("400");
        }
    }else if(CANCEL == list[0]){
        QString id = list[1];
        //op sql

        back_data.append("200"); // cancel successfully
        back_data.append("400"); // cancel faild
    }else if (RECORD == list[0]) {      // getRecord
        QString id = list[1];
        bool success = false;
        QString result = getAllAppointmentsOf(id, &success);
        if (success == true) {
            back_data.append(result);
        }
        else {
            back_data.append("400");
        }
    }else {
        back_data.append("none data!");
    }
    data_back(obj,back_data);
}

void tcp_server::data_back(QTcpSocket *obj, QString msg)
{
    // prase back msg and send
    qDebug() << "back data is" << msg;
    obj->write(msg.toUtf8());//转编码
    obj->flush();
}

void tcp_server::client_dis()
{
     QTcpSocket *obj = (QTcpSocket*)sender();//掉线对象
     qDebug()<<"dis"<<obj->peerAddress().toString();//打印出掉线对象的ip
}
