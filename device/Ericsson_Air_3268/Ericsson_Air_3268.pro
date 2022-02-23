DEFINES += product

include(../../include/config.pri)

HEADERS += \
    ../frontend.h \
    api.h \
    3268.h \
    cal_file.h \
    cal_table_tx_aclr.h \
    cal_table_tx_vga.h \
    cal_table_rx_rf_vga.h \
    cal_table_rx_gain_accu.h

SOURCES += \
    ../frontend.cpp \
    api.cpp \
    3268.cpp \
    cal_file.cpp \
    cal_table_tx_aclr.cpp \
    cal_table_tx_vga.cpp \
    cal_table_rx_rf_vga.cpp \
    cal_table_rx_gain_accu.cpp
