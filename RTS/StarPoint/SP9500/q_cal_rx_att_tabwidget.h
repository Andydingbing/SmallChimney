#ifndef Q_CAL_RX_ATT_TABWIDGET_H
#define Q_CAL_RX_ATT_TABWIDGET_H

#include "rddt_plot.hpp"
#include "rddt_tableview.hpp"
#include <QTabWidget>

struct CalParam;

class QwtPlotCurve;
class QR1CRxAttOPModel;
class QR1CRxAttIOModel;
class QwtRxAttOPData;
class QwtRxAttIOData;

class QCalR1CRXAttTabWidget : public QTabWidget
{
public:
    QCalR1CRXAttTabWidget(QWidget *parent = nullptr);
    void init(QR1CRxAttOPModel *model_OP,QR1CRxAttIOModel *model_IO);
    void resetShowWidget(CalParam *param);

public:
    Q_RDDT_CalPlot *plotOP;
    Q_RDDT_CalPlot *plotIO;
    QVector<QwtPlotCurve *> *curveOP;
    QVector<QwtPlotCurve *> *curveIO;
    Q_RDDT_TableView *tableViewOP;
    Q_RDDT_TableView *tableViewIO;
    QR1CRxAttOPModel *modelOP;
    QR1CRxAttIOModel *modelIO;
};

#endif // Q_CAL_RX_ATT_TABWIDGET_H
