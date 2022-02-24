#ifndef Q_CAL_R1C_RX_REF_THREAD_H
#define Q_CAL_R1C_RX_REF_THREAD_H

#include "q_winthread.h"

class QCalR1CRXRefThread : public QCalR1CBaseThread
{
public:
    explicit QCalR1CRXRefThread(const CalParam &param) :
        QCalR1CBaseThread(param), sgPwr(-160), pmIL(PM_IL) {}
    void run();
    void ajustSG(quint64 freq, double pwr);
    void cal(io_mode_t mode);
    void checkIt(io_mode_t mode);
    virtual void tuning(double &att1, double &att2, double &att3);

private:
    qint32 sgPwr; // Z28 power
    double pmIL;  // Z28 insertion loss
    qint64 ad;    // Last A/D(I^2 + Q^2)
};


class QExpR1CRXRefThread : public QExpR1CBaseThread
{
public:
    explicit QExpR1CRXRefThread(const CalParam &param) :
        QExpR1CBaseThread(param) {}
    void run();
};

#endif // Q_CAL_R1C_RX_REF_THREAD_H
