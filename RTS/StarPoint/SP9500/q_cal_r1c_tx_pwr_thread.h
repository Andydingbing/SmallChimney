#ifndef Q_CAL_R1C_TX_PWR_THREAD_H
#define Q_CAL_R1C_TX_PWR_THREAD_H

#include "q_winthread.h"

class QCalR1CTXPwrThread : public QCalR1CBaseThread
{
public:
    explicit QCalR1CTXPwrThread(const CalParam &param) :
        QCalR1CBaseThread(param) {}
    void run();

    void calIt(io_mode_t mode);
    void checkIt(io_mode_t mode);
    virtual void init();
    virtual void initTXBaseStateOP(double &att0,
                                   double &att1,
                                   double &att2,
                                   double &att3,
                                   double &dGain);

    virtual void initTXBaseStateIO(double &att0,
                                   double &att1,
                                   double &att2,
                                   double &att3,
                                   double &dGain);

    virtual void coarseTuning(double target, double &att0, double &att1);
    virtual void fineTuning(double target, double &dGain);
};


class QExpR1CTXPwrThread : public QExpR1CBaseThread
{
public:
    explicit QExpR1CTXPwrThread(const CalParam &param) :
        QExpR1CBaseThread(param) {}
    void run();
};

#endif // Q_CAL_R1C_TX_PWR_THREAD_H
