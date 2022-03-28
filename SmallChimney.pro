TEMPLATE = subdirs

DEVICES += \
device/Ericsson/Radio_4415/device_ericsson_radio_4415.pro \
device/Ericsson/Radio_6449/device_ericsson_radio_6449.pro \
device/StarPoint/SP9500/device_starpoint_sp9500.pro

SUBDIRS += \
log \
driver \
instrument \
report \
sequence \
sequence_compiler \
Qt/QHelper.pro \
RTS \
RTS_helper \
DDT \
$$DEVICES \
RTS/Ericsson/Radio_4415_B3/ericsson_radio_4415_b3.pro \
RTS/Ericsson/Radio_6449_B42/ericsson_radio_6449_b42.pro

instrument.depends = driver
report.depends = log
sequence_compiler.depends = sequence

DDT.depends = log driver
RTS.depends = \
    sequence \
    instrument \
    $$DEVICES
