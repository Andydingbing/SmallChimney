#include "temp_ctrl_widget.h"
#include "temp_ctrl_model.h"
#include <QAbstractItemModel>

#include "qwt_plot_helper.h"
#include "qwt_legend.h"
#include "qwt_plot_legenditem.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>

QTempCtrlDlg *dlgTempCtrl = nullptr;

QTempCtrlDlg::QTempCtrlDlg(QWidget *parent) :
    Q_Widget(parent),
    ui(new Ui::Q_Temp_Ctrl_Widget)
{
    ui->setupUi(this);

    initAllSlider(255);

    ui->plot->setTitle(tr("Temperature"));
    ui->plot->setAxisAutoScale(QwtPlot::xBottom);
    ui->plot->setAxisAutoScale(QwtPlot::yLeft);

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

    quint32 clrTableSize = ARRAY_SIZE(clrTable);
    for (quint8 i = 0;i < ns_sp9500::g_max_rf;i ++) {
        curveR1ATx.append(new QwtPlotCurve);
        curveR1ATx.at(i)->setVisible(false);
        curveR1ATx.at(i)->attach(ui->plot);

        curveR1ARx.append(new QwtPlotCurve);
        curveR1ARx.at(i)->setVisible(false);
        curveR1ARx.at(i)->attach(ui->plot);

        curveR1C.append(new QVector<QwtPlotCurve *>);
        dataR1C.append(new QVector<QwtTempData *>);
        curveR1C.at(i)->append(new QwtPlotCurve("Rx_LO2"));
        curveR1C.at(i)->append(new QwtPlotCurve("Rx_LNA"));
        curveR1C.at(i)->append(new QwtPlotCurve("Rx_LO1"));
        curveR1C.at(i)->append(new QwtPlotCurve("Rx_PA1"));
        curveR1C.at(i)->append(new QwtPlotCurve("Tx_LO2"));
        curveR1C.at(i)->append(new QwtPlotCurve("Tx_LO1"));
        curveR1C.at(i)->append(new QwtPlotCurve("Tx_PA4"));
        curveR1C.at(i)->append(new QwtPlotCurve("Tx_PA3"));

        for (quint8 j = 0;j < 8;j ++) {
            dataR1C.at(i)->append(new QwtTempData);
            curveR1C.at(i)->at(j)->setPen(clrTable[j % clrTableSize]);
            curveR1C.at(i)->at(j)->setVisible(true);
            curveR1C.at(i)->at(j)->attach(ui->plot);
            curveR1C.at(i)->at(j)->setSamples(dataR1C.at(i)->at(j));
        }
    }

}

QTempCtrlDlg::~QTempCtrlDlg()
{
    delete ui;
}

#define DECLARE_SLIDER_ARRAY \
    QSlider *slider[ns_sp9500::g_max_fan] = { \
        ui->verticalSlider1, \
        ui->verticalSlider2, \
        ui->verticalSlider3, \
        ui->verticalSlider4, \
        ui->verticalSlider5, \
        ui->verticalSlider6, \
        ui->verticalSlider7, \
        ui->verticalSlider8, \
        ui->verticalSlider9, \
        ui->verticalSlider10, \
        ui->verticalSlider11, \
        ui->verticalSlider12, \
    };

void QTempCtrlDlg::initAllSlider(int max)
{
    DECLARE_SLIDER_ARRAY

    for (quint8 i = 0;i < ns_sp9500::g_max_fan;i ++) {
        slider[i]->setTracking(true);
        slider[i]->setRange(0,max);
        slider[i]->setValue(0);
    }
}

#include <QThread>

void QTempCtrlDlg::on_verticalSlider1_valueChanged(int value)
{
    DECLARE_SLIDER_ARRAY

    SP3501.set_fan(0,value);
    ui->labelSpeed1->setText(QString("%1").arg(value));

    if (QApplication::keyboardModifiers() == Qt::ControlModifier) {
        for (quint8 i = 0;i < ns_sp9500::g_max_fan;i ++) {
            if (i != 0) {
                slider[i]->setValue(value);
                QThread::msleep(5);
//                sleep_ms(5);
            }
        }
    }
}

