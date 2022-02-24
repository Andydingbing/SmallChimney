#include "q_cal_tx_pwr_widget.h"
#include "q_cal_dlg.h"
#include "q_model_tx_pwr.h"
#include "q_winthread.h"
#include "freq_string.hpp"
#include <QHBoxLayout>
#include <QVBoxLayout>

using namespace rd::ns_sp1401;

QCalR1CTXPwrWidget::QCalR1CTXPwrWidget(QWidget *parent) :
    QWidget(parent),
    modelOP(nullptr),
    modelIO(nullptr)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    QHBoxLayout *tableViewLayout = new QHBoxLayout();

    tableViewOP = new Q_RDDT_TableView(this);
    tableViewIO = new Q_RDDT_TableView(this);

    tableViewLayout->addWidget(tableViewOP);
    tableViewLayout->addWidget(tableViewIO);
    tableViewLayout->setStretch(0,1);
    tableViewLayout->setStretch(1,1);

    layout->addLayout(tableViewLayout);

    plotDGain = new Q_RDDT_CalPlot(this);
    plotDGain->init(tx_freq_star / 1e6,tx_freq_stop / 1e6,R1C_DGAIN_MIN + 2.0,R1C_DGAIN_MAX - 5.0);
    QwtText titleplotDGain = plotDGain->title();
    titleplotDGain.setText(tr("Base Power's Digital Gain Curve(X:Freq(MHz) Y:Digital Gain(dB))"));
    titleplotDGain.setColor(QColor(Qt::blue));
    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(plotDGain->sizePolicy().hasHeightForWidth());
    plotDGain->setTitle(titleplotDGain);
    plotDGain->setSizePolicy(sizePolicy);
    plotDGain->setMinimumSize(QSize(0,150));
    plotDGain->setMaximumSize(QSize(16777215,150));
    plotDGain->setBaseSize(QSize(0,0));

    curveOP = new QVector<QwtPlotCurve *>;
    curveIO = new QVector<QwtPlotCurve *>;

    layout->addWidget(plotDGain);
    layout->setStretch(0,1);
}

void QCalR1CTXPwrWidget::init(QTXPwrModel *model_OP,QTXPwrModel *model_IO)
{
    modelOP = model_OP;
    modelIO = model_IO;

    tableViewOP->setModel(modelOP);
    tableViewOP->setColumnWidth(0,80);
    tableViewOP->setColumnWidth(1,70);
    tableViewOP->setColumnWidth(2,40);
    tableViewOP->setColumnWidth(3,40);
    tableViewOP->setColumnWidth(4,40);
    tableViewOP->setColumnWidth(5,40);
    tableViewOP->setColumnWidth(6,100);
    tableViewOP->setColumnWidth(7,100);
    tableViewOP->setColumnWidth(8,100);
    tableViewOP->setColumnWidth(9,100);
    tableViewOP->setColumnWidth(10,120);

    tableViewIO->setModel(modelIO);
    tableViewIO->setColumnWidth(0,80);
    tableViewIO->setColumnWidth(1,70);
    tableViewIO->setColumnWidth(2,40);
    tableViewIO->setColumnWidth(3,40);
    tableViewIO->setColumnWidth(4,40);
    tableViewIO->setColumnWidth(5,40);
    tableViewIO->setColumnWidth(6,100);
    tableViewIO->setColumnWidth(7,100);
    tableViewIO->setColumnWidth(8,100);
    tableViewIO->setColumnWidth(9,100);
    tableViewIO->setColumnWidth(10,120);
}

void QCalR1CTXPwrWidget::resetShowWidget(CalParam *param)
{
    range_freq_string freqString;
    range_freq<quint64> freqRange;
    freqString.star = param->rfFreqStar.toStdString();
    freqString.stop = param->rfFreqStop.toStdString();
    freqString.step = param->rfFreqStep.toStdString();

    parse_range_freq_string(freqString,freqRange);

    qint32 sections = qint32(freqRange.star.size());

    if (calOP(param->mode)) {
        resetCurve(modelOP,curveOP,freqRange.freqs.size(),sections,QColor(Qt::red));
        if (sections > 0) {
            curveOP->at(0)->setTitle(QString("OP@%1dBm").arg(r1c::tx_base_pwr_op));
        }
        tableViewOP->selectRow(0);
    }
    if (calIO(param->mode)) {
        resetCurve(modelIO,curveIO,freqRange.freqs.size(),sections,QColor(Qt::green));
        if (sections > 0) {
            curveIO->at(0)->setTitle(QString("IO@%1dBm").arg(r1c::tx_base_pwr_io));
        }
        tableViewIO->selectRow(0);
    }

    hw_ver_t RFVer = SP1401->get_hw_ver();

    if (RFVer == R1C || RFVer == R1D || RFVer == R1E) {
        plotDGain->setYAxisMinMax(R1C_DGAIN_MIN + 2.0,R1C_DGAIN_MAX - 5.0);
    } else if (RFVer == R1F) {
        plotDGain->setYAxisMinMax(R1F_DGAIN_MIN + 2.0,R1F_DGAIN_MAX - 5.0);
    }
    plotDGain->replot();
}

void QCalR1CTXPwrWidget::resetCurve(QTXPwrModel *model,
                                    QVector<QwtPlotCurve *> *curve,
                                    quint32 pts,
                                    qint32 sec,
                                    QColor color)
{
    tx_pwr_table_r1c::data_f_t data;

    model->resetData();
    for (qint32 i = 0;i < curve->size();i ++) {
        curve->at(i)->detach();
    }
    curve->clear();

    for (quint32 i = 0;i < pts;i ++) {
        model->calTable()->append(data);
    }

    for (int i = 0;i < sec;i ++) {
        model->iterTable()->append(new QwtTXPwrData);
        curve->append(new QwtPlotCurve);
        curve->at(i)->setPen(QColor(color),2);
        curve->at(i)->attach(plotDGain);
        curve->at(i)->setSamples(model->iterTable()->at(i));
    }
}
