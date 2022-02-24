#include "q_cal_tx_att_tabwidget.h"
#include "q_model_tx_att.h"
#include "q_winthread.h"
#include "freq_string.hpp"
#include <QHBoxLayout>

using namespace ns_sp1401;

QCalR1CTXAttTabWidget::QCalR1CTXAttTabWidget(QWidget *parent) :
    QTabWidget(parent),
    modelOP(nullptr),
    modelIO(nullptr)
{
    QWidget *plotWidget = new QWidget;
    QWidget *sheetWidget = new QWidget;

    plotOP = new Q_RDDT_CalPlot(plotWidget);
    plotIO = new Q_RDDT_CalPlot(plotWidget);
    plotOP->init(tx_freq_star/1000000.0,tx_freq_stop/1000000.0,-4.0,3.0);
    plotIO->init(tx_freq_star/1000000.0,tx_freq_stop/1000000.0,-4.0,3.0);
    QwtText titlePlotOP = plotOP->title();
    QwtText titlePlotIO = plotIO->title();
    titlePlotOP.setText(tr("Att Offset Curve(X:Freq(MHz) Y:Offset(dB))(Output)"));
    titlePlotIO.setText(tr("Att Offset Curve(X:Freq(MHz) Y:Offset(dB))(IO)"));
    titlePlotOP.setColor(QColor(Qt::blue));
    titlePlotIO.setColor(QColor(Qt::blue));
    plotOP->setTitle(titlePlotOP);
    plotIO->setTitle(titlePlotIO);
    plotOP->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    plotIO->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    QHBoxLayout *plotLayout = new QHBoxLayout(plotWidget);
    plotLayout->addWidget(plotOP);
    plotLayout->addWidget(plotIO);
    plotLayout->setStretch(0,1);
    plotLayout->setStretch(1,1);

    tableViewOP = new Q_RDDT_TableView(this);
    tableViewIO = new Q_RDDT_TableView(this);
    QHBoxLayout *sheetLayout = new QHBoxLayout(sheetWidget);
    sheetLayout->addWidget(tableViewOP);
    sheetLayout->addWidget(tableViewIO);
    sheetLayout->setStretch(0,1);
    sheetLayout->setStretch(1,1);

    curveOP = new QVector<QwtPlotCurve *>[R1C_TX_ATT_OP_POWER_PTS];
    curveIO = new QVector<QwtPlotCurve *>[R1C_TX_ATT_IO_POWER_PTS];

    clear();
    addTab(plotWidget,QString("Plot"));
    addTab(sheetWidget,QString("Sheet"));
}

void QCalR1CTXAttTabWidget::init(QR1CTXAttOPModel *model_OP,QR1CTXAttIOModel *model_IO)
{
    modelOP = model_OP;
    modelIO = model_IO;

    tableViewOP->setModel(modelOP);
    tableViewOP->setColumnWidth(0,80);
    tableViewOP->setColumnWidth(1,60);
    tableViewOP->setColumnWidth(2,40);
    tableViewOP->setColumnWidth(3,40);
    tableViewOP->setColumnWidth(4,40);
    tableViewOP->setColumnWidth(5,40);
    tableViewOP->setColumnWidth(6,70);
    tableViewOP->setColumnWidth(7,70);
    tableViewOP->setColumnWidth(8,100);
    tableViewOP->setColumnWidth(9,100);
    tableViewOP->setColumnWidth(10,100);
    tableViewOP->setColumnWidth(11,100);
    tableViewOP->setColumnWidth(12,120);

    tableViewIO->setModel(modelIO);
    tableViewIO->setColumnWidth(0,80);
    tableViewIO->setColumnWidth(1,60);
    tableViewIO->setColumnWidth(2,40);
    tableViewIO->setColumnWidth(3,40);
    tableViewIO->setColumnWidth(4,40);
    tableViewIO->setColumnWidth(5,40);
    tableViewIO->setColumnWidth(6,70);
    tableViewIO->setColumnWidth(7,70);
    tableViewIO->setColumnWidth(8,100);
    tableViewIO->setColumnWidth(9,100);
    tableViewIO->setColumnWidth(10,100);
    tableViewIO->setColumnWidth(11,100);
    tableViewIO->setColumnWidth(12,120);
}

void QCalR1CTXAttTabWidget::resetShowWidget(CalParam *param)
{
    range_freq_string freqString;
    range_freq<quint64> freqRange;
    freqString.star = param->rfFreqStar.toStdString();
    freqString.stop = param->rfFreqStop.toStdString();
    freqString.step = param->rfFreqStep.toStdString();

    parse_range_freq_string(freqString,freqRange);

    qint32 sections = qint32(freqRange.star.size());

    /*
     * Color table taken from CChartCtrl library,which is a chart control library used in MFC.
     * Sources are in directory rd/lib/ChartCtrl.Color table defined in ChartCtrl.cpp,line 85.
     */
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

    qint32 clrTableSize = ARRAY_SIZE(clrTable);

    if (calOP(param->mode)) {
        if (modelOP != nullptr) {
            modelOP->resetData();
        }

        tx_att_op_table_r1cd::data_f_t data;
        for (quint32 i = 0;i < freqRange.freqs.size();i ++) {
            modelOP->calTable()->append(data);
        }

        for (qint32 i = 0;i < qint32(R1C_TX_ATT_OP_POWER_PTS);i ++) {
            for (qint32 j = 0;j < curveOP[i].size();j ++) {
                curveOP[i].at(j)->detach();
            }
            curveOP[i].clear();

            for (qint32 j = 0;j < sections;j ++) {
                modelOP->iterTable(i)->append(new QwtR1CTXAttOPData(i));
                curveOP[i].append(new QwtPlotCurve);
                curveOP[i].at(j)->setPen(clrTable[i % clrTableSize]);
                curveOP[i].at(j)->attach(plotOP);
                curveOP[i].at(j)->setSamples(modelOP->iterTable(i)->at(j));
            }
        }

        tableViewOP->selectRow(0);
        plotOP->replot();
    }

    if (calIO(param->mode)) {
        if (modelIO != nullptr) {
            modelIO->resetData();
        }

        tx_att_io_table_r1cd::data_f_t data;
        for (quint32 i = 0;i < freqRange.freqs.size();i ++) {
            modelIO->calTable()->append(data);
        }

        for (qint32 i = 0;i < qint32(R1C_TX_ATT_IO_POWER_PTS);i ++) {
            for (qint32 j = 0;j < curveIO[i].size();j ++) {
                curveIO[i].at(j)->detach();
            }
            curveIO[i].clear();

            for (qint32 j = 0;j < sections;j ++) {
                modelIO->iterTable(i)->append(new QwtR1CTXAttIOData(i));
                curveIO[i].append(new QwtPlotCurve);
                curveIO[i].at(j)->setPen(clrTable[i % clrTableSize]);
                curveIO[i].at(j)->attach(plotIO);
                curveIO[i].at(j)->setSamples(modelIO->iterTable(i)->at(j));
            }
        }

        tableViewIO->selectRow(0);
        plotIO->replot();
    }
}
