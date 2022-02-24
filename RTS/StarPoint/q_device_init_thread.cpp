#include "q_device_init_thread.h"
#include "mainwindow.h"
#include "child_widget_helper.h"
#include "rf_driver.h"

void QDeviceInitThread::run()
{
    bool result = true;

    emit threadProcess(STARTED);
    initProgress(QString("Init"),100);

    switch(project) {
    case SP9500  : { result = initSP9500();  break; }
    case DT3308  : { result = initDT3308();  break; }
    case SP9500X : { result = initSP9500X(); break; }
    default      : { return; }
    }

    if (result == false) {
        emit threadProcess(END_EXCEPT);
        Log.add_msg(-1,"Boot Fail");
        return;
    }

    SET_PROG_POS(100);
    Log.add_msg(0,"Booted");

    emit threadProcess(END_NORMAL);
    emit done(true);
}

bool QDeviceInitThread::initSP9500()
{
    SET_PROG_POS(20);
    INT_CHECKB(SP3501.open_board());
    SET_PROG_POS(60);
    INT_CHECKB(SP3301->set_program_bit(nullptr,false,false));
    INT_CHECKB(SP3301->program_bit());
    INT_CHECKB(SP3301->boot());

    emit swhwVerReady(SP3301->get_info());

    SET_PROG_POS(90);

    return true;
}

bool QDeviceInitThread::initDT3308()
{
    SET_PROG_POS(20);
    INT_CHECKB(DT3308_RF_Boot());
    SET_PROG_POS(60);
    SET_PROG_POS(90);

    return true;
}

bool QDeviceInitThread::initSP9500X()
{
    SET_PROG_POS(20);
    INT_CHECKB(SP3103_0.boot(silent));

    return true;
}
