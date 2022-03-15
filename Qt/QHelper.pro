include(../include/config.pri)

CONFIG += shared
TEMPLATE = lib
DEFINES += QHELPER_DLL_EXPORT

QT += core gui network svg
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

UI_DIR = ./ui
#INCLUDEPATH += ../..

LIBS += \
-llog \
-ldriver \
-linstrument \
-lreport \
-lEricsson_Radio_4415 \
-lsequence -lQt5Core -lQt5Gui -lQt5Network -lQt5svg -lQt5Widgets

HEADERS += \
    qhelper_global.h \
    config_table.h \
    table_model.h \
    winthread.h \
    thread_widget.h \
    QTreeWidget_helper.h

SOURCES += \
    config_table.cpp \
    table_model.cpp \
    winthread.cpp \
    thread_widget.cpp

FORMS += \

