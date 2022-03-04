include(../include/config.pri)

include(Ericsson/Radio_4415.pri)
include(Ericsson/Air_3268.pri)

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
-llog \
-ldriver \
-linstrument \
-lreport \
-lsequence \
-lEricsson_Radio_4415 \
-lEricsson_Air_3268
#-lSP9500

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
    ../Qt/config_table.h \
    ../Qt/table_model.h \
    ../Qt/thread_widget.h \
    ../Qt/qwt_plot_helper.h \
    main_thread.h \
    main_dialog.h \
    mainwindow.h \
    log_model.hpp \
    child_widget_helper.h \
    device_init_thread.h \
    winthread.h \
    signal_analyzer_freq_widget.h

SOURCES += \
    ../Qt/config_table.cpp \
    ../Qt/table_model.cpp \
    ../Qt/thread_widget.cpp \
    ../Qt/qwt_plot_helper.cpp \
    global.cpp \
    main.cpp \
    main_thread.cpp \
    main_dialog.cpp \
    mainwindow.cpp \
    child_widget_helper.cpp \
    device_init_thread.cpp \
    winthread.cpp \
    signal_analyzer_freq_widget.cpp

FORMS += \
    main_dialog.ui \
    mainwindow.ui \
    common/sp1403_r1a.ui \
    common/config_result.ui
