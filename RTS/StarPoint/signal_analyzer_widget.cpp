#include "signal_analyzer_widget.h"
#include "ui_signal_analyzer.h"
#include "sp9500_iq_cap_config_widget.h"
#include "dt3308_iq_cap_config_widget.h"
#include "sp9500x_iq_cap_config_widget.h"
#include "signal_analyzer_freq_widget.h"
#include "qwt_plot_canvas.h"
#include "qwt_plot_grid.h"
#include "qwt_plot_picker.h"
#include "qwt_plot_zoomer.h"
#include "qwt_scale_div.h"
#include <QStackedWidget>
#include <QFileDialog>
#include <QComboBox>
#include <QTableView>
#include "mainwindow.h"

Q_SA_Widget::Q_SA_Widget(QWidget *parent) :
    Q_RD_Widget(parent),
    ui(new Ui::Q_SA_Widget)
{
    ui->setupUi(this);

    widget_Config[SP9500]  = new NS_SP9500::Q_IQ_Cap_Config_Widget_Helper(this);
    widget_Config[DT3308]  = new NS_DT3308::Q_IQ_Cap_Config_Widget_Helper(this);
    widget_Config[SP9500X] = new NS_SP9500X::Q_IQ_Cap_Config_Widget_Helper(this);
    tableView_Config = new Q_Config_Table_View(ui->tab);

    widget_Freq = new Q_SA_Freq_Widget_Helper(this);
    tableView_Freq = new Q_Config_Table_View(ui->tab);

    ui->tab->addTab(tableView_Config,QString("Config"));
    ui->tab->addTab(tableView_Freq,QString("Frequency"));

    ui->plot->init();


//    _data_Magnitude._sequence = &_finalSequence;

//    curveMagnitude = new QwtPlotCurve();
//    curveMagnitude->setPen(QColor(Qt::yellow));
//    curveMagnitude->setVisible(true);;
//    curveMagnitude->attach(ui->plot);
//    curveMagnitude->setSamples(&_data_Magnitude);

    curveI = new QwtPlotCurve("I");
    curveI->setPen(QColor(Qt::red));
    curveI->setVisible(false);
    curveI->attach(ui->plot);
    curveI->setSamples(&_data_I);

    curveQ = new QwtPlotCurve("Q");
    curveQ->setPen(QColor(Qt::green));
    curveQ->setVisible(false);
    curveQ->attach(ui->plot);
    curveQ->setSamples(&_data_Q);

    curveDFT = new QwtPlotCurve("DFT");
    curveDFT->setPen(QColor(Qt::yellow));
    curveDFT->setVisible(true);
    curveDFT->attach(ui->plot);
    curveDFT->setSamples(&_data_DFT);
}

Q_SA_Widget::~Q_SA_Widget()
{
    delete ui;
}

void Q_SA_Widget::plotToTimeDomain()
{
    ui->plot->setAxisScale(QwtPlot::xBottom,0.0,double(complexSequence->samples()));
    ui->plot->setAxisScale(QwtPlot::yLeft,-32768.0 - 1000.0,32768.0 + 1000.0);
    ui->plot->replot();
}

void Q_SA_Widget::plotToFreqDomain()
{
    ui->plot->setAxisScale(QwtPlot::xBottom,complexSequence->sr() / 1e6 / -2.0,complexSequence->sr() / 1e6 / 2.0);
    ui->plot->setAxisScale(QwtPlot::yLeft,-180.0,10.0);
    ui->plot->replot();
//    ui->plot->setAxisScale(QwtPlot::xBottom,ns_sp1401::rx_freq_star - FREQ_M(50),ns_sp1401::rx_freq_stop + FREQ_M(50));
//    ui->plot->setAxisScale(QwtPlot::yLeft,-110.0,10.0);
//    ui->plot->replot();
}

void Q_SA_Widget::updatePlotFromBuf()
{
    if (g_MainW == nullptr) {
        return;
    }

    quint32 samples = complexSequence->samples();

    if (project == SP9500) {
        INT_CHECKV(SP3301->iq_cap_iq2buf(RFIdx,samples));
    } else if (project == DT3308) {
        DT3308_BB_F->ddr()->iq2buf(complexSequence->i(),complexSequence->q());
    }

    if (showingDFT) {
        complexSequence->dft();
    }

    ui->plot->replot();
}

void Q_SA_Widget::init()
{
    widget_Config[project]->init();
    widget_Freq->init();

    plotToFreqDomain();
    showingDFT = true;
}

void Q_SA_Widget::updatePlot()
{
    if (this->isVisible()) {
        updatePlotFromBuf();
    }
}

