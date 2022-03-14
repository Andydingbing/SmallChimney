#include "ericsson_radio_4415_b3_widget.h"
#include "ui_ericsson_radio_4415_b3.h"
#include "ericsson_radio_4415_b3_global.h"

using namespace ns_ericsson;
using namespace ns_radio_4415;

Q_4415_Widget::Q_4415_Widget(QWidget *parent) :
    Q_Widget(parent),
    ui(new Ui::Q_Ericsson_4415_Widget)
{
    ui->setupUi(this);

    ui->comboBoxRX_Type->addItems(Global_Enum::radio_system());
}

Q_4415_Widget::~Q_4415_Widget()
{
    delete ui;
}

void Q_4415_Widget::init()
{

}

void Q_4415_Widget::on_lineEditTX_Freq_textEdited(const QString &arg1)
{

}

void Q_4415_Widget::on_checkBoxTX_clicked(bool checked)
{

}

void Q_4415_Widget::on_comboBoxTX_StepAtt_activated(int index)
{

}

void Q_4415_Widget::on_checkBoxTX_PA_Bias_clicked(bool checked)
{
//    ericsson_radio_4415_pabias(RFIdx);
}

void Q_4415_Widget::on_pushButtonGetTX_PA_Current_clicked()
{
//    ericsson_radio_4415_pacm(RFIdx);
}

void Q_4415_Widget::on_lineEditTX_VGA_textEdited(const QString &arg1)
{
//    ericsson_radio_4415_txattmain(RFIdx,arg1.toUInt());
}

void Q_4415_Widget::on_lineEditDL_CW_textEdited(const QString &arg1)
{
//    ericsson_radio_4415_intdldcw(RFIdx,arg1.toDouble(),ui->checkBoxDL_CW->isChecked());
}

void Q_4415_Widget::on_checkBoxDL_CW_clicked(bool checked)
{
//    ericsson_radio_4415_intdldcw(RFIdx,ui->lineEditDL_CW->text().toDouble(),checked);
}

void Q_4415_Widget::on_lineEditRX_Freq_textEdited(const QString &arg1)
{

}

void Q_4415_Widget::on_checkBoxRX_LNA_clicked(bool checked)
{
    Radio.rxrfsw(RFIdx,checked,ui->checkBoxRX_Att->isChecked());
}

void Q_4415_Widget::on_checkBoxRX_Att_clicked(bool checked)
{
    Radio.rxrfsw(RFIdx,ui->checkBoxRX_LNA->isChecked(),checked);
}

void Q_4415_Widget::on_checkBoxRX_AGC_clicked(bool checked)
{
//    ericsson_radio_4415_rxagc(RFIdx,checked);
}

void Q_4415_Widget::on_lineEditRX_VGA_textEdited(const QString &arg1)
{
//    ericsson_radio_4415_rxrfvga(RFIdx,arg1.toDouble());
}

void Q_4415_Widget::on_lineEditUL_Gain_textEdited(const QString &arg1)
{
//    ericsson_radio_4415_rxulg(RFIdx,arg1.toDouble());
}

void Q_4415_Widget::on_comboBoxRX_Type_activated(int index)
{

}

