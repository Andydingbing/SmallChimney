#include "qwt_plot_helper.h"
#include "qwt_plot.h"
#include "qwt_plot_grid.h"
#include "qwt_plot_curve.h"
#include "qwt_plot_canvas.h"
#include "qwt_plot_picker.h"
#include "qwt_plot_zoomer.h"
#include "qwt_scale_div.h"
#include "qwt_text.h"
#include <QPen>

void QwtPlotHelper::init(QwtPlot *plot)
{
    QwtPlotCanvas *canvas = new QwtPlotCanvas;
    canvas->setFocusIndicator(QwtPlotCanvas::CanvasFocusIndicator);
    canvas->setFocusPolicy(Qt::StrongFocus);
    canvas->setPalette(Qt::white);
    plot->setCanvas(canvas);

    QwtPlotGrid *grid = new QwtPlotGrid();
    grid->enableXMin(false);
    grid->enableYMin(false);
    grid->setMajorPen(Qt::gray,0,Qt::DotLine);
    grid->attach(plot);

    QwtPlotPicker *picker = new QwtPlotPicker(canvas);

#if QWT_VERSION < 0x060200
    picker->setAxis(QwtPlot::xBottom,QwtPlot::yLeft);
#else
    picker->setAxes(QwtPlot::xBottom,QwtPlot::yLeft);
#endif

    picker->setRubberBand(QwtPicker::CrossRubberBand);
    picker->setTrackerMode(QwtPicker::AlwaysOn);
    picker->setTrackerPen(QColor(Qt::black));
    picker->setRubberBandPen(QColor(Qt::black));

    QwtPlotZoomer *zoomer = new QwtPlotZoomer(canvas);
    zoomer->setTrackerMode(QwtPicker::AlwaysOff);
    zoomer->setRubberBandPen(QColor(Qt::black));
    zoomer->setMousePattern(QwtEventPattern::MouseSelect2,Qt::RightButton,Qt::ControlModifier);
    zoomer->setMousePattern(QwtEventPattern::MouseSelect3,Qt::RightButton);

    QwtText title = plot->title();

    QFont titleFont = title.font();
    titleFont.setPointSize(12);
    titleFont.setBold(false);

    title.setFont(titleFont);
    title.setColor(QColor(Qt::blue));
    plot->setTitle(title);

    plot->setFrameShape(QFrame::StyledPanel);
    plot->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
}

void QwtPlotHelper::setXAxisMinMax(QwtPlot *plot,double min,double max)
{
    if (min - max != 0.0) {
        plot->setAxisScale(QwtPlot::xBottom,min,max);
    } else {
        plot->setAxisAutoScale(QwtPlot::xBottom);
    }
}

void QwtPlotHelper::setYAxisMinMax(QwtPlot *plot,double min,double max)
{
    if (min - max != 0.0) {
        plot->setAxisScale(QwtPlot::yLeft,min,max);
    } else {
        plot->setAxisAutoScale(QwtPlot::yLeft);
    }
}

void QwtPlotHelper::init(QwtPlot *plot,double xMin,double xMax,double yMin,double yMax)
{
    setXAxisMinMax(plot,xMin,xMax);
    setYAxisMinMax(plot,yMin,yMax);
    init(plot);
}


//void Q_SA_Plot::init()
//{
//    Q_RDDT_Plot::init();

//    canvas->setPalette(Qt::black);
//    picker->setRubberBandPen(QColor(Qt::white));
//    picker->setTrackerPen(QColor(Qt::green));
//    zoomer->setRubberBandPen(QColor(Qt::white));
//}
