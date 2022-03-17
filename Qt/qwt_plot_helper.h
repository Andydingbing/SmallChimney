#ifndef QWT_PLOT_HELPER_H
#define QWT_PLOT_HELPER_H

#include "qhelper_global.h"
#include "preprocessor/prefix.h"
#include "qwt_plot.h"

class QHELPER_EXPORT QwtPlotHelper
{
public:
    static void init(QwtPlot *plot);

    static void setXAxisMinMax(QwtPlot *plot,double min,double max);

    static void setYAxisMinMax(QwtPlot *plot,double min,double max);

    static void init(QwtPlot *plot,double xMin,double xMax,double yMin,double yMax);

    template<typename T_x,typename T_y>
    static void init(QwtPlot *plot,const T_x xMin,const T_x xMax,const T_y yMin,const T_y yMax)
    { return init(plot,double(xMin),double(xMax),double(yMin),double(yMax)); }
};

SYM_INLINE static void initPlot(QwtPlot *plot)
{ return QwtPlotHelper::init(plot); }

SYM_INLINE static void setXAxisMinMax(QwtPlot *plot,double min,double max)
{ return QwtPlotHelper::setXAxisMinMax(plot,min,max); }

SYM_INLINE static void setYAxisMinMax(QwtPlot *plot,double min,double max)
{ return QwtPlotHelper::setYAxisMinMax(plot,min,max); }

SYM_INLINE static void initPlot(QwtPlot *plot,double xMin,double xMax,double yMin,double yMax)
{ return QwtPlotHelper::init(plot,xMin,xMax,yMin,yMax); }

template<typename T_x,typename T_y>
SYM_INLINE static void initPlot(QwtPlot *plot,const T_x xMin,const T_x xMax,const T_y yMin,const T_y yMax)
{ return QwtPlotHelper::init<T_x,T_y>(plot,xMin,xMax,yMin,yMax); }

#endif