void Q_SA_Widget::on_pushButtonCap_clicked()
{
    if (project == SP9500) {
        INT_CHECKV(SP3301->iq_cap(RFIdx));
    } else if (project == DT3308) {
        INT_CHECKV(DT3308_BB_F->ddr()->fpga_w());
    } else if (project == SP9500X) {
        INT_CHECKV(NS_SP9500X::SP2406->set_iq_cap_abort());
        INT_CHECKV(NS_SP9500X::SP2406->set_iq_cap_start());
//        INT_CHECKV(NS_SP9500X::SP2406->ddr()->dump("c:\\iq.txt"));
    }
    updatePlotFromBuf();
}


void Q_SA_Widget::on_pushButtonAbort_clicked()
{
    if (project == SP9500X) {
        NS_SP9500X::SP2406->set_iq_cap_abort();
    }
}

//void Q_SA_Widget::on_pushButtonSetPath_clicked()
//{
//    QString path = QFileDialog::getOpenFileName(this,tr("Select Target File"),"","");
//    if (!path.isEmpty()) {
//        ui->lineEditPath->setText(path);
//    }
//}

void Q_SA_Widget::on_pushButtonShowTD_clicked()
{
    if (showingDFT) {
        curveDFT->setVisible(false);
        ui->checkBoxMaxHold->setEnabled(false);
        ui->checkBoxMinHold->setEnabled(false);

        ui->checkBoxShowI->setEnabled(true);
        ui->checkBoxShowQ->setEnabled(true);
        curveI->setVisible(ui->checkBoxShowI->isChecked());
        curveQ->setVisible(ui->checkBoxShowQ->isChecked());
    }
    plotToTimeDomain();
    showingDFT = false;
    updatePlotFromBuf();
}

void Q_SA_Widget::on_pushButtonShowDFT_clicked()
{
    if (!showingDFT) {
        curveI->setVisible(false);
        curveQ->setVisible(false);
        ui->checkBoxShowI->setEnabled(false);
        ui->checkBoxShowQ->setEnabled(false);

        ui->checkBoxMaxHold->setEnabled(true);
        ui->checkBoxMinHold->setEnabled(true);
        curveDFT->setVisible(true);
//        curveMagnitude->setVisible(true);
    }
    plotToFreqDomain();
    showingDFT = true;
    updatePlotFromBuf();
}

void Q_SA_Widget::on_checkBoxShowI_clicked(bool checked)
{
    curveI->setVisible(checked);
    ui->plot->replot();
}

void Q_SA_Widget::on_checkBoxShowQ_clicked(bool checked)
{
    curveQ->setVisible(checked);
    ui->plot->replot();
}

void Q_SA_Widget::on_checkBoxMaxHold_clicked()
{
//    _finalSequence.trace_set_hold(trace_hold_t::Max_Hold);
    if (ui->checkBoxMaxHold->isChecked()) {
        ui->checkBoxMinHold->setChecked(false);
        complexSequence->trace_set_hold(trace_hold_t::Max_Hold);
    } else {
        complexSequence->trace_set_hold(trace_hold_t::Normal);
    }
}

void Q_SA_Widget::on_checkBoxMinHold_clicked()
{
    if (ui->checkBoxMinHold->isChecked()) {
        ui->checkBoxMaxHold->setChecked(false);
        complexSequence->trace_set_hold(trace_hold_t::Min_Hold);
    } else {
        complexSequence->trace_set_hold(trace_hold_t::Normal);
    }
}

void Q_SA_Widget::sweepOnce()
{
    quint64 freq = 0;
    quint32 sample = complexSequence->samples();
    quint32 sampleInBand = sample * 10000 / 24576;
    quint32 seg_idx = 0;

    _finalSequence._new(SERIE_SIZE(ns_sp1401::rx_freq_star,ns_sp1401::rx_freq_stop,FREQ_M(100)) * sampleInBand);

    for (quint32 i = 0;i < _finalSequence.samples();++i) {
        _finalSequence._normalized_freq[i] = ns_sp1401::rx_freq_star + i * complexSequence->rbw();
        _finalSequence._magnitude[i] = double(_finalSequence.samples() - i) / -100.0;
    }

//    SP3301->rf_set_rx_freq(RFIdx,FREQ_G(2));
//    SP3301->rf_set_rx_level(RFIdx,-10.0);
    SP3301->set_iq_cap_samples(RFIdx,sample);

    for (freq = ns_sp1401::rx_freq_star;freq <= ns_sp1401::rx_freq_stop;freq += FREQ_M(100)) {
        SP1401->set_rx_freq(freq);

        SP3301->iq_cap(RFIdx);
        SP3301->iq_cap_iq2buf(RFIdx,sample);
        complexSequence->dft();

        memcpy(_finalSequence.magnitude() + seg_idx * sampleInBand,
               complexSequence->magnitude(-50e6),sampleInBand * sizeof(double));
        ++seg_idx;
    }
    ui->plot->replot();
}
