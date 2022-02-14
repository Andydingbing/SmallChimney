include( ../include/config.pri )

CONFIG += shared
CONFIG -= qt
TEMPLATE = lib
DEFINES += DLL_EXPORT

HEADERS += \
    sequence.h

SOURCES += \
    sequence.cpp
