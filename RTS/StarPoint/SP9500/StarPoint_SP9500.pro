DEFINES += ui_api
VENDOR = StarPoint
LIBS += -ldevice_starpoint_sp9500

include(../../../include/config.pri)

HEADERS += \
    global.h \
    child_widget.h \
    adf5355_dlg.h \
    sp1401_r1a_widget.h \
    sp1401_r1c_widget.h \
    sp1401_r1f_widget.h \
    sp1401_r1a_adv_widget.h \
    sp1401_r1c_adv_widget.h \
    sp1401_r1f_adv_widget.h \
    temp_ctrl_widget.h \
    cal_tx_lo_leak.h \
    cal_tx_sideband.h \
    cal_tx_pwr.h

SOURCES += \
    global.cpp \
    child_widget.cpp \
    adf5355_dlg.cpp \
    sp1401_r1a_widget.cpp \
    sp1401_r1c_widget.cpp \
    sp1401_r1f_widget.cpp \
    sp1401_r1a_adv_widget.cpp \
    sp1401_r1c_adv_widget.cpp \
    sp1401_r1f_adv_widget.cpp \
    temp_ctrl_widget.cpp \
    cal_tx_lo_leak.cpp \
    cal_tx_sideband.cpp \
    cal_tx_pwr.cpp

FORMS += \
    ../../chip/adf5355.ui \
    sp1401_r1a.ui \
    sp1401_r1c.ui \
    sp1401_r1f.ui \
    sp1401_r1a_adv.ui \
    sp1401_r1c_adv.ui \
    sp1401_r1f_adv.ui \
    temp_ctrl_widget.ui \
    cal_tx_lo_leak.ui \
    cal_tx_sideband.ui \
    cal_tx_pwr.ui

