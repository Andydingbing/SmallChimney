include(../include/config.pri)

CONFIG += shared
CONFIG -= qt
TEMPLATE = lib
DEFINES += PLUGIN_DLL_EXPORT

HEADERS += \
    global.h

SOURCES += \
    global.cpp

