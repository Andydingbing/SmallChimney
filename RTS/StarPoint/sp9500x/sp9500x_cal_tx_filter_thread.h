#ifndef SP9500X_CAL_TX_FILTER_THREAD_H
#define SP9500X_CAL_TX_FILTER_THREAD_H

#include "q_winthread.h"
#include "q_model_tx_filter.h"

namespace NS_SP9500X {

class Q_Cal_TXFilter_Thread : public QCalBaseThread
{
public:
    explicit Q_Cal_TXFilter_Thread(const CalParam &param) :
        QCalBaseThread(param) { }
    void cal();
    void check();

    void init();
    void sweepRF_IF_0000_3000();
    void sweepRF_0000_3000();
    void sweepRF_3000_4800();
    void sweepRF_4800_6000();
    void sweepRF_6000_7500();
    void sweepIF_3000_7500();
    void generateFreqResponse();
    void generateCoef();
    void updateCalFile();

public:
    uint64_t BW_Max;
    ns_sp1403::port_t port;
    bool cal_TX0;
    bool cal_TX1;
    bool justRebuildCoef;
    bool cal_RF_IF_FR_0000_3000;
    bool cal_RF_FR_0000_3000;
    bool cal_RF_FR_3000_4800;
    bool cal_RF_FR_4800_6000;
    bool cal_RF_FR_6000_7500;
    bool cal_IF_FR_3000_7500;
};


class Q_Exp_TXFilter_Thread : public QExpBaseThread
{
public:
    explicit Q_Exp_TXFilter_Thread(const CalParam &param) :
        QExpBaseThread(param) {}
    void run();
};

} // namespace NS_SP9500X

#endif // SP9500X_CAL_TX_FILTER_THREAD_H
