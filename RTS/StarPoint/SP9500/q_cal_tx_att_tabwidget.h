#ifndef Q_CAL_TX_ATT_TABWIDGET_H
#define Q_CAL_TX_ATT_TABWIDGET_H

#include "rddt_plot.hpp"
#include "rddt_tableview.hpp"

struct CalParam;

class QwtPlotCurve;
class QR1CTXAttOPModel;
class QR1CTXAttIOModel;

class QCalR1CTXAttTabWidget : public QTabWidget
{
public:
    QCalR1CTXAttTabWidget(QWidget *parent = nullptr);
    void init(QR1CTXAttOPModel *model_OP,QR1CTXAttIOModel *model_IO);
    void resetShowWidget(CalParam *param);

public:
    Q_RDDT_CalPlot *plotOP;
    Q_RDDT_CalPlot *plotIO;
    QVector<QwtPlotCurve *> *curveOP;
    QVector<QwtPlotCurve *> *curveIO;
    Q_RDDT_TableView *tableViewOP;
    Q_RDDT_TableView *tableViewIO;
    QR1CTXAttOPModel *modelOP;
    QR1CTXAttIOModel *modelIO;
};

#endif // Q_CAL_TX_ATT_TABWIDGET_H
