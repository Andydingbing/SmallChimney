#ifndef MAIN_THREAD_H
#define MAIN_THREAD_H

#include "winthread.h"

class Q_Main_Thread : public Q_Thread_Base
{
    Q_OBJECT

public:
    Q_Main_Thread(QObject *parent = nullptr);

    void run();
    void waitChildThread(int timeout = 0, int idx = 0);

signals:
    void prepare(const int idx);

public slots:
    void childThreadDone(bool success = true);

public:
    bool childThreadRunning;
};

#endif
