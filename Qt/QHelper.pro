include(../include/config.pri)

CONFIG += shared
TEMPLATE = lib
DEFINES += QHELPER_DLL_EXPORT QWT_DLL

QT += core gui network svg
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

LIBS += \
-linstrument \
-lreport \
-lsequence

win32-g++ {
    LIBS += -lqwt-qt5.dll
}

win32-clang-g++ {
    LIBS += -lqwt-qt5.dll
}

win32-msvc {
    equals(VAR_DEBUG_RELEASE, "debug")   { LIBS += -lqwtd }
    equals(VAR_DEBUG_RELEASE, "release") { LIBS += -lqwt }
}

HEADERS += \
    qhelper_global.h \
    scroll_lineedit.h \
    config_table.h \
    table_model.h \
    winthread.h \
    thread_widget.h \
    qwt_plot_helper.h

SOURCES += \
    qhelper_global.cpp \
    config_table.cpp \
    table_model.cpp \
    winthread.cpp \
    thread_widget.cpp \
    qwt_plot_helper.cpp

