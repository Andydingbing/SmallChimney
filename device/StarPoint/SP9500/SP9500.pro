DEFINES += product

include( ../../../include/config.pri )

LIBS += -lfftw3

DESTDIR = ../$$DESTDIR

win32 { LIBS += -lws2_32 }

HEADERS += \
    ../../frontend.h \
    ../../common/ftplib.h \
    ../arb_reader.h \
    spec.h \
    rf_driver.h \
    cal_file.h \
    cfv_ctrl.h \
    cal_file_r1ab.h \
    cal_file_r1cd.h \
    tx_lol_table.h \
    tx_sb_table.h \
    tx_filter_table.h \
    tx_pwr_table.h \
    tx_att_table.h \
    tx_filter_offset_table.h \
    rx_filter_table.h \
    rx_ref_table.h \
    rx_att_table.h \
    rx_filter_offset_table.h \
    tx_tc_table.h \
    dma_mgr.h \
    complex_sequence.h \
    ftp.h \
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
    rd_sp9500.h

SOURCES += \
    ../../frontend.cpp \
    ../../common/ftplib.c \
    ../arb_reader.cpp \
    spec.cpp \
    rf_driver.cpp \
    cal_file.cpp \
    cfv_ctrl.cpp \
    cal_file_r1ab.cpp \
    cal_file_r1cd.cpp \
    tx_lol_table.cpp \
    tx_sb_table.cpp \
    tx_filter_table.cpp \
    tx_pwr_table.cpp \
    tx_att_table.cpp \
    tx_filter_offset_table.cpp \
    rx_filter_table.cpp \
    rx_ref_table.cpp \
    rx_att_table.cpp \
    rx_filter_offset_table.cpp \
    tx_tc_table.cpp \
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
    rd_sp9500.cpp
