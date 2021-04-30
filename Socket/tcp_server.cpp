#include "tcp_server.h"
#include "../Tools/Tools.h"
#include "../Tools/ConstValue.h"

tcp_server::tcp_server(QObject *parent) : QObject(parent) {
    connect(&mServer, SIGNAL(newConnection()), this, SLOT(connectNewClient()));
    initServer();
    initDatabase();
}

void tcp_server::initDatabase() {
    // 数据库采用了单例模式，这里获取数据库实例的指针
    database = Database::getSingleDatabase();
}

// 初始化并开始监听
void tcp_server::initServer() {
    if(mServer.isListening()){
        qDebug() << "Socket: has Listening";
        emit logSignal(tr("Socket: 正在监听"));
    }
    else {
        if(mServer.listen(QHostAddress::AnyIPv4, ConstValue::port)) {
            qDebug() << "Socket: listen port: " << ConstValue::port << "successfully!";
            emit logSignal(tr("Socket: 成功监听端口") + QString::number(ConstValue::port));
        }
    }
}

// 建立一个新的链接
void tcp_server::connectNewClient() {
    qDebug() << "Socket: enter new client";
    emit logSignal(tr("Socket: 开始一个新的客户端连接"));
    // 获取一个Socket连接
    mSocket = mServer.nextPendingConnection();
    qDebug() << "Socket: get a new connection from"
             << mSocket->peerAddress().toString();
    emit logSignal(tr("Socket: 客户端IP地址：") + mSocket->peerAddress().toString());
    // 监听从客户端发来的数据
    connect(mSocket, SIGNAL(readyRead()), this, SLOT(getClientRequest()));
    // 在与客户端断开连接之后进行适当的处理
    connect(mSocket, SIGNAL(disconnected()), this, SLOT(dealUnconnectWithClient()));
}

// 获取客户端的请求
void tcp_server::getClientRequest() {
     // 从所有的Socket读取数据
     QTcpSocket *obj = (QTcpSocket*)sender();
     QString msg = obj->readAll();
     qDebug() << "Socket: server request:" << msg;
     emit logSignal(tr("Socket: 收到客户端的请求：") + msg);
     dealRequest(obj, msg);
}

// 处理客户端的请求
int tcp_server::dealRequest(QTcpSocket *obj, QString msg) {
    emit logSignal(tr("Socket: 开始处理客户端请求"));
    // 操作字符串列表
    // 第一个维度表示操作类型，后续维度是需要的数据
    QStringList list = msg.split(" ");
    if(list.length() == 0) {
        return -1;
    }
    // 发送给客户端的数据
    QString back_data = "";

    // 登录
    if(ConstValue::Login == list[0]) {
        QString studentNum = list[1];
        QString password = list[2];
        emit logSignal(tr("Socket: 客户端请求登录，学号为：") + studentNum);

        // 判断用户是否存在
        bool success = false;
        bool result = database->isStudentExists(studentNum.toStdString(), &success);
        if (success == true) {
            // 登陆成功
            if (result == true) {
                qDebug() << "Socket: Student login successfully!";
                emit logSignal(tr("Socket: 客户端登录成功"));
                back_data.append("200");
            }
            // 学号不存在，返回400
            else {
                qDebug() << "Socket: Student not exists!";
                emit logSignal(tr("Socket: 学生不存在，登录失败"));
                back_data.append("400");
            }
        }
        // 操作数据库失败，返回400
        else {
            qDebug() << "Socket: database failed in login";
            emit logSignal(tr("Socket: 操作数据库失败，登陆失败"));
            back_data.append("400");
        }
    }
    // 进行预约
    else if(ConstValue::MakeAppointment == list[0]) {
        emit logSignal(tr("Socket: 客户端请求构造一个新的预约"));
        // 构造预约信息结构体
        AliasName::Appointment newAppointment = constructNewAppointment(list[1], list[3], list[2]);
        // 写入数据库
        bool success = database->makeNewAppoinment(newAppointment);
        // 向数据库插入预约记录成功
        if (success == true) {
            emit logSignal(tr("Socket: 写入预约记录成功"));
            back_data.append("200");
        }
        // 向数据库插入预约记录失败
        else {
            emit logSignal(tr("Socket: 写入预约记录失败"));
            back_data.append("400");
        }
    }
    // 获取所有座位的当前可用状态
    else if(ConstValue::GetSeatsState == list[0]) {
        qDebug() << "Socket: STATE.......";
        emit logSignal(tr("Socket: 客户端请求获取所有座位当前的可用状态"));
        // 获取所有座位的状态是否成功的标志
        bool success = false;
        // 获取结果字符串
        QString result = getSeatInfos(&success);
        // 获取所有座位当前可用状态成功
        if (success == true) {
            emit logSignal(tr("Socket: 获取所有座位当前的可用状态成功"));
            back_data.append("200 ");
            back_data.append(result);
        }
        // 获取所有座位当前可用状态失败
        else {
            emit logSignal(tr("Socket: 获取所有座位当前的可用状态失败"));
            back_data.append("400");
        }
    }
    // 取消预约，移动客户端未实现，不可能进入这个分支
    else if(ConstValue::CancelAppointment == list[0]) {
        // 待取消的预约号
        QString appointmentId = list[1];
        emit logSignal(tr("Socket: 客户端请求取消预约号为") + QString::number(appointmentId) + tr("的预约"));

        // 取消结果
        bool result = false;
        result = database->cancelAppointment(appointmentId.toInt());

        // 成功取消预约
        if (result == true) {
            emit logSignal(tr("Socket: 客户端取消预约成功"));
            back_data.append("200");
        }
        // 取消预约失败
        else {
            emit logSignal(tr("Socket: 客户端取消预约失败"));
            back_data.append("400");
        }
    }
    // 续约，移动客户端未实现，不可能进入这个分支
    else if (ConstValue::ContinueAppointment == list[0]) {
        qDebug() << "Socket: continue appointment!";
        emit logSignal(tr("Socket: 客户端请求续约"));
    }
    // 获取相应学号的学生的所有预约记录
    else if (ConstValue::GetRecords == list[0]) {
        QString studentNum = list[1];
        emit logSignal(tr("Socket: 客户端请求获取学号为：") + studentNum + tr("学生的所有预约记录"));
        bool success = false;
        QString result = getAllAppointmentsOf(studentNum, &success);
        if (success == true) {
            emit logSignal(tr("Socket: 获取预约记录成功"));
            back_data.append(result);
        }
        else {
            emit logSignal(tr("Socket: 获取预约记录失败"));
            back_data.append("400");
        }
    }
    // 没有有效的请求信息
    else {
        emit logSignal(tr("Socket: 未检测到有效的请求信息"));
        back_data.append("none data!");
    }
    // 向客户端发送其请求的数据，或者相应的请求是否成功的信息
    sendDataToClient(obj, back_data);
}

