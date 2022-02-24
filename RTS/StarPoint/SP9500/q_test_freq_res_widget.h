#ifndef Q_TEST_FREQ_RES_WIDGET_H
#define Q_TEST_FREQ_RES_WIDGET_H

#include "rddt_plot.hpp"
#include "q_model_freq_res.h"
#include "q_model_temp_pwr.hpp"

struct TestBaseParam;
class QwtPlotCurve;

class QTestFreqResWidget : public QWidget
{
public:
    QTestFreqResWidget(QWidget *parent = nullptr);
    void resetShowWidget(TestBaseParam *param);

public:
    QwtRF_TX_FreqResData *dataRF_TX;
    QwtIF_TX_FreqResData *dataIF_TX;
    QwtRF_RX_FreqResData *dataRF_RX;
    QwtIF_RX_FreqResData *dataIF_RX;
    QwtPlotCurve *curveRF_TX;
    QwtPlotCurve *curveIF_TX;
    QwtPlotCurve *curveRF_RX;
    QwtPlotCurve *curveIF_RX;
    Q_RDDT_TestPlot *plotRF;
    Q_RDDT_TestPlot *plotIF;
};


class QTestTempPwrWidget :public QWidget
{
public:
    QTestTempPwrWidget(QWidget *parent = nullptr);
    void resetShowWidget(TestBaseParam *param);

public:
    QVector<QwtTempPwrData *> data;
    QVector<QwtPlotCurve *> curve;
    Q_RDDT_TestPlot *plot;
};

#endif // Q_TEST_FREQ_RES_WIDGET_H
