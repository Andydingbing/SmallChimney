#include "device_init_thread.h"
#include "mainwindow.h"
#include "device.h"

void QDeviceInitThread::run()
{
    bool result = true;

    emit threadProcess(Started);
    initProgress(QString("Init"),100);

    switch(project) {
    case Ericsson_Radio_4415_B3 : { result = initSP9500X(); break; }
    default      : { return; }
    }

    if (result == false) {
        emit threadProcess(End_Except);
        LoggerMsg.add(-1,"Boot Fail");
        return;
    }

    setProgressPos(100);
    LoggerMsg.add(0,"Booted");

    emit threadProcess(End_Normal);
    emit done(true);
}

bool QDeviceInitThread::initSP9500X()
{
    setProgressPos(20);

    return true;
}
