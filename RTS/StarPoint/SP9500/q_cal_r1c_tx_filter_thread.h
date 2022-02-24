#ifndef Q_CAL_R1C_TX_FILTER_THREAD_H
#define Q_CAL_R1C_TX_FILTER_THREAD_H

#include "q_winthread.h"
#include "q_model_tx_filter.h"

class QCalR1CTXFilterThread : public QCalR1CBaseThread
{
public:
    explicit QCalR1CTXFilterThread(const CalParam &param) :
        QCalR1CBaseThread(param) {}
    void cal();
    void check();

    void initTXChain();
    void sweepRF_0(Qwt_TX_RF_FR_Data *qwtData);
    void sweepRF_1(Qwt_TX_RF_FR_Data *qwtData);
    void sweepIF(Qwt_TX_IF_FR_Data *qwtData);
    void generateFreqResponse();
    void generateCoef();
    void updateCalFile();
};


class QExpR1CTXFilterThread : public QExpR1CBaseThread
{
public:
    explicit QExpR1CTXFilterThread(const CalParam &param) :
        QExpR1CBaseThread(param) {}
    void run();
};

#endif // Q_CAL_R1C_TX_FILTER_THREAD_H
