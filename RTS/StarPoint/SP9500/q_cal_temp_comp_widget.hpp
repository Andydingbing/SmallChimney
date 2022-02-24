#ifndef Q_CAL_TEMP_COMP_WIDGET_HPP
#define Q_CAL_TEMP_COMP_WIDGET_HPP

#include "rddt_plot.hpp"

struct CalParam;
class QwtTXTempPropertyData;
class QwtTempCompData;
class QwtPlotCurve;
class QLineEdit;

typedef class QCalTxTempCompWidget : public QWidget
{
public:
    explicit QCalTxTempCompWidget(QWidget *parent = nullptr);
    ~QCalTxTempCompWidget();
    void resetShowWidget(CalParam *param);
    void updateCoef();
    QwtTXTempPropertyData *dataProperty() { return _dataProperty; }
    QwtTempCompData *dataComp() { return _dataComp; }
    Q_RDDT_CalPlot *plot() { return _plot; }

private:
    QwtTXTempPropertyData *_dataProperty;
    QwtTempCompData *_dataComp;
    QwtPlotCurve *_curveProperty;
    QwtPlotCurve *_curveComp;
    Q_RDDT_CalPlot *_plot;
    QLineEdit *_lineEditPolynomail;
} QCalRXTempCompWidget;

#endif // Q_CAL_TEMP_COMP_WIDGET_HPP
