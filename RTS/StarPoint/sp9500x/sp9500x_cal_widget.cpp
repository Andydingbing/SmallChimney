#include "cal_widget.h"
#include "sp9500x_cal_tx_filter_childwidgets.h"
#include "sp9500x_cal_tx_pwr_childwidgets.h"
#include "sp9500x_cal_tx_att_childwidgets.h"
#include "spec.h"

using namespace ns_sp1403;
using namespace ns_sp9500x;
using namespace NS_SP9500X;

void Q_Cal_TXFilter_Widget::init()
{
//    ui->lineEditRFFreqs->setText(QString::fromStdString(SP1403->cal_file()->tx_rf_if_fr_0000_3000_table()->freq_sequence_string()));
    ui->lineEditRFFreqs->setText("250M:10M:3G");
    ui->lineEditIFFreqs->setText("100M,200M,400M,800M");

    childs = new Cal_TXFilter_ChildWidgets(this);
    model->push_back(childs->model_100);
    model->push_back(childs->model_200);
    model->push_back(childs->model_400);
    model->push_back(childs->model_800);

    addIdleWidget(ui->pushButtonExport);
    addIdleWidget(ui->pushButtonStar);
    addIdleWidget(ui->pushButtonGetSet);
    addRunningWidget(ui->pushButtonStop);
    addToPauseWidget(ui->pushButtonPaus);
    addToRunningWidget(ui->pushButtonCont);
    widgetResume();
    QCalBaseDlg::init();
}

void Q_Cal_TXFilter_Widget::prepare(const bool is_exp)
{
    SP1403->cal_file()->prepare_cal(cal_table_t::TX_RF_FR_0000_3000);

    childs->prepare(is_exp);
    childs->plotRF->replot();
    childs->plotIF->replot();
    emit reset();
}

void Q_Cal_TXFilter_Widget::resetShowWidget(CalParam *param) {}

void Q_Cal_TXFilter_Widget::uiToCalParam(CalParam *param)
{
    param->parent = this;
    param->cal = ui->checkBoxCal->isChecked();
    param->check = ui->checkBoxCheck->isChecked();
}

void Q_Cal_TXFilter_Widget::updateFromParam(const CalR1CParam &param)
{
//    ui->checkBoxRebuildCoef->setChecked(false);
}

void Q_Cal_TXFilter_Widget::uiInsert(const int first,const int last,const int cal_table) {}

void Q_Cal_TXFilter_Widget::uiUpdate(const int first,const int last,const int cal_table)
{
    switch (cal_table) {
    case cal_table_t::TX_IF_FR_3000_7500 :
        childs->plotIF->replot();
        return;
    case cal_table_t::TX_RF_IF_FR_0000_3000 :
    case cal_table_t::TX_RF_FR_0000_3000 :
    case cal_table_t::TX_RF_FR_3000_4800 :
    case cal_table_t::TX_RF_FR_4800_6000 :
    case cal_table_t::TX_RF_FR_6000_7500 :
        childs->plotRF->replot();
        return;
    }
}

void Q_Cal_TXFilter_Widget::getset()
{
}

void Q_Cal_TX_Pwr_Widget::init()
{
    ui->textEditRFFreqs->setText("250M:10M:3G");

    childs = new Cal_TX_Pwr_ChildWidgets(this);

    model->push_back(&childs->modelTX0_Output);
    model->push_back(&childs->modelTX0_IO);
    model->push_back(&childs->modelTX1);

    addIdleWidget(ui->pushButtonExport);
    addIdleWidget(ui->pushButtonStar);
    addIdleWidget(ui->pushButtonGetSet);
    addRunningWidget(ui->pushButtonStop);
    addToPauseWidget(ui->pushButtonPaus);
    addToRunningWidget(ui->pushButtonCont);
    widgetResume();
    QCalBaseDlg::init();
}

void Q_Cal_TX_Pwr_Widget::prepare(const bool is_exp)
{
    childs->prepare(is_exp);
    ui->plot->replot();
    emit reset();
}

void Q_Cal_TX_Pwr_Widget::resetShowWidget(CalParam *param) {}

void Q_Cal_TX_Pwr_Widget::uiToCalParam(CalParam *param)
{
    param->parent = this;
    param->cal = ui->checkBoxCal->isChecked();
    param->check = ui->checkBoxCheck->isChecked();
}

void Q_Cal_TX_Pwr_Widget::updateFromParam(const CalR1CParam &param)
{
//    ui->checkBoxRebuildCoef->setChecked(false);
}

void Q_Cal_TX_Pwr_Widget::uiInsert(const int first,const int last,const int cal_table) {}

void Q_Cal_TX_Pwr_Widget::uiUpdate(const int first,const int last,const int cal_table)
{
    ignore_unused(last);
    if (cal_table == cal_table_t::TX0_Pwr_Output) {
        ui->tabWidget->setCurrentIndex(0);
        ui->tableViewTX0_Output->selectRow(first);
    } else if (cal_table == cal_table_t::TX0_Pwr_IO) {
        ui->tabWidget->setCurrentIndex(1);
        ui->tableViewTX0_IO->selectRow(first);
    } else {
        ui->tabWidget->setCurrentIndex(2);
        ui->tableViewTX1->selectRow(first);
    }
    ui->plot->replot();
}

void Q_Cal_TX_Pwr_Widget::getset()
{
}









void Q_Cal_TX_Att_Widget::init()
{
    ui->textEditRFFreqs->setText("250M:10M:3G");

    childs = new Cal_TX_Att_ChildWidgets(this);

    model->push_back(&childs->modelTX0_Output);
    model->push_back(&childs->modelTX0_IO);
    model->push_back(&childs->modelTX1);

    addIdleWidget(ui->pushButtonExport);
    addIdleWidget(ui->pushButtonStar);
    addIdleWidget(ui->pushButtonGetSet);
    addRunningWidget(ui->pushButtonStop);
    addToPauseWidget(ui->pushButtonPaus);
    addToRunningWidget(ui->pushButtonCont);
    widgetResume();
    QCalBaseDlg::init();
}

void Q_Cal_TX_Att_Widget::prepare(const bool is_exp)
{
    childs->prepare(is_exp);
    ui->plot->replot();
    emit reset();
}

void Q_Cal_TX_Att_Widget::resetShowWidget(CalParam *param) {}

void Q_Cal_TX_Att_Widget::uiToCalParam(CalParam *param)
{
    param->parent = this;
    param->cal = ui->checkBoxCal->isChecked();
    param->check = ui->checkBoxCheck->isChecked();
}

void Q_Cal_TX_Att_Widget::updateFromParam(const CalR1CParam &param)
{
//    ui->checkBoxRebuildCoef->setChecked(false);
}

void Q_Cal_TX_Att_Widget::uiInsert(const int first,const int last,const int cal_table) {}

void Q_Cal_TX_Att_Widget::uiUpdate(const int first,const int last,const int cal_table)
{
    ignore_unused(last);
    if (cal_table == cal_table_t::TX0_Pwr_Output) {
        ui->tabWidget->setCurrentIndex(0);
        ui->tableViewTX0_Output->selectRow(first);
    } else if (cal_table == cal_table_t::TX0_Pwr_IO) {
        ui->tabWidget->setCurrentIndex(1);
        ui->tableViewTX0_IO->selectRow(first);
    } else {
        ui->tabWidget->setCurrentIndex(2);
        ui->tableViewTX1->selectRow(first);
    }
    ui->plot->replot();
}

void Q_Cal_TX_Att_Widget::getset()
{
}
