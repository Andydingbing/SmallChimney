#ifndef Q_CAL_R1C_RX_FILTER_THREAD_H
#define Q_CAL_R1C_RX_FILTER_THREAD_H

#include "q_winthread.h"
#include "q_model_rx_filter.h"

class QCalR1CRXFilterThread : public QCalR1CBaseThread
{
public:
    explicit QCalR1CRXFilterThread(const CalParam &param) :
        QCalR1CBaseThread(param) { fakeRef = 0; }
    void run();

public:
    void initRXChain();
    void sweepRF(Qwt_RX_RF_FR_Data *qwtData);
    void sweepIF(Qwt_RX_IF_FR_Data *qwtData);
    void generateFreqResponse();
    void generateCoef();
    void updateCalFile();
    void ajustSG(double freq, qint32 pwr);
    double getRXPwr();

protected:
    qint32 fakeRef;
};


class QExpR1CRXFilterThread : public QExpR1CBaseThread
{
public:
    explicit QExpR1CRXFilterThread(const CalParam &param) :
        QExpR1CBaseThread(param) {}
    void run();
};

#endif // Q_CAL_R1C_RX_FILTER_THREAD_H
