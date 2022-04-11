DEFINES += device_api
VENDOR = StarPoint

include( ../../../include/config.pri )

LIBS += -lfftw3

#win32 { LIBS += -lws2_32 }

HEADERS += \
    ../../frontend.h \
    ../arb_reader.h \
    spec.h \
    rf_driver.h \
    cal_file.h \
    dma_mgr.h \
    complex_sequence.h \
    sp1401.h \
    sp1401_r1a.h \
    sp1401_r1b.h \
    sp1401_r1c.h \
    sp1401_r1d.h \
    sp1401_r1e.h \
    sp1401_r1f.h \
    sp2401_r1a.h \
    sp3301.h \
    sp3501.h \
    self_cal_helper.h \
    rd_sp9500.h \
    ../../cal_table_freq_response.h \
    cal_table_x9119.h \
    cal_table_tx_lol.h \
    cal_table_tx_sideband.h \
    cal_table_tx_filter.h \
    cal_table_tx_pwr.h \
    cal_table_tx_att.h \
    cal_table_tx_filter_offset.h \
    cal_table_rx_filter.h \
    cal_table_rx_ref.h \
    cal_table_rx_att.h \
    cal_table_rx_filter_offset.h

SOURCES += \
    ../../frontend.cpp \
    ../arb_reader.cpp \
    spec.cpp \
    rf_driver.cpp \
    cal_file.cpp \
    dma_mgr.cpp \
    complex_sequence.cpp \
    sp1401.cpp \
    sp1401_r1a.cpp \
    sp1401_r1c.cpp \
    sp1401_r1e.cpp \
    sp1401_r1f.cpp \
    sp2401_r1a.cpp \
    sp3301.cpp \
    sp3501.cpp \
    self_cal_helper.cpp \
    rd_sp9500.cpp \
    ../../cal_table_freq_response.cpp \
    cal_table_x9119.cpp \
    cal_table_tx_lol.cpp \
    cal_table_tx_sideband.cpp \
    cal_table_tx_pwr.cpp \
    cal_table_tx_att.cpp \
    cal_table_tx_filter_offset.cpp \
    cal_table_rx_ref.cpp \
    cal_table_rx_att.cpp \
    cal_table_rx_filter_offset.cpp
