#-------------------------------------------------
#
# Project created by QtCreator 2021-11-16T14:34:55
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = serialport_ymodem
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
        mainwindow.cpp \
    download.cpp \
    file.cpp \
    clicklabel.cpp \
    crc16ccitt.cpp

HEADERS += \
        mainwindow.h \
    download.h \
    file.h \
    clicklabel.h \
    crc16ccitt.h

FORMS += \
        mainwindow.ui

RESOURCES += \
    res.qrc

RC_FILE = UploadTool_1ico.rc
