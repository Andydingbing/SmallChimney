#ifndef Q_CAL_TX_FILTER_TABWIDGET_H
#define Q_CAL_TX_FILTER_TABWIDGET_H

#include "rddt_plot.hpp"
#include "rddt_tableview.hpp"
#include "q_model_tx_filter.h"
#include <QTabWidget>

class QwtPlotCurve;
class QTXFilterModel;

class QCalR1CTXFilterTabWidget : public QTabWidget
{
public:
    QCalR1CTXFilterTabWidget(QWidget *parent = nullptr);
    void init(QTXFilterModel *model80,QTXFilterModel *model160);
    void resetShowWidget();

public:
    // The 2(RF & IF) plots.
    Q_RDDT_CalPlot *plotRF;
    Q_RDDT_CalPlot *plotIF;

    // The curves in particular plot.
    QwtPlotCurve *curveRF_FR_0;
    QwtPlotCurve *curveRF_FR_1;
    QwtPlotCurve *curveIF_FR;

    // The curve data.
    Qwt_TX_RF_FR_Data *dataRF_FR_0;
    Qwt_TX_RF_FR_Data *dataRF_FR_1;
    Qwt_TX_IF_FR_Data *dataIF_FR;

    // The filter coef View/Model.
    Q_RDDT_TableView *tableView_80;
    Q_RDDT_TableView *tableView_160;
    QTXFilterModel *model_80;
    QTXFilterModel *model_160;
};

#endif // Q_CAL_TX_FILTER_TABWIDGET_H
