#ifndef QDEVICEINITTHREAD_H
#define QDEVICEINITTHREAD_H

#include "q_winthread.h"

class QDeviceInitThread : public QWinThread
{
    Q_OBJECT

public:
    QDeviceInitThread(QObject *parent = nullptr) :
        QWinThread(parent) { silent = false; }
    void run();
    bool initSP9500();
    bool initDT3308();
    bool initSP9500X();

signals:
    void swhwVerReady(const sp3301::rfu_info_t &info);

public:
    bool silent;
};

#endif // QDEVICEINITTHREAD_H
