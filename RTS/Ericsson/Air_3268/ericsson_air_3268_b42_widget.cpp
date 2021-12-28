#include "ericsson_air_3268_b42_widget.h"
#include "ericsson_air_3268_b42_global.h"

using namespace ns_ericsson;
using namespace ns_air_3268;

Q_3268_Widget::Q_3268_Widget(QWidget *parent) :
    Q_Widget(parent),
    ui(new Ui::Q_Ericsson_Air_3268_B42_Widget)
{
    ui->setupUi(this);

//    ui->comboBoxRX_Type->addItems(Global_Enum::radio_system());
}

Q_3268_Widget::~Q_3268_Widget()
{
    delete ui;
}

void Q_3268_Widget::init()
{

}

void Q_3268_Widget::on_lineEditTX_Freq_textEdited(const QString &arg1)
{
    Radio.set_tx_frequency(RFIdx,freq_string_to_uint64_t(arg1.toStdString()));
}

void Q_3268_Widget::on_checkBoxTX_clicked(bool checked)
{

}

void Q_3268_Widget::on_comboBoxTX_StepAtt_activated(int index)
{

}

void Q_3268_Widget::on_checkBoxTX_PA_Bias_clicked(bool checked)
{
    Radio.pabias(RFIdx);
}

void Q_3268_Widget::on_pushButtonGetTX_PA_Current_clicked()
{
    Radio.pacm(RFIdx);
}

void Q_3268_Widget::on_lineEditTX_VGA_textEdited(const QString &arg1)
{
    Radio.txattmain(RFIdx,arg1.toUInt());
}

void Q_3268_Widget::on_lineEditDL_CW_textEdited(const QString &arg1)
{
    Radio.intdldcw(RFIdx,arg1.toDouble(),ui->checkBoxDL_CW->isChecked());
}

void Q_3268_Widget::on_checkBoxDL_CW_clicked(bool checked)
{
    Radio.intdldcw(RFIdx,ui->lineEditDL_CW->text().toDouble(),checked);
}

void Q_3268_Widget::on_lineEditRX_Freq_textEdited(const QString &arg1)
{

}

void Q_3268_Widget::on_checkBoxRX_LNA_clicked(bool checked)
{
    Radio.rxrfsw(RFIdx,checked,ui->checkBoxRX_Att->isChecked());
}

void Q_3268_Widget::on_checkBoxRX_Att_clicked(bool checked)
{
    Radio.rxrfsw(RFIdx,ui->checkBoxRX_LNA->isChecked(),checked);
}

void Q_3268_Widget::on_checkBoxRX_AGC_clicked(bool checked)
{
    Radio.rxagc(RFIdx,checked);
}

void Q_3268_Widget::on_lineEditRX_VGA_textEdited(const QString &arg1)
{
    Radio.rxrfvga(RFIdx,arg1.toDouble());
}

void Q_3268_Widget::on_lineEditUL_Gain_textEdited(const QString &arg1)
{
    Radio.rxulg(RFIdx,arg1.toDouble());
}

void Q_3268_Widget::on_comboBoxRX_Type_activated(int index)
{

}

