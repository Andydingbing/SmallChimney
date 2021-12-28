include( ../include/config.pri )

CONFIG += shared
CONFIG -= qt
TEMPLATE = lib
DEFINES += DLL_EXPORT
LIBS += -llog

win32 { LIBS += -lws2_32 }

HEADERS += \
    memory.h \
    item_table.hpp \
    report_header.h \
    report.h

SOURCES += \
    memory.cpp \
    item_table.cpp \
    report_header.cpp
