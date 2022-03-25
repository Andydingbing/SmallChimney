include(../include/config.pri)

#include(StarPoint/SP9500.pri)

QT += core gui network svg
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DEFINES += RD_EXCEPTION

TARGET = RTS
TEMPLATE = app
RC_ICONS = RTS.ico

INCLUDEPATH += \
chip \
test \
../device

UI_DIR = ./ui

LIBS += \
-ldriver \
-linstrument \
-lreport \
-lsequence \
-lQHelper \
-lRTS_helper \
-lericsson_radio_4415_b3 \
-lericsson_radio_6449_b42
#-lSP9500

LIBS += $$dependLib(log)

win32-g++ {
    DEFINES += QWT_DLL
    LIBS += -lqwt-qt5.dll
}

win32-clang-g++ {
    DEFINES += QWT_DLL
    LIBS += -lqwt-qt5.dll
}

win32-msvc {
    equals(VAR_DEBUG_RELEASE, "debug")   { LIBS += -lqwtd }
    equals(VAR_DEBUG_RELEASE, "release") { LIBS += -lqwt }
}

unix  { LIBS += -lrsnrpz -lfftw3 -lqwt}

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
