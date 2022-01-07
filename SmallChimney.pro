TEMPLATE = subdirs

DEVICES += \
device/Ericsson_Radio_4415 \
device/Ericsson_Air_3268

SUBDIRS += \
log \
driver \
instrument \
report \
sequence \
sequence_compiler \
RTS \
DDT \
$$DEVICES

instrument.depends = driver
report.depends = log

DDT.depends = log driver
RTS.depends = \
    instrument \
    $$DEVICES