// 向客户端发送数据
void tcp_server::sendDataToClient(QTcpSocket *obj, QString msg) {
    // 解析发回的数据并发送
    qDebug() << "Socket: back data is" << msg;
    emit logSignal(tr("Socket: 发送给客户端请求的数据：") + msg);
    // 转编码
    obj->write(msg.toUtf8());
    obj->flush();
}

// 在与客户端断开连接时做出一定的响应
void tcp_server::dealUnconnectWithClient() {
    // 掉线对象
    QTcpSocket *obj = (QTcpSocket*)sender();
    // 打印出掉线对象的ip
    qDebug() << "Socket: disconnected with ip:" << obj->peerAddress().toString();
    emit logSignal(tr("Socket: 与IP地址为：") + obj->peerAddress().toString() + tr("的客户端断开连接"));
}

// 构造新的预约结构体
AliasName::Appointment tcp_server::constructNewAppointment(QString studentNum, QString seatNumString, QString timeScopeString) {
    // 因为客户端的预约时间段精确到了分钟，而服务器这边简化为了整点，所以进行一下转换
    timeScopeString[3] = '0';
    timeScopeString[4] = '0';
    timeScopeString[9] = '0';
    timeScopeString[10] = '0';
    return AliasName::Appointment(
                studentNum.toStdString(),
                seatNumString.toInt(),
                // 构造包含日期的时间
                Tools::getCurrentDate() + " " + timeScopeString.toStdString(),
                ConstValue::UsingSeat);
}

// 获取所有座位当前的可用状态
QString tcp_server::getSeatInfos(bool* success) {
    AliasName::SeatInfos seatInfos = database->getAllSeatsInfo(success);
    QString seatInfosString = "";
    if (*success == false) {
        *success = false;
    }
    else {
        size_t size = seatInfos.size();
        for (size_t i = 0; i < size; i++) {
            seatInfosString += QString::number(seatInfos[i].num);
            seatInfosString += " ";
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

// 获取指定学号的同学的所有预约记录
QString tcp_server::getAllAppointmentsOf(QString studentNum, bool* success) {
    AliasName::Appointments appointments = database->getAllAppointmentsOf(studentNum.toStdString(), success);
    QString result = "";
    if (*success == false) {
        *success = false;
    }
    // 构造预约记录字符串
    else {
        size_t size = appointments.size();
        for (size_t i = 0; i < size; i++) {
            // 添加座位号
            result += QString::number(appointments[i].seatNum);
            result += " ";
            // 添加时间段
            result += QString::fromStdString(appointments[i].time);
            result += " ";
            // 添加预约状态
            result += QString::fromStdString(appointments[i].status);
            if (i < size - 1) {
                result += " ";
            }
        }
        *success = true;
    }
    return result;
}
