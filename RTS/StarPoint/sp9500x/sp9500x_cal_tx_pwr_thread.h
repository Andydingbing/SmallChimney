#ifndef SP9500X_CAL_TX_PWR_THREAD_H
#define SP9500X_CAL_TX_PWR_THREAD_H

#include "q_winthread.h"
#include "q_model_tx_filter.h"

namespace NS_SP9500X {

class Q_Cal_TX_Pwr_Thread : public QCalBaseThread
{
public:
    explicit Q_Cal_TX_Pwr_Thread(const CalParam &param) :
        QCalBaseThread(param) { }
    void cal() OVERRIDE;
    void calIt();
    void check();

    void init();

    void coarseTuning(const uint64_t freq, data_f_tx_pwr &data);
    void fineTuning(const uint64_t freq, data_f_tx_pwr &data);

public:
    ns_sp1403::port_t port;
    io_mode_t mode;
};


class Q_Exp_TX_Pwr_Thread : public QExpBaseThread
{
public:
    explicit Q_Exp_TX_Pwr_Thread(const CalParam &param) :
        QExpBaseThread(param) {}
    void run();
};

} // namespace NS_SP9500X

#endif // SP9500X_CAL_TX_PWR_THREAD_H
