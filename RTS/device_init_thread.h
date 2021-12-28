#ifndef QDEVICEINITTHREAD_H
#define QDEVICEINITTHREAD_H

#include "winthread.h"

class QDeviceInitThread : public Q_Thread
{
    Q_OBJECT

public:
    QDeviceInitThread(QObject *parent = nullptr) :
        Q_Thread(parent) { silent = false; }
    void run();
    bool initSP9500();
    bool initSP9500X();

public:
    bool silent;
};

#endif // QDEVICEINITTHREAD_H
