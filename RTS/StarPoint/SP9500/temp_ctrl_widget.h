#ifndef STARPOINT_SP9500_TEMP_CTRL_WIDGET_H
#define STARPOINT_SP9500_TEMP_CTRL_WIDGET_H

#include "global.h"
#include "temp_ctrl_thread.h"

#include "ui_temp_ctrl_widget.h"

class QwtPlotCurve;
class QwtLegend;
class QwtTempData;

class QTempCtrlDlg : public Q_Widget
{
    Q_OBJECT

public slots:
    void update(double *gettemp);

public:
    QVector<QwtPlotCurve *> curveR1ATx;
    QVector<QwtPlotCurve *> curveR1ARx;
    QVector<QVector<QwtPlotCurve *> *> curveR1C;
    QVector<QVector<QwtTempData *> *> dataR1C;

    QR1CTempCtrlThread::Param param;
    QwtLegend *d_externalLegend;

public:
    explicit QTempCtrlDlg(QWidget *parent = nullptr);
    ~QTempCtrlDlg();

public:
    void initAllSlider(int max);

private slots:
    void on_verticalSlider1_valueChanged(int value);

    void on_verticalSlider2_valueChanged(int value);

    void on_verticalSlider3_valueChanged(int value);

    void on_verticalSlider4_valueChanged(int value);

    void on_verticalSlider5_valueChanged(int value);

    void on_verticalSlider6_valueChanged(int value);

    void on_verticalSlider7_valueChanged(int value);

    void on_verticalSlider8_valueChanged(int value);

    void on_verticalSlider9_valueChanged(int value);

    void on_verticalSlider10_valueChanged(int value);

    void on_verticalSlider11_valueChanged(int value);

    void on_verticalSlider12_valueChanged(int value);

    void on_pushButtonStart_clicked();

    void on_pushButton_clicked();

private:
    Ui::Q_Temp_Ctrl_Widget *ui;
};

extern QTempCtrlDlg *dlgTempCtrl;

#endif
