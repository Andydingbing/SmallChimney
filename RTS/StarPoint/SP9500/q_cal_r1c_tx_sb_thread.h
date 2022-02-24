#ifndef Q_CAL_R1C_TX_SB_THREAD_H
#define Q_CAL_R1C_TX_SB_THREAD_H

#include "q_winthread.h"

class QCalR1CTXSBThread : public QCalR1CBaseThread
{
public:
    explicit QCalR1CTXSBThread(const CalParam &param) :
        QCalR1CBaseThread(param) {}
    void run();

public:
    void initTXChain();
    void calUseSA(double &thM,uint16_t &am_i_m,uint16_t &am_q_m,double &pwrSb);

public:
    double slopeTh_SA(double th,double delta,double &pwr);
    double slopeAmI_SA(uint16_t am_i,uint16_t am_q,uint16_t delta,double &pwr);
    double slopeAmQ_SA(uint16_t am_i,uint16_t am_q,uint16_t delta,double &pwr);
};

class QExpR1CTXSBThread : public QExpR1CBaseThread
{
public:
    explicit QExpR1CTXSBThread(const CalParam &param) :
        QExpR1CBaseThread(param) {}
    void run();
};

#endif // Q_CAL_R1C_TX_SB_THREAD_H
