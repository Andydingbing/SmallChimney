include(../include/config.pri)

CONFIG += shared
TEMPLATE = lib
DEFINES += RTS_HELPER_DLL_EXPORT

QT += core gui network svg
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

LIBS += \
-llog \
-ldriver \
-linstrument \
-lreport \
-lsequence -lQt5Core -lQt5Gui -lQt5Network -lQt5svg -lQt5Widgets

HEADERS += \
    global.h

SOURCES += \
    global.cpp
