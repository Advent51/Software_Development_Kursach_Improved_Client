#-------------------------------------------------
#
# Project created by QtCreator 2015-12-08T22:03:09
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Sinicin_ver_01
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    logindialog.cpp \
    synclist.cpp \
    drivelist.cpp

HEADERS  += mainwindow.h \
    Encryption.h \
    logindialog.h \
    synclist.h \
    drivelist.h

FORMS    += mainwindow.ui \
    logindialog.ui \
    synclist.ui \
    drivelist.ui
