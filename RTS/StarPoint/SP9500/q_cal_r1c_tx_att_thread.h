#ifndef Q_CAL_R1C_TX_ATT_THREAD_H
#define Q_CAL_R1C_TX_ATT_THREAD_H

#include "q_winthread.h"

class QCalR1CTXAttThread : public QCalR1CBaseThread
{
public:
    explicit QCalR1CTXAttThread(const CalParam &param) :
        QCalR1CBaseThread(param) { idxOffsetStar = 0; }
    void run();
    void checkIt(io_mode_t mode);
    void calOneSec(io_mode_t mode, double pwrStar, double pwrStop);

private:
    double ajustSA(double pwr) const;

private:
    // The index of the start att configuration of one section;
    quint32 idxOffsetStar;
};


class QExpR1CTXAttThread : public QExpR1CBaseThread
{
public:
    explicit QExpR1CTXAttThread(const CalParam &param) :
        QExpR1CBaseThread(param) {}
    void run();
};

#endif // Q_CAL_R1C_TX_ATT_THREAD_H
