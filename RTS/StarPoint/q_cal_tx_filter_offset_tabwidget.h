#ifndef Q_CAL_TX_FILTER_OFFSET_TABWIDGET_H
#define Q_CAL_TX_FILTER_OFFSET_TABWIDGET_H

#include "q_model_filter_offset.h"
#include "rddt_plot.hpp"
#include "rddt_tableview.hpp"
#include <QTabWidget>

struct CalParam;
class QwtPlotCurve;

class QCalTXFilterOffsetWidget : public QWidget
{
    friend class QCalTXFilterOffsetTabWidget;
    friend class QCalR1CTXFilterOffsetDlg;

public:
    explicit QCalTXFilterOffsetWidget(QWidget *parent = nullptr);
    void init(QTXFilterOffsetModel *model);
    void resetShowWidget(qint32 pts, qint32 sec);

private:
    Q_RDDT_CalPlot *plot;
    QVector<QwtPlotCurve *> *curve;
    Q_RDDT_TableView *tableView;
    QTXFilterOffsetModel *model;
};


class QCalTXFilterOffsetTabWidget : public QTabWidget
{
    friend class QCalR1CTXFilterOffsetDlg;

public:
    explicit QCalTXFilterOffsetTabWidget(QWidget *parent = nullptr);
    void init(QTXFilterOffsetModel *modelOP);
    void resetShowWidget(CalParam *param);

private:
    QCalTXFilterOffsetWidget *widget_80;
};

#endif // Q_CAL_TX_FILTER_OFFSET_TABWIDGET_H
