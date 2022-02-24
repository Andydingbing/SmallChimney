#ifndef Q_SA_Widget_H
#define Q_SA_Widget_H

#include "global.h"
#include "qwt_plot_curve.h"

class Qwt_I_Data : public Qwt_Data
{
public:
    Qwt_I_Data() : Qwt_Data() { _sequence = nullptr; }

    size_t size() const
    { return _sequence != nullptr ? _sequence->samples() : 0; }

    QPointF sample(size_t idx) const
    { return QPointF(idx,_sequence->i(idx)); }

public:
    complex_sequence *_sequence;
};

class Qwt_Q_Data : public Qwt_Data
{
public:
    Qwt_Q_Data() : Qwt_Data() { _sequence = nullptr; }

    size_t size() const
     { return _sequence != nullptr ? _sequence->samples() : 0; }

    QPointF sample(size_t idx) const
    { return QPointF(idx,_sequence->q(idx)); }

public:
    complex_sequence *_sequence;
};

class Qwt_DFT_Data : public Qwt_Data
{
public:
    Qwt_DFT_Data() : Qwt_Data() { _sequence = nullptr; }

    size_t size() const
     { return _sequence != nullptr ? _sequence->samples() : 0; }

    QPointF sample(size_t idx) const
    { return QPointF(_sequence->normalized_freq(idx),_sequence->magnitude(idx)); }

public:
    complex_sequence *_sequence;
};


namespace Ui {
class Q_SA_Widget;
}

class Qwt_I_Data;
class Qwt_Q_Data;
class Qwt_DFT_Data;
class Q_IQ_Cap_Config_Widget_Helper;
class Q_SA_Freq_Widget_Helper;

class Q_SA_Widget : public Q_RD_Widget
{
    Q_OBJECT

    friend class Q_IQ_Cap_Config_Widget_Helper;
    friend class Q_SA_Freq_Widget_Helper;

public:
    explicit Q_SA_Widget(QWidget *parent = nullptr);
    ~Q_SA_Widget();

    void plotToTimeDomain();
    void plotToFreqDomain();

    void updatePlotFromBuf();
    void sweepOnce();

public slots:
    void init();
    void updatePlot();

private slots:
    void on_pushButtonCap_clicked();

    void on_pushButtonAbort_clicked();

    void on_pushButtonShowTD_clicked();

    void on_pushButtonShowDFT_clicked();

    void on_checkBoxShowI_clicked(bool checked);

    void on_checkBoxShowQ_clicked(bool checked);

    void on_checkBoxMaxHold_clicked();

    void on_checkBoxMinHold_clicked();

public:
    complex_sequence _finalSequence;
    Qwt_DFT_Data _data_Magnitude;
    QwtPlotCurve *curveMagnitude;

    bool showingDFT;

    Qwt_I_Data _data_I;
    Qwt_Q_Data _data_Q;
    Qwt_DFT_Data _data_DFT;

    QwtPlotCurve *curveI;
    QwtPlotCurve *curveQ;
    QwtPlotCurve *curveDFT;

    Q_Config_Table_View *tableView_Config;
    Q_Config_Table_View *tableView_Freq;
    Q_IQ_Cap_Config_Widget_Helper *widget_Config[PROJECTS];
    Q_SA_Freq_Widget_Helper *widget_Freq;

    Ui::Q_SA_Widget *ui;
};

#endif // Q_SA_Widget_H
