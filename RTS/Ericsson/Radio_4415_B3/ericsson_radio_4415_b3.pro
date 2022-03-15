include(../../../include/config.pri)

CONFIG += shared
TEMPLATE = lib
DEFINES += DLL_EXPORT RD_EXCEPTION

QT += core gui network svg
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#DESTDIR = ../../$$VAR_ARCH/$$VAR_DEBUG_RELEASE/$$make_spec
UI_DIR = ./ui
INCLUDEPATH += ../..

LIBS += \
-llog \
-ldriver \
-linstrument \
-lreport \
-lEricsson_Radio_4415 \
-lsequence -lQt5Core -lQt5Gui -lQt5Network -lQt5svg -lQt5Widgets -lQHelper

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

HEADERS += \
    ericsson_radio_4415_b3_child_widget.h \
    ericsson_radio_4415_b3_widget.h \
    ericsson_radio_4415_b3_cal_tx_vga.h \
    ericsson_radio_4415_b3_cal_rx_rf_vga.h \
    ericsson_radio_4415_b3_test_tx_aclr.h \
    ericsson_radio_4415_b3_test_rx_gain_accuracy.h

SOURCES += \
    ericsson_radio_4415_b3_child_widget.cpp \
    ericsson_radio_4415_b3_widget.cpp \
    ericsson_radio_4415_b3_cal_tx_vga.cpp \
    ericsson_radio_4415_b3_cal_rx_rf_vga.cpp \
    ericsson_radio_4415_b3_test_tx_aclr.cpp \
    ericsson_radio_4415_b3_test_rx_gain_accuracy.cpp

FORMS += \
    ericsson_radio_4415_b3.ui \
    ericsson_radio_4415_b3_cal_tx_vga.ui \
    ericsson_radio_4415_b3_cal_rx_rf_vga.ui \
    ericsson_radio_4415_b3_test_tx_aclr.ui \
    ericsson_radio_4415_b3_test_rx_gain_accuracy.ui
