#include "q_cal_tx_filter_offset_tabwidget.h"
#include <QHBoxLayout>
#include "q_winthread.h"

using namespace ns_sp1401;

QCalTXFilterOffsetWidget::QCalTXFilterOffsetWidget(QWidget *parent) :
    QWidget(parent)
{
    model = nullptr;

    plot = new Q_RDDT_CalPlot(this);
    plot->init(tx_freq_star/1000000.0,tx_freq_stop/1000000.0,-1.0,1.0);
    QwtText textTitle = plot->title();
    textTitle.setText(tr("Filter Offset Curve(X:Freq(MHz) Y:Offset(dB))"));
    textTitle.setColor(QColor(Qt::blue));
    plot->setTitle(textTitle);
    plot->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    tableView = new Q_RDDT_TableView(this);
    tableView->setColumnWidth(0,80);
    tableView->setColumnWidth(1,70);
    tableView->setColumnWidth(2,100);
    tableView->setColumnWidth(3,100);
    tableView->setColumnWidth(4,100);
    tableView->setColumnWidth(5,100);
    tableView->setColumnWidth(6,140);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(tableView);
    layout->addWidget(plot);
    layout->setStretch(0,10);
    layout->setStretch(1,1);
    layout->setSpacing(0);
    layout->setMargin(0);

    curve = new QVector<QwtPlotCurve *>;
}

void QCalTXFilterOffsetWidget::init(QTXFilterOffsetModel *model)
{
    this->model = model;
    tableView->setModel(model);
}

void QCalTXFilterOffsetWidget::resetShowWidget(qint32 pts, qint32 sec)
{
    tx_filter_offset_table::data_f_t data;

    model->resetData();

    for (qint32 i = 0;i < pts;i ++) {
        model->calTable()->append(data);
    }

    for (qint32 i = 0;i < curve->size();i ++) {
        curve->at(i)->detach();
    }
    curve->clear();

    for (qint32 i = 0;i < sec;i ++) {
        model->iterTable()->append(new QwtTXFilterOffsetData);
        curve->append(new QwtPlotCurve);
        curve->at(i)->setPen(QColor(Qt::red));
        curve->at(i)->attach(plot);
        curve->at(i)->setSamples(model->iterTable()->at(i));
    }

    tableView->selectRow(0);
    plot->replot();
}


QCalTXFilterOffsetTabWidget::QCalTXFilterOffsetTabWidget(QWidget *parent) :
    QTabWidget(parent)
{
    widget_80 = new QCalTXFilterOffsetWidget(this);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(widget_80);
    layout->setStretch(0,1);
    layout->setSpacing(0);
    layout->setMargin(0);

    this->clear();
    this->addTab(widget_80,QString("80M"));
}

void QCalTXFilterOffsetTabWidget::init(QTXFilterOffsetModel *modelOP)
{
    widget_80->init(modelOP);
}

void QCalTXFilterOffsetTabWidget::resetShowWidget(CalParam *param)
{
    quint32 pts = parseRangeFreqStringFrom<quint64>(param);
    quint32 sec = set_helper::sections(param->rfFreqStar.toStdString());
    widget_80->resetShowWidget(pts,sec);
}