void QTempCtrlDlg::on_verticalSlider2_valueChanged(int value)
{
    SP3501.set_fan(1,value);
    ui->labelSpeed2->setText(QString("%1").arg(value));
}

void QTempCtrlDlg::on_verticalSlider3_valueChanged(int value)
{
    SP3501.set_fan(2,value);
    ui->labelSpeed3->setText(QString("%1").arg(value));
}

void QTempCtrlDlg::on_verticalSlider4_valueChanged(int value)
{
    SP3501.set_fan(3,value);
    ui->labelSpeed4->setText(QString("%1").arg(value));
}

void QTempCtrlDlg::on_verticalSlider5_valueChanged(int value)
{
    SP3501.set_fan(4,value);
    ui->labelSpeed5->setText(QString("%1").arg(value));
}

void QTempCtrlDlg::on_verticalSlider6_valueChanged(int value)
{
    SP3501.set_fan(5,value);
    ui->labelSpeed6->setText(QString("%1").arg(value));
}

void QTempCtrlDlg::on_verticalSlider7_valueChanged(int value)
{
    SP3501.set_fan(6,value);
    ui->labelSpeed7->setText(QString("%1").arg(value));
}

void QTempCtrlDlg::on_verticalSlider8_valueChanged(int value)
{
    SP3501.set_fan(7,value);
    ui->labelSpeed8->setText(QString("%1").arg(value));
}

void QTempCtrlDlg::on_verticalSlider9_valueChanged(int value)
{
    SP3501.set_fan(8,value);
    ui->labelSpeed9->setText(QString("%1").arg(value));
}

void QTempCtrlDlg::on_verticalSlider10_valueChanged(int value)
{
    SP3501.set_fan(9,value);
    ui->labelSpeed10->setText(QString("%1").arg(value));
}

void QTempCtrlDlg::on_verticalSlider11_valueChanged(int value)
{
    SP3501.set_fan(10,value);
    ui->labelSpeed11->setText(QString("%1").arg(value));
}

void QTempCtrlDlg::on_verticalSlider12_valueChanged(int value)
{
    SP3501.set_fan(11,value);
    ui->labelSpeed12->setText(QString("%1").arg(value));
}

void QTempCtrlDlg::update(double *gettemp)
{
    ui->plot->setAxisScale(QwtPlot::xBottom,0,gettemp[1]+10);
    ui->plot->setAxisScale(QwtPlot::yLeft,35,gettemp[0]+10);

//    ui->plot->setAxisScale(QwtPlot::xBottom,0,gettemp[0]+10);
//    ui->plot->setAxisScale(QwtPlot::yLeft,20,gettemp[1]+10);
    ui->plot->replot();
    ui->plot->show();
}

void QTempCtrlDlg::on_pushButtonStart_clicked()
{
//    param.data = dataR1C;
//    param.SP3301 = SP3301;

//    for (qint8 i = 0;i < ns_sp9500::g_max_rf;i ++) {
//        for (qint8 j = 0;j < 8;j ++) {
//            dataR1C.at(i)->at(j)->calTable()->clear();
//        }
//    }

//    ui->plot->replot();
//    ui->plot->show();

//    QWinThread::g_threadStop = false;
//    QWinThread::g_threadThread = new QR1CTempCtrlThread(&param,this);
//    QR1CTempCtrlThread *thread = (QR1CTempCtrlThread *)(QWinThread::g_threadThread);
//    QWinThread::g_threadThread->start();
//    connect(thread,SIGNAL(update(double*)),this,SLOT(update(double*)),Qt::BlockingQueuedConnection);
}

void QTempCtrlDlg::on_pushButton_clicked()
{
//    QWinThread::g_threadStop = true;
//    ui->plot->replot();
}

