include( ../include/config.pri )

CONFIG += shared
CONFIG -= qt
TEMPLATE = lib
DEFINES += DLL_EXPORT

HEADERS += \
    log.h \
    logger.h \
    logger_msg.h \
    logger_reg.h \
    logger_com.h \
    liblog.h

SOURCES += \
    logger.cpp \
    logger_msg.cpp \
    logger_reg.cpp \
    logger_com.cpp \
    log.cpp
