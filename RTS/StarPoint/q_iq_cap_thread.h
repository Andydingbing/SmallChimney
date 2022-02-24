#ifndef QIQCAPTHREAD_H
#define QIQCAPTHREAD_H

#include "q_winthread.h"

class QIQCapThread : public QWinThread
{
    Q_OBJECT

public:
    explicit QIQCapThread(QObject *parent = nullptr) : QWinThread(parent) {}
    void run();

signals:
    void updatePlot();
};

#endif // QIQCAPTHREAD_H
