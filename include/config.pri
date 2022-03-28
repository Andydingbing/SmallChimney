# Thrid party libraries
QWT_VER    = 6.1.3
FFTW_VER   = 3.3.8
BOOST_VER  = 1_75_0
PYTHON_VER = 3.9
FMTLIB_VER = 8.0.1

LIB_DIR_QWT    = .
LIB_DIR_FFTW   = .
LIB_DIR_BOOST  = .
LIB_DIR_PYTHON = .
LIB_DIR_FMTLIB = .


defineReplace(dependLib) {
    win32-g++       { return ($$sprintf("-l$$1%1",".dll")) }
    win32-clang-g++ { return ($$sprintf("-l$$1%1",".dll")) }
    win32-msvc      { return ($$-l$$1) }
}

contains(QT_ARCH,i386) {
    VAR_ARCH = x86
    VAR_ARCH_BIT = 32
} else {
    VAR_ARCH = x64
    VAR_ARCH_BIT = 64
}

CONFIG(debug,debug|release) {
    DEFINES += _DEBUG
    VAR_DEBUG_RELEASE = debug
} else {
    VAR_DEBUG_RELEASE = release
}

win32-g++ {
    make_spec = win32-g++

    VAR_INCLUDE_ROOT = ..
    VAR_LIB_ROOT = C:/msys64/mingw$$VAR_ARCH_BIT/lib
    VAR_INCLUDE_SYSTEM = C:/msys64/mingw$$VAR_ARCH_BIT

    INCLUDEPATH += \
        $$VAR_INCLUDE_SYSTEM/include/boost \
        $$VAR_INCLUDE_SYSTEM/include/qwt-qt5 \

    LIBS += \
        -lboost_timer-mt \
        -lboost_chrono-mt \
        -lboost_system-mt \
        -lboost_filesystem-mt \
        -lfmt
}

win32-clang-g++ {
    make_spec = win32-clang-g++

    VAR_INCLUDE_ROOT = C:/msys64/clang$$VAR_ARCH_BIT
    VAR_LIB_ROOT = C:/msys64/clang$$VAR_ARCH_BIT/lib

    LIBS += \
        -lboost_timer-mt \
        -lboost_chrono-mt \
        -lboost_system-mt \
        -lboost_filesystem-mt \
        -lfmt
}

win32-msvc {
    make_spec = win32-msvc

    VAR_INCLUDE_ROOT = ..
    VAR_LIB_ROOT = ../lib/$$VAR_ARCH/$$make_spec

    LIB_DIR_QWT    = $$VAR_LIB_ROOT/qwt/$$QWT_VER
    LIB_DIR_FFTW   = $$VAR_LIB_ROOT/fftw/$$FFTW_VER
    LIB_DIR_BOOST  = $$VAR_LIB_ROOT/boost/$$BOOST_VER
    LIB_DIR_PYTHON = $$VAR_LIB_ROOT/python$$PYTHON_VER
    LIB_DIR_FMTLIB = $$VAR_LIB_ROOT/fmt/$$FMTLIB_VER

    # Multi-Processor
    QMAKE_CFLAGS += /MP
    QMAKE_CFLAGS_DEBUG += /MP
    QMAKE_CXXFLAGS += /MP
    QMAKE_CXXFLAGS_DEBUG += /MP

    # QMAKE_LFLAGS_DEBUG -= /DEBUG

    contains(VAR_DEBUG_RELEASE,release) {
        LIBS += -lfmt
    } else {
        LIBS += -lfmtd
    }
}

linux-g++ {
    make_spec = linux-g++

    LIBS += \
        -lboost_timer \
        -lboost_chrono \
        -lboost_system \
        -lboost_filesystem \
        -lfmt
}

win32 {
    LIBS += \
        -lvisa$$VAR_ARCH_BIT \
        -lrsnrpz_$$VAR_ARCH_BIT \
        -lAdvapi32 \
        -lSetupapi
}

unix {
    LIBS += \
        -L/usr/lib64 \
        -lvisa
}

