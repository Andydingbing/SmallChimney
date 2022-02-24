#include "q_cal_rx_filter_offset_tabwidget.h"
#include <QHBoxLayout>
#include "q_model_rx_filter_offset.h"
#include "q_winthread.h"

using namespace ns_sp1401;

QCalRXFilterOffsetWidget::QCalRXFilterOffsetWidget(QWidget *parent) :
    QWidget(parent)
{
    model = nullptr;

    plot = new Q_RDDT_CalPlot(this);
    plot->init(rx_freq_star/1000000.0,rx_freq_stop/1000000.0,-1.0,1.0);
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

    curve = new QVector<QwtPlotCurve *>[3];
}

void QCalRXFilterOffsetWidget::init(QRXFilterOffsetModel *model)
{
    this->model = model;
    tableView->setModel(model);
}

void QCalRXFilterOffsetWidget::resetShowWidget(qint32 pts, qint32 sec)
{
    rx_filter_offset_table::data_f_t data;

    model->resetData();

    for (qint32 i = 0;i < pts;i ++) {
        model->calTable()->append(data);
    }

    for (quint32 i = 0;i < ARRAY_SIZE(data.offset);i ++) {
        for (qint32 j = 0;j < curve[i].size();j ++) {
            curve[i].at(j)->detach();
        }
        curve[i].clear();

        for (qint32 j = 0;j < sec;j ++) {
            model->iterTable(i)->append(new QwtRXFilterOffsetData(i));
            curve[i].append(new QwtPlotCurve);
            curve[i].at(j)->setPen(QColor(Qt::red));
            curve[i].at(j)->attach(plot);
            curve[i].at(j)->setSamples(model->iterTable(i)->at(j));
        }
    }

    tableView->selectRow(0);
    plot->replot();
}


QCalRXFilterOffsetTabWidget::QCalRXFilterOffsetTabWidget(QWidget *parent) :
    QTabWidget(parent)
{
    widget_80 = new QCalRXFilterOffsetWidget(this);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(widget_80);
    layout->setStretch(0,1);
    layout->setSpacing(0);
    layout->setMargin(0);

    this->clear();
    this->addTab(widget_80,QString("80M"));
}

void QCalRXFilterOffsetTabWidget::init(QRXFilterOffsetModel *model)
{
    widget_80->init(model);
}

void QCalRXFilterOffsetTabWidget::resetShowWidget(CalParam *param)
{
    quint32 pts = parseRangeFreqStringFrom<quint64>(param);
    quint32 sec = set_helper::sections(param->rfFreqStar.toStdString());
    widget_80->resetShowWidget(pts,sec);
}
