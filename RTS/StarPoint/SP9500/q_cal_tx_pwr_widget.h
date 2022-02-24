#ifndef Q_CAL_TX_PWR_WIDGET_H
#define Q_CAL_TX_PWR_WIDGET_H

#include "rddt_plot.hpp"
#include "rddt_tableview.hpp"
#include <QWidget>

struct CalParam;

class QwtPlotCurve;
class QTXPwrModel;

class QCalR1CTXPwrWidget : public QWidget
{
public:
    QCalR1CTXPwrWidget(QWidget *parent = nullptr);
    void init(QTXPwrModel *model_OP,QTXPwrModel *model_IO);
    void resetShowWidget(CalParam *param);

private:
    void resetCurve(QTXPwrModel *model,
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
    QTXPwrModel *modelOP;
    QTXPwrModel *modelIO;
};

#endif // Q_CAL_TX_PWR_WIDGET_H
