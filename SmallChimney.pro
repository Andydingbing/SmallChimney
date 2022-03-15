TEMPLATE = subdirs

DEVICES += \
device/Ericsson_Radio_4415 \
device/Ericsson_Radio_6449 \
device/Ericsson_Air_3268 \
device/StarPoint/SP9500

SUBDIRS += \
log \
driver \
instrument \
report \
sequence \
sequence_compiler \
Qt/QHelper.pro \
RTS \
DDT \
$$DEVICES \
RTS/Ericsson/Radio_4415_B3/ericsson_radio_4415_b3.pro

instrument.depends = driver
report.depends = log
sequence_compiler.depends = sequence

DDT.depends = log driver
RTS.depends = \
    sequence \
    instrument \
    $$DEVICES
