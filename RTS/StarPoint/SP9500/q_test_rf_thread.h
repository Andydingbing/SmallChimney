#ifndef Q_TEST_RF_THREAD_H
#define Q_TEST_RF_THREAD_H

#include "q_winthread.h"
#include "q_model_freq_res.h"

struct TestRFParam : TestBaseParam
{
    bool isTestTX_IOSw;
    bool isTestTX_PwrModSw;
    bool isTestTX_FilterSw;
    bool isTestTX_LOLD;
    bool isTestTX_PhaseNoise;
    bool isTestTX_NoiseFloor;
};

// The signal : update(const QModelIndex, // Not used.
//                     const QModelIndex, // Not used.
//                     test_item_t,       // The current test item.
//                     int sec)           // The test result.0:fail;1:pass.
class QTestR1CRFThread : public QTestThread
{
    Q_OBJECT

public:
    explicit QTestR1CRFThread(TestBaseParam *param) :
        QTestThread(param) {}

    void run();
    void TX_IOSw();
    void TX_PwrModSw();
    void TX_FilterSw();
    void TX_FilterSw_R1F();
    void TX_LOLD();
    void TX_PhaseNoise();
    void TX_NoiseFloor();

signals:
    void testTXIOSwBox(const QColor TX, const QColor RX, int &exec);

private:
    TestRFParam *param;
};

#endif // Q_TEST_RF_THREAD_H
