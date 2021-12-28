include( ../include/config.pri )

CONFIG += shared
CONFIG -= qt
TEMPLATE = lib
DEFINES += DLL_EXPORT

unix {
    target.path = /usr/lib
    INSTALLS += target

    DEFINES += UNIX
    DEFINES += LINUX
    DEFINES += KERNEL_64BIT
}

HEADERS += \
    pci_dev.h \
    pci_dev_udp.h \
    pci_dev_vi.h \
    pci_dev_wd.h \
    mem_io_udp.h \
    mem_io_vi.h \
    gpib_dev.h \
    serial_dev.h \
    serial_dev_vi.h \
    libbd.h

SOURCES += \
    pci_dev_udp.cpp \
    pci_dev_vi.cpp \
    pci_dev_wd.cpp \
    mem_io_udp.cpp \
    mem_io_vi.cpp \
    gpib_dev.cpp \
    serial_dev.cpp \
    serial_dev_vi.cpp
