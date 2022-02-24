#ifndef QCALR1CTHREAD_H
#define QCALR1CTHREAD_H

#include "q_winthread.h"
#include <QStandardItemModel>

class QCalR1CThread : public QCalBaseThread
{
    Q_OBJECT

public:
    CalR1CParam calParam;
    bool subThreadRunning;
    bool subThreadContinue;
    QStandardItemModel model;

public slots:
    void subThreadDone();
    void subThreadResult(const Process p);

public:
    void waitSubThread(int timeout = 0);

public:
    explicit QCalR1CThread(QObject *parent, const CalR1CParam &param);
    void run();
};

#endif // QCALR1CTHREAD_H
