#ifndef QR1CTEMPCTRLTHREAD_H
#define QR1CTEMPCTRLTHREAD_H

#include "starpoint_sp9500_global.h"
#include "starpoint_sp9500_temp_ctrl_model.h"
#include "winthread.h"
#include <QTimer>

class QR1CTempCtrlThread : public Q_Thread
{
    Q_OBJECT

public:
    struct Param {
        QVector<QVector<QwtTempData *> *> data;
        sp3301 *SP3301;
    };

    Param param;

public:
    explicit QR1CTempCtrlThread(Param *param, QObject *parent = nullptr);
    void run();

signals:
    void update(double *temp);
};

#endif
