DEFINES += device_api
VENDOR = Ericsson

include(../../../include/config.pri)

HEADERS += \
    ../../frontend.h \
    api.h \
    6449.h \
    cal_file.h \
    cal_table_tx_aclr.h \
    cal_table_tx_vga.h \
    cal_table_rx_rf_vga.h \
    cal_table_rx_gain_accu.h

SOURCES += \
    ../../frontend.cpp \
    api.cpp \
    6449.cpp \
    cal_file.cpp \
    cal_table_tx_aclr.cpp \
    cal_table_tx_vga.cpp \
    cal_table_rx_rf_vga.cpp \
    cal_table_rx_gain_accu.cpp
