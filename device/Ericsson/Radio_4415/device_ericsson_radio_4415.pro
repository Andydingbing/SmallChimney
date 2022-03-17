DEFINES += device_api

include(../../../include/config.pri)

HEADERS += \
    ../../frontend.h \
    api.h \
    4415.h \
    4415_b3.h \
    cal_file.h \
    cal_table_tx_aclr.h \
    cal_table_tx_vga.h \
    cal_table_rx_rf_vga.h \
    cal_table_rx_gain_accu.h

SOURCES += \
    ../../frontend.cpp \
    api.cpp \
    4415.cpp \
    4415_b3.cpp \
    cal_file.cpp \
    cal_table_tx_aclr.cpp \
    cal_table_tx_vga.cpp \
    cal_table_rx_rf_vga.cpp \
    cal_table_rx_gain_accu.cpp