INCLUDEPATH += \
          $$VAR_INCLUDE_ROOT/include \
       ../$$VAR_INCLUDE_ROOT/include \
    ../../$$VAR_INCLUDE_ROOT/include \
          $$VAR_INCLUDE_ROOT/include/boost/$$BOOST_VER \
       ../$$VAR_INCLUDE_ROOT/include/boost/$$BOOST_VER \
    ../../$$VAR_INCLUDE_ROOT/include/boost/$$BOOST_VER \
          $$VAR_INCLUDE_ROOT/include/fmt/$$FMTLIB_VER \
       ../$$VAR_INCLUDE_ROOT/include/fmt/$$FMTLIB_VER \
    ../../$$VAR_INCLUDE_ROOT/include/fmt/$$FMTLIB_VER \
          $$VAR_INCLUDE_ROOT/include/qwt/$$QWT_VER \
       ../$$VAR_INCLUDE_ROOT/include/qwt/$$QWT_VER \
    ../../$$VAR_INCLUDE_ROOT/include/qwt/$$QWT_VER \
          $$VAR_INCLUDE_ROOT/log \
       ../$$VAR_INCLUDE_ROOT/log \
    ../../$$VAR_INCLUDE_ROOT/log \
          $$VAR_INCLUDE_ROOT/driver \
       ../$$VAR_INCLUDE_ROOT/driver \
    ../../$$VAR_INCLUDE_ROOT/driver \
          $$VAR_INCLUDE_ROOT/utilities \
       ../$$VAR_INCLUDE_ROOT/utilities \
    ../../$$VAR_INCLUDE_ROOT/utilities \
          $$VAR_INCLUDE_ROOT/device \
       ../$$VAR_INCLUDE_ROOT/device \
    ../../$$VAR_INCLUDE_ROOT/device \
          $$VAR_INCLUDE_ROOT/report \
       ../$$VAR_INCLUDE_ROOT/report \
    ../../$$VAR_INCLUDE_ROOT/report \
          $$VAR_INCLUDE_ROOT/device/common \
       ../$$VAR_INCLUDE_ROOT/device/common \
    ../../$$VAR_INCLUDE_ROOT/device/common \
          ../Qt \
       ../../Qt \
    ../../../Qt \
    ./chip \
    ../device/chip \
    ./common \
    ./common/analyzer \
    ../device/common/analyzer \
    ../report \
    ../../report \
    ../instrument \
    ../sequence \
    ../../sequence \

DESTDIR     = ../$$VAR_ARCH/$$VAR_DEBUG_RELEASE/$$make_spec
OBJECTS_DIR =  ./$$VAR_ARCH/$$VAR_DEBUG_RELEASE/$$make_spec

contains(DEFINES,device_api) {
    DEFINES += DLL_EXPORT
    CONFIG += shared
    CONFIG -= qt
    TEMPLATE = lib
    LIBS += -ldriver -lreport
    DESTDIR = ../../../$$VAR_ARCH/$$VAR_DEBUG_RELEASE/$$make_spec

    LIBS += $$dependLib(log)
}

contains(DEFINES,frontend) {
    DEFINES += QWT_DLL DLL_EXPORT RD_EXCEPTION
    CONFIG += shared qt
    TEMPLATE = lib
    INCLUDEPATH += ../..

    QT += core gui network svg
    greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

    DESTDIR = ../../../$$VAR_ARCH/$$VAR_DEBUG_RELEASE/$$make_spec
    UI_DIR  = ./ui

    LIBS += \
        -ldriver \
        -linstrument \
        -lreport \
        -lRTS_helper \
        -lsequence \
        -lQt5Core -lQt5Gui -lQt5Network -lQt5svg -lQt5Widgets -lQHelper

    LIBS += $$dependLib(log)

    win32-g++       { LIBS += -lqwt-qt5.dll }
    win32-clang-g++ { LIBS += -lqwt-qt5.dll }
    win32-msvc {
        equals(VAR_DEBUG_RELEASE, "debug")   { LIBS += -lqwtd }
        equals(VAR_DEBUG_RELEASE, "release") { LIBS += -lqwt }
    }
}

LIBS += \
          -L../lib/$$VAR_ARCH \
       -L../../lib/$$VAR_ARCH \
    -L../../../lib/$$VAR_ARCH \
          -L$$VAR_LIB_ROOT \
       -L../$$VAR_LIB_ROOT \
    -L../../$$VAR_LIB_ROOT \
          -L$$LIB_DIR_FFTW \
       -L../$$LIB_DIR_FFTW \
    -L../../$$LIB_DIR_FFTW \
          -L$$LIB_DIR_BOOST \
       -L../$$LIB_DIR_BOOST \
    -L../../$$LIB_DIR_BOOST \
          -L$$LIB_DIR_FMTLIB \
       -L../$$LIB_DIR_FMTLIB \
    -L../../$$LIB_DIR_FMTLIB \
          -L$$LIB_DIR_QWT \
       -L../$$LIB_DIR_QWT \
    -L../../$$LIB_DIR_QWT \
          -L$$DESTDIR \
       -L../$$DESTDIR \
    -L../../$$DESTDIR \


message("$$VAR_ARCH $$make_spec")
message($$INCLUDEPATH)
message($$LIBS)
message($$LIB_DIR_QWT)
message($$LIB_DIR_FFTW)
message($$LIB_DIR_BOOST)
message($$LIB_DIR_PYTHON)
message($$LIB_DIR_FMTLIB)
message($$DESTDIR)
message($$OBJECTS_DIR)
