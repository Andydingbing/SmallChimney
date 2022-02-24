#ifndef Q_CAL_RX_REF_WIDGET_H
#define Q_CAL_RX_REF_WIDGET_H

#include "rddt_plot.hpp"
#include "rddt_tableview.hpp"

struct CalParam;

class QwtPlotCurve;
class QR1CRXRefModel;

class QCalR1CRXRefWidget : public QWidget
{
public:
    QCalR1CRXRefWidget(QWidget *parent = nullptr);
    void init(QR1CRXRefModel *model_OP,QR1CRXRefModel *model_IO);
    void resetShowWidget(CalParam *param);

private:
    void resetCurve(QR1CRXRefModel *model,
                    QVector<QwtPlotCurve *> *curve,
                    quint32 pts,
                    qint32 sec,
                    QColor color);

public:
    Q_RDDT_CalPlot *plotDGain;

    QVector<QwtPlotCurve *> *curveOP;
    QVector<QwtPlotCurve *> *curveIO;

    Q_RDDT_TableView *tableViewOP;
    Q_RDDT_TableView *tableViewIO;
    QR1CRXRefModel *modelOP;
    QR1CRXRefModel *modelIO;
};

#endif // Q_CAL_RX_REF_WIDGET_H
