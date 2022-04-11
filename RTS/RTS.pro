include(../include/config.pri)

QT += core gui network svg
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DEFINES += RD_EXCEPTION QWT_DLL

TARGET = RTS
TEMPLATE = app
RC_ICONS = RTS.ico

UI_DIR = ./ui

LIBS += \
-ldriver \
-linstrument \
-lreport \
-lsequence \
-lQHelper \
-lRTS_helper

LIBS += $$dependLib(log)

win32-g++ {
    LIBS += -lqwt-qt5.dll
}

win32-clang-g++ {
    LIBS += -lqwt-qt5.dll
}

win32-msvc {
    LIBS += -lqwtd
}

HEADERS += \
    global.h \
    main_thread.h \
    main_dialog.h \
    mainwindow.h \
    log_model.hpp \
    device_init_thread.h

SOURCES += \
    global.cpp \
    main.cpp \
    main_thread.cpp \
    main_dialog.cpp \
    mainwindow.cpp \
    device_init_thread.cpp

FORMS += \
    main_dialog.ui \
    mainwindow.ui \
    common/sp1403_r1a.ui \
    common/config_result.ui
