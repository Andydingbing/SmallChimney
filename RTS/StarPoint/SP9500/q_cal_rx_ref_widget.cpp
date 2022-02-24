#include "q_cal_rx_ref_widget.h"
#include "q_model_rx_ref.h"
#include "q_winthread.h"
#include "freq_string.hpp"
#include <QHBoxLayout>
#include <QVBoxLayout>

using namespace rd::ns_sp1401;

QCalR1CRXRefWidget::QCalR1CRXRefWidget(QWidget *parent) :
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
    plotDGain->init(rx_freq_star / 1e6,rx_freq_stop / 1e6,-1.0,2.2);
    QwtText titleplotDGain = plotDGain->title();
    titleplotDGain.setText(tr("Ref's Digital Gain Curve(X:Freq(MHz) Y:Digital Gain(dB))"));
    titleplotDGain.setColor(QColor(Qt::blue));
    plotDGain->setTitle(titleplotDGain);
    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(plotDGain->sizePolicy().hasHeightForWidth());
    plotDGain->setTitle(titleplotDGain);
    plotDGain->setSizePolicy(sizePolicy);
    plotDGain->setMinimumSize(QSize(0,150));
    plotDGain->setMaximumSize(QSize(16777215,150));
    plotDGain->setBaseSize(QSize(0,0));

    curveOP = new QVector<QwtPlotCurve *>[3];
    curveIO = new QVector<QwtPlotCurve *>[3];

    layout->addWidget(plotDGain);
    layout->setStretch(0,1);
}

void QCalR1CRXRefWidget::init(QR1CRXRefModel *model_OP,QR1CRXRefModel *model_IO)
{
    modelOP = model_OP;
    modelIO = model_IO;

    tableViewOP->setModel(modelOP);
    tableViewOP->setColumnWidth(0,80);
    tableViewOP->setColumnWidth(1,70);
    tableViewOP->setColumnWidth(2,60);
    tableViewOP->setColumnWidth(3,60);
    tableViewOP->setColumnWidth(4,40);
    tableViewOP->setColumnWidth(5,40);
    tableViewOP->setColumnWidth(6,40);
    tableViewOP->setColumnWidth(7,100);
    tableViewOP->setColumnWidth(8,100);
    tableViewOP->setColumnWidth(9,100);
    tableViewOP->setColumnWidth(10,100);
    tableViewOP->setColumnWidth(11,100);
    tableViewOP->setColumnWidth(12,120);

    tableViewIO->setModel(modelIO);
    tableViewIO->setColumnWidth(0,80);
    tableViewIO->setColumnWidth(1,70);
    tableViewIO->setColumnWidth(2,60);
    tableViewIO->setColumnWidth(3,60);
    tableViewIO->setColumnWidth(4,40);
    tableViewIO->setColumnWidth(5,40);
    tableViewIO->setColumnWidth(6,40);
    tableViewIO->setColumnWidth(7,100);
    tableViewIO->setColumnWidth(8,100);
    tableViewIO->setColumnWidth(9,100);
    tableViewIO->setColumnWidth(10,100);
    tableViewIO->setColumnWidth(11,100);
    tableViewIO->setColumnWidth(12,120);
}

void QCalR1CRXRefWidget::resetShowWidget(CalParam *param)
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
        tableViewOP->selectRow(0);

    }
    if (calIO(param->mode)) {
        resetCurve(modelIO,curveIO,freqRange.freqs.size(),sections,QColor(Qt::green));
        tableViewIO->selectRow(0);
    }
    plotDGain->replot();
}

void QCalR1CRXRefWidget::resetCurve(QR1CRXRefModel *model,
                                    QVector<QwtPlotCurve *> *curve,
                                    quint32 pts,
                                    qint32 sec,
                                    QColor color)
{
    QColor clrTable[] = {
        QColor(255,0,0),
        QColor(0,150,0),
        QColor(0,0,255),
        QColor(255,255,0),
        QColor(0,255,255),
        QColor(255,128,0),
        QColor(128,0,128),
        QColor(128,128,0),
        QColor(255,0,255),
        QColor(64,128,128)
    };

    int clrTableSize = ARRAY_SIZE(clrTable);
    int n = 0;
    if(color == Qt::green)
    {
       n = 3;
    }
    rx_ref_table_r1cd::data_f_t data;

    model->resetData();
    for (quint32 i = 0;i < pts;i ++) {
        model->calTable()->append(data);
    }

    for (qint8 i = 0;i < 3;i ++) {
        for (int j = 0;j < curve[i].size();j ++)
            curve[i].at(j)->detach();
        curve[i].clear();

        for (int j = 0;j < sec;j ++) {
            if(color == Qt::green)
            {
                n ++;
            }
            model->iterTable(i)->append(new QwtRXRefData(i));
            curve[i].append(new QwtPlotCurve);
            curve[i].at(j)->setPen(clrTable[(i+n) % clrTableSize],2);
            curve[i].at(j)->attach(plotDGain);
            curve[i].at(j)->setSamples(model->iterTable(i)->at(j));
        }
    }

}
