#ifndef Q_TEST_FREQ_RES_THREAD_H
#define Q_TEST_FREQ_RES_THREAD_H

#include "q_winthread.h"
#include "q_model_freq_res.h"

struct TestFreqResParam : TestBaseParam
{
    bool isTestRF_Tx;
    bool isTestIF_Tx;
    bool isTestRF_Rx;
    bool isTestIF_Rx;
    QString RF_TxFreqStar;
    QString RF_TxFreqStop;
    QString RF_TxFreqStep;
    QString RF_RxFreqStar;
    QString RF_RxFreqStop;
    QString RF_RxFreqStep;
};

class Q_Test_FreqRes_Thread : public QTestThread
{
public:
    explicit Q_Test_FreqRes_Thread(TestBaseParam *param) :
        QTestThread(param),
        pwrBefore(0.0),
        pwrMin(0.0),
        pwrMax(0.0) { }

    void run();
    virtual void sweepTx() = 0;
    virtual void sweepRx() = 0;
    void checkRF_Tx(rd::rf_tx_freq_res_test_data &data);
    void checkIF_Tx(rd::if_tx_freq_res_test_data &data);
    void checkRF_Rx(rd::rf_rx_freq_res_test_data &data);
    void checkIF_Rx(rd::if_rx_freq_res_test_data &data);

protected:
    TestFreqResParam *param;
    range_freq_string RF_TXFreqString;
    range_freq_string RF_RXFreqString;
    range_freq<quint64> RF_TXFreqRange;
    range_freq<quint64> RF_RXFreqRange;
    QwtRF_TX_FreqResData *dataRF_Tx;
    QwtIF_TX_FreqResData *dataIF_Tx;
    QwtRF_RX_FreqResData *dataRF_Rx;
    QwtIF_RX_FreqResData *dataIF_Rx;
    double pwrBefore;
    double pwrMin;
    double pwrMax;
};

// The signal : update(const QModelIndex, // Not used.
//                     const QModelIndex, // Not used.
//                     test_item_t,       // The current test item.
//                     int sec)           // The test result.0:fail;1:pass.
class QTestR1CFreqResThread : public Q_Test_FreqRes_Thread
{
public:
    explicit QTestR1CFreqResThread(TestBaseParam *param) :
        Q_Test_FreqRes_Thread(param) { }

    void sweepTx();
    void sweepRx();

};

#endif // Q_TEST_FREQ_RES_THREAD_H
