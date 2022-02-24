#ifndef QCALR1CRXFILTEROFFSETTHREAD_H
#define QCALR1CRXFILTEROFFSETTHREAD_H

#include "q_winthread.h"

class QCalR1CRXFilterOffsetThread : public QCalR1CBaseThread
{
public:
    explicit QCalR1CRXFilterOffsetThread(const CalParam &param) :
        QCalR1CBaseThread(param) {}
    void run();
};


class QExpR1CRXFilterOffsetThread : public QExpR1CBaseThread
{
public:
    explicit QExpR1CRXFilterOffsetThread(const CalParam &param) :
        QExpR1CBaseThread(param) {}
    void run();
};

#endif // QCALR1CRXFILTEROFFSETTHREAD_H
