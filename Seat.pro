#-------------------------------------------------
#
# Project created by QtCreator 2021-04-07T14:42:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Seat
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    main.cpp \
    MainWindow.cpp \
    Tools/Dialog/AbstractTimeSelectionDialog.cpp \
    Tools/DigitKeyBoard/DigitKeyBoard.cpp \
    AdminPage/AdminPage.cpp \
    OperationPage/OperationPage.cpp \
    OperationPage/MakeAppointment/MakeAppointment.cpp \
    OperationPage/MakeAppointment/TimeSelectionDialog.cpp \
    OperationPage/AppointmentRecord/AppointmentRecord.cpp \
    OperationPage/AppointmentRecord/ContinueTimeDialog.cpp \
    Database/Database.cpp \
    RFID/HF13Point56M/ISO14443/ISO14443.cpp \
    RFID/HF13Point56M/RC632/RC632.cpp \
    RFID/RFIDThread/RfidThread.cpp \
    RFID/TTY/tty.cpp \
    RFID/RFID.cpp

HEADERS += \
    MainWindow.h \
    Tools/AliasName.h \
    Tools/ClickedableLabel.h \
    Tools/ConstValue.h \
    Tools/FontFactory.h \
    Tools/MacroDefinition.h \
    Tools/Tools.h \
    Tools/Dialog/AbstractTimeSelectionDialog.h \
    Tools/Dialog/AutoCloseMessageBox.h \
    Tools/Dialog/ConfirmDialog.h \
    Tools/Dialog/InputDialog.h \
    Tools/DigitKeyBoard/DigitKeyBoard.h \
    AdminPage/AdminPage.h \
    HomePage/HomePage.h \
    HomePage/AdminLabel.h \
    OperationPage/OperationPage.h \
    OperationPage/HeadWidget/CountDown.h \
    OperationPage/HeadWidget/HeadWidget.h \
    OperationPage/MakeAppointment/MakeAppointment.h \
    OperationPage/MakeAppointment/SeatWidget.h \
    OperationPage/MakeAppointment/TimeSelectionDialog.h \
    OperationPage/AppointmentRecord/OperationAndStatus.h \
    OperationPage/AppointmentRecord/AppointmentRecord.h \
    OperationPage/AppointmentRecord/ContinueTimeDialog.h \
    Database/Database.h \
    RFID/HF13Point56M/ISO14443/ISO14443.h \
    RFID/HF13Point56M/RC632/RC632.h \
    RFID/RFIDThread/RfidThread.h \
    RFID/TTY/tty.h \
    RFID/RFID.h \
    Tools/LogSave.h

RESOURCES += \
    images.qrc
