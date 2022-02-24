#ifndef Q_CAL_R1C_TX_TEMP_COMP_THREAD_H
#define Q_CAL_R1C_TX_TEMP_COMP_THREAD_H

#include "q_winthread.h"

struct Point {
    quint32 points;
    double avgPwr;
    Point() : points(0),avgPwr(-100.0) {}
};

class QCalR1CTxTempCompThread : public QCalR1CBaseThread
{
public:
    explicit QCalR1CTxTempCompThread(const CalParam &param) :
        QCalR1CBaseThread(param) { avgTime = 5; }
    void cal();
    void initTXChain();
    void getTemp0(double &temp) const;
    void getTemp1(double &temp) const;

private:
    quint32 avgTime;
};


class QExpR1CTxTempCompThread : public QExpR1CBaseThread
{
public:
    explicit QExpR1CTxTempCompThread(const CalParam &param) :
        QExpR1CBaseThread(param) {}
    void run();
};


class QCalR1CRXTempCompThread : public QCalR1CBaseThread
{
public:
    explicit QCalR1CRXTempCompThread(const CalParam &param) :
        QCalR1CBaseThread(param) { avgTime = 5; }
    void cal();
    void initRXChain();
    void getTemp(double &temp) const;

private:
    quint32 avgTime;
};

class QExpR1CRXTempCompThread : public QExpR1CBaseThread
{
public:
    explicit QExpR1CRXTempCompThread(const CalParam &param) :
        QExpR1CBaseThread(param) {}
    void run();
};

#endif // Q_CAL_R1C_TX_TEMP_COMP_THREAD_H
