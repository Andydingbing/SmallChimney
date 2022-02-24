#ifndef Q_CAL_R1C_TX_LOLEAK_THREAD_H
#define Q_CAL_R1C_TX_LOLEAK_THREAD_H

#include "q_winthread.h"

class QCalR1CTXLOLeakThread : public QCalR1CBaseThread
{
public:
    explicit QCalR1CTXLOLeakThread(const CalParam &param) :
        QCalR1CBaseThread(param) {}
    void run();

public:
    void initTXChain();
    void calX9119();
    void calUseSA(qint16 &dc_i_m,qint16 &dc_q_m,double &pwrLOL);
    void calUseLoop(qint16 &dc_i_m,qint16 &dc_q_m,double &pwrLOL);

public:
    double slopeI_SA(qint16 dc_i_m,qint16 dc_q_m,qint16 delta,double &pwr);
    double slopeQ_SA(qint16 dc_i_m,qint16 dc_q_m,qint16 delta,double &pwr);
    int64_t getMinDCOffsetI_Rx(qint16 step,qint16 lr_coef,qint16 *dc_i_l,qint16 *dc_i_r,qint16 *dc_i_m,qint16 *dc_q_m,int64_t *ad);
    int64_t getMinDCOffsetQ_Rx(qint16 step,qint16 lr_coef,qint16 *dc_q_l,qint16 *dc_q_r,qint16 *dc_i_m,qint16 *dc_q_m,int64_t *ad);
};


class QExpR1CTXLOLeakThread : public QExpR1CBaseThread
{
public:
    explicit QExpR1CTXLOLeakThread(const CalParam &param) :
        QExpR1CBaseThread(param) {}
    void run();
};

#endif // Q_CAL_R1C_TX_LOLEAK_THREAD_H
