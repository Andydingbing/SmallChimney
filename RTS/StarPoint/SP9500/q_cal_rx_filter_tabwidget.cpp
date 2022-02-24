#include "q_cal_rx_filter_tabwidget.h"
#include <QVBoxLayout>

using namespace ns_sp1401;
using namespace ns_sp2401;

QCalR1CRXFilterTabWidget::QCalR1CRXFilterTabWidget(QWidget *parent) :
    QTabWidget(parent),
    model_80(nullptr),
    model_160(nullptr)
{
    QWidget *plotWidget = new QWidget;

    plotRF = new Q_RDDT_CalPlot(plotWidget);
    plotRF->init(rx_filter_freq_star/1000000.0,rx_filter_freq_stop/1000000.0,-30.0,-5.0);
    QwtText titleplotRF = plotRF->title();
    titleplotRF.setText(tr("RF Freq Response Curve(X:Freq(MHz) Y:Power(dBm))"));
    titleplotRF.setColor(QColor(Qt::blue));
    plotRF->setTitle(titleplotRF);
    plotRF->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    plotIF = new Q_RDDT_CalPlot(plotWidget);
    plotIF->init(ul_filter_160M_freq_star/1000000.0,ul_filter_160M_freq_stop/1000000.0,-25.0,-5.0);
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

    dataRF_FR = new Qwt_RX_RF_FR_Data;
    dataIF_FR = new Qwt_RX_IF_FR_Data;

    curveRfFr = new QwtPlotCurve("RF Freq Response");
    curveRfFr->setPen(QColor(Qt::red));
    curveRfFr->setVisible(true);
    curveRfFr->attach(plotRF);
    curveRfFr->setSamples(dataRF_FR);

    curveIfFr = new QwtPlotCurve("IF Freq Response");
    curveIfFr->setPen(QColor(Qt::red));
    curveIfFr->setVisible(true);
    curveIfFr->attach(plotIF);
    curveIfFr->setSamples(dataIF_FR);

    tableView_80  = new Q_RDDT_TableView(this);
    tableView_160 = new Q_RDDT_TableView(this);

    clear();
    addTab(plotWidget,QString("Freq Response"));
    addTab(tableView_80,QString("Coef_80M"));
    addTab(tableView_160,QString("Coef_160M"));
}

void QCalR1CRXFilterTabWidget::init(QRXFilterModel_80M *model80, QRXFilterModel_160M *model160)
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

void QCalR1CRXFilterTabWidget::resetShowWidget()
{
    if (model_80 != nullptr) {
        model_80->calTable()->clear();
    }
    if (model_160 != nullptr) {
        model_160->calTable()->clear();
    }

    dataRF_FR->calTable()->clear();
    dataIF_FR->calTable()->clear();
    plotRF->replot();
    plotIF->replot();
}
