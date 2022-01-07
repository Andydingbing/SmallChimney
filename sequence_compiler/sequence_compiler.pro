include( ../include/config.pri )

TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lsequence

SOURCES += \
        main.cpp
