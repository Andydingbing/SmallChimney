#include "q_cal_tx_filter_tabwidget.h"
#include <QVBoxLayout>

using namespace rd::ns_sp1401;
using namespace rd::ns_sp2401;

QCalR1CTXFilterTabWidget::QCalR1CTXFilterTabWidget(QWidget *parent) :
    QTabWidget(parent),
    model_80(nullptr),
    model_160(nullptr)
{
    QWidget *plotWidget = new QWidget;

    plotRF = new Q_RDDT_CalPlot(plotWidget);
    plotRF->init(tx_filter_freq_star_0/1e6,tx_filter_freq_stop_1/1e6,-20.0,10.0);
    QwtText titleplotRF = plotRF->title();
    titleplotRF.setText(tr("RF Freq Response Curve(X:Freq(MHz) Y:Power(dBm))"));
    titleplotRF.setColor(QColor(Qt::blue));
    plotRF->setTitle(titleplotRF);
    plotRF->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    plotIF = new Q_RDDT_CalPlot(plotWidget);
    plotIF->init(dl_filter_160M_freq_star/1e6,dl_filter_160M_freq_stop/1e6,-20.0,4.0);
    QwtText titleplotIF = plotIF->title();
    titleplotIF.setText(tr("IF Freq Response(@RF 2GHz) Curve(X:Freq(MHz) Y:Power(dBm))"));
    titleplotIF.setColor(QColor(Qt::blue));
    plotIF->setTitle(titleplotIF);
    plotIF->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    QVBoxLayout *plotLayout = new QVBoxLayout(plotWidget);
    plotLayout->addWidget(plotRF);
    plotLayout->addWidget(plotIF);
    plotLayout->setStretch(0,1);
    plotLayout->setStretch(1,1);

    dataRF_FR_0 = new Qwt_TX_RF_FR_Data;
    dataRF_FR_1 = new Qwt_TX_RF_FR_Data;
    dataIF_FR = new Qwt_TX_IF_FR_Data;

    curveRF_FR_0 = new QwtPlotCurve("RF Freq Response 0");
    curveRF_FR_0->setPen(QColor(Qt::red));
    curveRF_FR_0->setVisible(true);
    curveRF_FR_0->attach(plotRF);
    curveRF_FR_0->setSamples(dataRF_FR_0);

    curveRF_FR_1 = new QwtPlotCurve("RF Freq Response 1");
    curveRF_FR_1->setPen(QColor(Qt::green));
    curveRF_FR_1->setVisible(true);
    curveRF_FR_1->attach(plotRF);
    curveRF_FR_1->setSamples(dataRF_FR_1);

    curveIF_FR = new QwtPlotCurve("IF Freq Response");
    curveIF_FR->setPen(QColor(Qt::red));
    curveIF_FR->setVisible(true);
    curveIF_FR->attach(plotIF);
    curveIF_FR->setSamples(dataIF_FR);

    tableView_80  = new Q_RDDT_TableView(this);
    tableView_160 = new Q_RDDT_TableView(this);

    clear();
    addTab(plotWidget,QString("Freq Response"));
    addTab(tableView_80,QString("Coef_80M"));
    addTab(tableView_160,QString("Coef_160M"));
}

void QCalR1CTXFilterTabWidget::init(QTXFilterModel *model80, QTXFilterModel *model160)
{
    model_80 = model80;
    model_160 = model160;

    tableView_80->setModel(model_80);
    tableView_80->setColumnWidth(0,80);
    tableView_80->setColumnWidth(1,1000);
    tableView_80->setColumnWidth(2,1000);
    tableView_80->setColumnWidth(3,100);
    tableView_80->setColumnWidth(4,100);
    tableView_80->setColumnWidth(5,100);
    tableView_80->setColumnWidth(6,100);
    tableView_80->setColumnWidth(7,120);

    tableView_160->setModel(model_160);
    tableView_160->setColumnWidth(0,80);
    tableView_160->setColumnWidth(1,1000);
    tableView_160->setColumnWidth(2,1000);
    tableView_160->setColumnWidth(3,100);
    tableView_160->setColumnWidth(4,100);
    tableView_160->setColumnWidth(5,100);
    tableView_160->setColumnWidth(6,100);
    tableView_160->setColumnWidth(7,120);
}

void QCalR1CTXFilterTabWidget::resetShowWidget()
{
    if (model_80 != nullptr) {
        model_80->calTable()->clear();
    }
    if (model_160 != nullptr) {
        model_160->calTable()->clear();
    }

    dataRF_FR_0->calTable()->clear();
    dataRF_FR_1->calTable()->clear();
    dataIF_FR->calTable()->clear();
    plotRF->replot();
    plotIF->replot();
}
