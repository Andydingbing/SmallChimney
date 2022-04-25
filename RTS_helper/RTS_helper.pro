include(../include/config.pri)

CONFIG += shared
TEMPLATE = lib
DEFINES += RTS_HELPER_DLL_EXPORT

QT += core gui network svg
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

LIBS += -lsequence

HEADERS += \
    global.h \
    QTreeWidget_helper.h

SOURCES += \
    global.cpp \
    QTreeWidget_helper.cpp

