#-------------------------------------------------
#
# Project created by QtCreator 2015-03-20T14:58:18
#
#-------------------------------------------------

QT       += core gui network
CONFIG += c++11

RC_FILE += "configminet.rc"

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QMAKE_CXXFLAGS += -O2 -Os -s
QMAKE_CXXFLAGS += -std=c++11

TARGET = ConfigMinet
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    wiredconnectionwindow.cpp \
    log.cpp

HEADERS  += mainwindow.h \
    wiredconnectionwindow.h \
    log.h

TRANSLATIONS = translate_fr.ts

DISTFILES += \
    configminet.rc \
    ConfigMinet.exe.manifest
