include( ../include/config.pri )

CONFIG += shared
CONFIG -= qt
TEMPLATE = lib
DEFINES += DLL_EXPORT
LIBS += -ldriver

HEADERS += \
libinstrument.h \
instr.h \
pm.h \
pm_rs_nrp.h \
ps.h \
ps_keysight_603xa.h \
sa.h \
sa_keysight_psa.h \
sa_keysight_x.h \
sa_rs_fsup.h \
sg.h \
sg_keysight_esg.h \
sg_keysight_psg.h \
sg_keysight_x.h \
sg_rs_smf100a.h

SOURCES += \
libinstrument.cpp \
instr.cpp \
pm_rs_nrp.cpp \
ps.cpp \
sa.cpp \
sa_rs_fsup.cpp \
sg.cpp \
python.cpp
