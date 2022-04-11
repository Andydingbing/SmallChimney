DEFINES += ui_api
VENDOR = Ericsson
LIBS += -ldevice_ericsson_radio_4415

include(../../../include/config.pri)

HEADERS += \
    child_widget.h \
    ericsson_radio_4415_b3_widget.h \
    cal_tx_vga.h \
    cal_rx_rf_vga.h \
    test_tx_aclr.h \
    test_rx_gain_accuracy.h

SOURCES += \
    child_widget.cpp \
    ericsson_radio_4415_b3_widget.cpp \
    cal_tx_vga.cpp \
    cal_rx_rf_vga.cpp \
    test_tx_aclr.cpp \
    test_rx_gain_accuracy.cpp

FORMS += \
    ericsson_radio_4415_b3.ui \
    cal_tx_vga.ui \
    cal_rx_rf_vga.ui \
    test_tx_aclr.ui \
    test_rx_gain_accuracy.ui
