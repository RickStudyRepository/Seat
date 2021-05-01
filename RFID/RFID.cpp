#include "RFID.h"
#include <QDebug>
//#include "../Tools/MacroDefinition.h"
#include "../Tools/ConstValue.h"
#include "../Tools/Tools.h"

using namespace MacroDefinition;

RFID* RFID::singleRFID = NULL;

RFID::RFID(QWidget* parent)
    : QWidget(parent)
{
    // 这个用于打开串口
    rfidThread = new RfidThread();

    // 将初始化标志设置false
    isInited = false;
}

RFID*& RFID::getRFID(QWidget *parent) {
    if (RFID::singleRFID == NULL) {
        RFID::singleRFID = new RFID(parent);
        return RFID::singleRFID;
    }
    else {
        return RFID::singleRFID;
    }
}

RFID::~RFID() {
    delete rfidThread;
    delete RFID::singleRFID;
}

bool RFID::initRFID() {
    // 基于内部实现，初始化协议的等待的时间是0.1s
    if (RC632_OK != rfidProtocol.RFID_Set14443Type()) {
        emit logSignal(tr("RFID：初始化协议为14443失败"));
        return false;
    }
    emit logSignal(tr("RFID：初始化协议为14443成功"));
    // 若设置成功，则将初始化标志置为true
    isInited = true;
    return true;
}

bool RFID::selectCard() {
    // 检查协议是否已初始化
    if (this->isInited == false) {
        if (this->initRFID() == false) {
            emit logSignal(tr("RFID：协议初始化失败，停止选中卡片"));
            return false;
        }
    }
    if (RC632_OK != rfidProtocol.RFID_InventoryALL()) {
        emit logSignal(tr("RFID：选中卡片失败"));
        return false;
    }
    QString tagIdString;
    // 将卡片ID转换为字符串
    Tools::HexToQString(tagIdString, rfidProtocol.tagId, 4);
    emit logSignal(tr("RFID：选中卡片成功，卡片ID：") + tagIdString);
    return true;
}

bool RFID::writeStudentNum(QString studentNum) {
    // 选中一张卡片
    if (this->selectCard() == false) {
        emit logSignal(tr("RFID：寻卡失败，停止初始化卡片信息"));
        return false;
    }
    // 写入数据时需要传入按照HexToQString转换得到的16进制字符串转换成的十六进制数
    unsigned int uintStudentNum = studentNum.toUInt();
    // 将无符号整数按字节转换为16进制字符串
    QString hexString;
    Tools::HexToQString(hexString, (unsigned char*)(&uintStudentNum), 4);

    uint8 result = rfidProtocol.RFID_M1Init(
                ConstValue::sectorNum,
                ConstValue::blockNum,
                // 将字符串转换为无符号整型数，即：unsigned int
                hexString.toUInt(0, 16),
                // 使用const_cast去除const特性，但是如果使用转换后的指针操作写相应区域是未定义的行为
                // 不过在本实例中，只会读rfidPassword指向的内存，并没有写
                const_cast<unsigned char*>(ConstValue::rfidPassword)
    );
    // 向卡内写入学号
    if (RC632_OK != result) {
        emit logSignal(tr("RFID：写入信息失败"));
        return false;
    }
    emit logSignal(tr("RFID：写入信息成功，写入信息为：") + studentNum);
    return true;
}

bool RFID::readStudentNum(QString &studentNum) {
    // 选中一张卡片
    if (this->selectCard() == false) {
        emit logSignal(tr("RFID：寻卡失败，停止读取学号"));
        return false;
    }
    uint8 result = rfidProtocol.RFID_M1Balance(
                ConstValue::sectorNum,
                ConstValue::blockNum,
                // 使用const_cast去除const特性，但是如果使用转换后的指针操作相应区域是未定义的行为
                // 不过在本实例中，只会读rfidPassword指向的内存，并没有写
                const_cast<unsigned char*>(ConstValue::rfidPassword)
    );
    // 向卡内写入
    if (RC632_OK != result) {
        emit logSignal(tr("RFID：读取卡内学号失败"));
        return false;
    }
    studentNum = QString::number(rfidProtocol.balance);
    emit logSignal(tr("RFID：读取卡片内学号成功，学号为") + studentNum);
    return true;
}
