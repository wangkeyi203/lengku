#-------------------------------------------------
#
# Project created by QtCreator 2018-03-20T20:19:22
#
#-------------------------------------------------

QT       += core gui \
            sql      \
            network
CONFIG   -= app_bundle
CONFIG   += C++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = lengku
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
        login.cpp \
        admin.cpp \
        list.cpp \
        my_sqlite.cpp \
        tcpserver.cpp \
        tcpsocket.cpp \
        threadhandle.cpp


HEADERS += \
        login.h \
        admin.h \
        list.h \
        config.h \
        my_sqlite.h \
        tcpserver.h \
        tcpsocket.h \
        threadhandle.h

FORMS += \
        login.ui \
    admin.ui \
    list.ui


