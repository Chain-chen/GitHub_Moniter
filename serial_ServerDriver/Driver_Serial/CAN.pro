#-------------------------------------------------
#
# Project created by QtCreator 2018-08-06T15:00:36
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = CAN
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += 3rdparty/Eigen

SOURCES += \
    3rdparty/qcustomplot/qcustomplot.cpp \
    driverparams.cpp \
    file.cpp \
    loopparam.cpp \
        main.cpp \
        mainwindow.cpp \
    canthread.cpp \
    canparamsetting.cpp \
    motormatch.cpp \
    userdefined.cpp \
    waveformwidget.cpp

HEADERS += \
    3rdparty/qcustomplot/qcustomplot.h \
    driverparams.h \
    file.h \
    loopparam.h \
        mainwindow.h \
    canthread.h \
    canparamsetting.h \
    motormatch.h \
    userdefined.h \
    waveformwidget.h

FORMS += \
    driverparams.ui \
    file.ui \
    loopparam.ui \
        mainwindow.ui \
    canparamsetting.ui \
    motormatch.ui \
    userdefined.ui \
    waveformwidget.ui

#CONFIG += qwt
#DEFINES += QT_DLL QWT_DLL
#LIBS += -L"C:\Qt\build-qwt-Desktop_Qt_5_9_3_MinGW_32bit-Debug\lib" -lqwtd
#LIBS += -L"C:\Qt\build-qwt-Desktop_Qt_5_9_3_MinGW_32bit-Debug\lib" -lqwt
#INCLUDEPATH += C:\Qt\Qt5.9.3\5.9.3\mingw53_32\include\qwt

LIBS += -L"C:\Users\Administrator\Desktop\Driver_Serial" -lControlCAN

win32: LIBS += -L$$PWD/./ -lControlCAN

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.

DISTFILES +=

RESOURCES += \
    resonrces.qrc
