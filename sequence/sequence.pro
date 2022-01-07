include( ../include/config.pri )

CONFIG += shared
CONFIG -= qt
TEMPLATE = lib
DEFINES += DLL_EXPORT

HEADERS += \
    sequence.h \
    parser.h

SOURCES += \
    sequence.cpp \
    parser.cpp
