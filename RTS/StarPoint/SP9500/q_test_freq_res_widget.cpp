#include "q_test_dlg.h"
#include "q_test_freq_res_widget.h"
#include "q_test_freq_res_thread.h"
#include "q_model_temp_pwr.hpp"
#include "global.h"
#include <QVBoxLayout>

using namespace ns_sp1401;
using namespace ns_sp2401;
using namespace std;

QTestFreqResWidget::QTestFreqResWidget(QWidget *parent) :
    QWidget(parent)
{
    plotRF = new Q_RDDT_TestPlot(this);
    plotRF->init(tx_freq_star / 1e6,tx_freq_stop / 1e6, -15.0,5.0);

    QwtText title = plotRF->title();
    title.setText(tr("RF T/RX Freq Response(X:Freq(MHz) Y:Power(dBm))"));
    plotRF->setTitle(title);
    plotRF->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);

    plotIF = new Q_RDDT_TestPlot(this);
    plotIF->init(dl_filter_160M_freq_star/1e6,dl_filter_160M_freq_stop/1e6,-15.0,5.0);
    title = plotIF->title();
    title.setText(tr("IF T/RX Freq Response(X:Freq(MHz) Y:Power(dBm))"));
    plotIF->setTitle(title);
    plotIF->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);

    dataRF_TX  = new QwtRF_TX_FreqResData;
    curveRF_TX = new QwtPlotCurve;
    curveRF_TX->attach(plotRF);
    curveRF_TX->setPen(QColor(Qt::red),2);

    dataIF_TX  = new QwtIF_TX_FreqResData;
    curveIF_TX = new QwtPlotCurve;
    curveIF_TX->attach(plotIF);
    curveIF_TX->setPen(QColor(Qt::red),2);

    dataRF_RX  = new QwtRF_RX_FreqResData;
    curveRF_RX = new QwtPlotCurve;
    curveRF_RX->attach(plotRF);
    curveRF_RX->setPen(QColor(Qt::green),2);

    dataIF_RX  = new QwtIF_RX_FreqResData;
    curveIF_RX = new QwtPlotCurve;
    curveIF_RX->attach(plotIF);
    curveIF_RX->setPen(QColor(Qt::green),2);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(plotRF);
    layout->addWidget(plotIF);
    layout->setStretch(0,1);
    layout->setStretch(1,1);
}

void QTestFreqResWidget::resetShowWidget(TestBaseParam *param)
{
    TestFreqResParam *p = static_cast<TestFreqResParam *>(param);

    if (p->isTestRF_Tx) {
        SP1401->prepare_tr_rf_tx_freq_res_test();
        dataRF_TX->setReport(SP1401->tr_rf_tx_freq_res_test());
        curveRF_TX->setSamples(dataRF_TX);
    }
    if (p->isTestIF_Tx) {
        SP1401->prepare_tr_if_tx_freq_res_test();
        dataIF_TX->setReport(SP1401->tr_if_tx_freq_res_test());
        curveIF_TX->setSamples(dataIF_TX);
    }
    if (p->isTestRF_Rx) {
        SP1401->prepare_tr_rf_rx_freq_res_test();
        dataRF_RX->setReport(SP1401->tr_rf_rx_freq_res_test());
        curveRF_RX->setSamples(dataRF_RX);
    }
    if (p->isTestIF_Rx) {
        SP1401->prepare_tr_if_rx_freq_res_test();
        dataIF_RX->setReport(SP1401->tr_if_rx_freq_res_test());
        curveIF_RX->setSamples(dataIF_RX);
    }

    if (p->isTestRF_Tx || p->isTestRF_Rx) {
        plotRF->replot();
    }
    if (p->isTestIF_Tx || p->isTestIF_Rx) {
        plotIF->replot();
    }
}


QTestTempPwrWidget::QTestTempPwrWidget(QWidget *parent) :
    QWidget(parent)
{
    plot = new Q_RDDT_TestPlot(this);
    plot->init(double(g_temp_star),double(g_temp_stop), -95.0,20.0);

    QwtText title = plot->title();
    title.setText(tr("RF Temp Property(X:Temp Y:Power(dBm))"));
    plot->setTitle(title);
    plot->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(plot);
}

void QTestTempPwrWidget::resetShowWidget(TestBaseParam *param)
{
    vector<common_atts_t> states;
    TestTempPwrParam *p = static_cast<TestTempPwrParam *>(param);

    if (p->ch == sp1401::CH_TX) {
        SP1401->tx_att_states(states);
    } else {
        SP1401->rx_att_states(states);
    }

    if (data.empty()) {
        for (quint32 i = 0;i < states.size();i ++) {
            QwtTempPwrData *tempData = new QwtTempPwrData;
            data.push_back(tempData);
        }
    }

    if (curve.empty()) {
        for (quint32 i = 0;i < states.size();i ++) {
            QwtPlotCurve *tempCurve = new QwtPlotCurve;
            tempCurve->attach(plot);
            tempCurve->setPen(Qt::red);
            curve.push_back(tempCurve);
        }
    }

    for (int i = 0;i < data.size();i ++) {
        data.at(i)->reset();
        curve.at(i)->setSamples(data.at(i));
    }

    plot->replot();
}
