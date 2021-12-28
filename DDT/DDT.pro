include(../include/config.pri)

QT += core gui network svg
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DEFINES += RD_EXCEPTION

TARGET = DDT
TEMPLATE = app
RC_ICONS = DDT.ico

INCLUDEPATH += \
../device

UI_DIR = ./ui

LIBS += \
-llog \
-ldriver

HEADERS += \
global.h \
../Qt/press_event_combobox.hpp \
mainwindow.h

SOURCES += \
main.cpp \
mainwindow.cpp

FORMS += \
mainwindow.ui
