#ifndef Q_CAL_R1C_TX_FILTER_OFFSET_THREAD_H
#define Q_CAL_R1C_TX_FILTER_OFFSET_THREAD_H

#include "q_winthread.h"

class QCalR1CTXFilterOffsetThread : public QCalR1CBaseThread
{
public:
    explicit QCalR1CTXFilterOffsetThread(const CalParam &param) :
        QCalR1CBaseThread(param), dds1(0.0) {}
    void run();
    void init();

private:
    double dds1;
};


class QExpR1CTXFilterOffsetThread : public QExpR1CBaseThread
{
public:
    explicit QExpR1CTXFilterOffsetThread(const CalParam &param) :
        QExpR1CBaseThread(param) {}
    void run();
};

#endif // Q_CAL_R1C_TX_FILTER_OFFSET_THREAD_H
