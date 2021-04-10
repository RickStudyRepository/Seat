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
    OperationPage/OperationPage.cpp \
    OperationPage/MakeAppointment/MakeAppointment.cpp \
    OperationPage/MakeAppointment/TimeSelectionDialog.cpp \
    OperationPage/AppointmentRecord/AppointmentRecord.cpp \
    OperationPage/AppointmentRecord/ContinueTimeDialog.cpp \
    Tools/Dialog/AbstractTimeSelectionDialog.cpp

HEADERS += \
    MainWindow.h \
    Tools/ClickedableLabel.h \
    Tools/Tools.h \
    HomePage/HomePage.h \
    OperationPage/OperationPage.h \
    OperationPage/HeadWidget/CountDown.h \
    OperationPage/HeadWidget/HeadWidget.h \
    OperationPage/MakeAppointment/MakeAppointment.h \
    OperationPage/MakeAppointment/SeatWidget.h \
    OperationPage/MakeAppointment/TimeSelectionDialog.h \
    Tools/AliasName.h \
    OperationPage/AppointmentRecord/OperationAndStatus.h \
    OperationPage/AppointmentRecord/AppointmentRecord.h \
    OperationPage/AppointmentRecord/ContinueTimeDialog.h \
    Tools/ConstValue.h \
    Tools/Dialog/AbstractTimeSelectionDialog.h \
    Tools/Dialog/AutoCloseMessageBox.h \
    Tools/Dialog/ConfirmDialog.h

RESOURCES += \
    images.qrc
