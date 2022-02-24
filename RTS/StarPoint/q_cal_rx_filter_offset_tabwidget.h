#ifndef Q_CAL_RX_FILTER_OFFSET_TABWIDGET_H
#define Q_CAL_RX_FILTER_OFFSET_TABWIDGET_H

#include "rddt_plot.hpp"
#include "rddt_tableview.hpp"
#include <QTabWidget>

struct CalParam;
class QwtPlotCurve;
class QRXFilterOffsetModel;
class QwtRxFilterOffsetData;

class QCalRXFilterOffsetWidget : public QWidget
{
    friend class QCalRXFilterOffsetTabWidget;
    friend class QCalR1CRXFilterOffsetDlg;

public:
    explicit QCalRXFilterOffsetWidget(QWidget *parent = nullptr);
    void init(QRXFilterOffsetModel *model);
    void resetShowWidget(qint32 pts, qint32 sec);

private:
    Q_RDDT_CalPlot *plot;
    QVector<QwtPlotCurve *> *curve;
    Q_RDDT_TableView *tableView;
    QRXFilterOffsetModel *model;
};


class QCalRXFilterOffsetTabWidget : public QTabWidget
{
    friend class QCalR1CRXFilterOffsetDlg;

public:
    explicit QCalRXFilterOffsetTabWidget(QWidget *parent = nullptr);
    void init(QRXFilterOffsetModel *model);
    void resetShowWidget(CalParam *param);

private:
    QCalRXFilterOffsetWidget *widget_80;
};

#endif // Q_CAL_RX_FILTER_OFFSET_TABWIDGET_H
