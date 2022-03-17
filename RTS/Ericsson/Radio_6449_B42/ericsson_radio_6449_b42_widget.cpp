#include "ericsson_radio_6449_b42_widget.h"
#include "ui_ericsson_radio_6449_b42.h"
#include "global.h"

using namespace ns_ericsson;
using namespace ns_radio_6449;

Q_6449_Widget::Q_6449_Widget(QWidget *parent) :
    Q_Widget(parent),
    ui(new Ui::Q_Ericsson_6449_Widget)
{
    ui->setupUi(this);

    ui->comboBoxRX_Type->addItems(Global_Enum::radio_system());
}

Q_6449_Widget::~Q_6449_Widget()
{
    delete ui;
}

void Q_6449_Widget::init()
{

}

void Q_6449_Widget::on_lineEditTX_Freq_textEdited(const QString &arg1)
{

}

void Q_6449_Widget::on_checkBoxTX_clicked(bool checked)
{

}

void Q_6449_Widget::on_comboBoxTX_StepAtt_activated(int index)
{

}

void Q_6449_Widget::on_checkBoxTX_PA_Bias_clicked(bool checked)
{

}

void Q_6449_Widget::on_pushButtonGetTX_PA_Current_clicked()
{

}

void Q_6449_Widget::on_lineEditTX_VGA_textEdited(const QString &arg1)
{

}

void Q_6449_Widget::on_lineEditDL_CW_textEdited(const QString &arg1)
{

}

void Q_6449_Widget::on_checkBoxDL_CW_clicked(bool checked)
{

}

void Q_6449_Widget::on_lineEditRX_Freq_textEdited(const QString &arg1)
{

}

void Q_6449_Widget::on_checkBoxRX_LNA_clicked(bool checked)
{
//    Radio.rxrfsw(RFIdx,checked,ui->checkBoxRX_Att->isChecked());
}

void Q_6449_Widget::on_checkBoxRX_Att_clicked(bool checked)
{
//    Radio.rxrfsw(RFIdx,ui->checkBoxRX_LNA->isChecked(),checked);
}

void Q_6449_Widget::on_checkBoxRX_AGC_clicked(bool checked)
{

}

void Q_6449_Widget::on_lineEditRX_VGA_textEdited(const QString &arg1)
{

}

void Q_6449_Widget::on_lineEditUL_Gain_textEdited(const QString &arg1)
{

}

void Q_6449_Widget::on_comboBoxRX_Type_activated(int index)
{

}

