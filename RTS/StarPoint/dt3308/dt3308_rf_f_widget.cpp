#include "dt3308_rf_f_widget.h"
#include "ui_dt3308_rf_f.h"

using namespace ns_dt3308::f;

Q_DT3308_RF_F_Widget::Q_DT3308_RF_F_Widget(QWidget *parent) :
    Q_RD_Widget(parent),
    dlg_TX_HMC832(nullptr),
    dlg_TX_HMC833(nullptr),
    ui(new Ui::Q_DT3308_RF_F_Widget)
{
    ui->setupUi(this);

    QString att;
    for (quint8 i = 0;i < SERIE_SIZE(0,31,1);i ++) {
        att = QString("%1").arg(i);
        ui->comboBoxTxAtt0->addItem(att);
        ui->comboBoxTxAtt1->addItem(att);
        ui->comboBoxTxAtt2->addItem(att);
        ui->comboBoxTxAtt3->addItem(att);
        ui->comboBoxTxAtt4->addItem(att);
        ui->comboBoxTxAtt5->addItem(att);
        ui->comboBoxRxAtt0->addItem(att);
        ui->comboBoxRxAtt1->addItem(att);
    }

    for (quint8 i = 0;i < SERIE_SIZE(0,31.75,0.25);i ++) {
        att = QString("%1").arg(double(i) / 4);
        ui->comboBoxPortAtt0->addItem(att);
        ui->comboBoxPortAtt1->addItem(att);
        ui->comboBoxPortAtt2->addItem(att);
        ui->comboBoxPortAtt3->addItem(att);
        ui->comboBoxPortAtt4->addItem(att);
        ui->comboBoxPortAtt5->addItem(att);
        ui->comboBoxPortAtt6->addItem(att);
        ui->comboBoxPortAtt7->addItem(att);
    }

    QString IOMode[5] = { "IO","Output","Loop","Input","Close"};
    for (quint8 i = 0;i < ARRAY_SIZE(IOMode);i ++) {
        ui->comboBoxIOMode0->addItem(IOMode[i]);
        ui->comboBoxIOMode1->addItem(IOMode[i]);
        ui->comboBoxIOMode2->addItem(IOMode[i]);
        ui->comboBoxIOMode3->addItem(IOMode[i]);
        ui->comboBoxIOMode4->addItem(IOMode[i]);
        ui->comboBoxIOMode5->addItem(IOMode[i]);
        ui->comboBoxIOMode6->addItem(IOMode[i]);
        ui->comboBoxIOMode7->addItem(IOMode[i]);
    }
    ui->comboBoxIOMode8->addItem("On");
    ui->comboBoxIOMode8->addItem("Off");

    ui->comboBoxRxLNAAttSw0->addItem("LNA");
    ui->comboBoxRxLNAAttSw0->addItem("Att");
    ui->comboBoxRxLNAAttSw1->addItem("LNA");
    ui->comboBoxRxLNAAttSw1->addItem("Att");

    ui->comboBoxIFFilterSw->addItem("40M");
    ui->comboBoxIFFilterSw->addItem("100M");
    ui->comboBoxIFFilterSw->addItem("160M");

    ui->lineEditTxFreq->setText("2G");
    ui->comboBoxTxAtt1->setCurrentIndex(0);
    ui->comboBoxTxAtt2->setCurrentIndex(0);
    ui->comboBoxTxAtt3->setCurrentIndex(0);

    ui->lineEditRxFreq->setText("2G");
    ui->comboBoxRxLNAAttSw0->setCurrentIndex(RX_ATT_0);
    ui->comboBoxRxLNAAttSw1->setCurrentIndex(RX_ATT_1);
    ui->comboBoxRxAtt0->setCurrentIndex(0);
    ui->comboBoxRxAtt1->setCurrentIndex(0);
    ui->comboBoxIFFilterSw->setCurrentIndex(2);
    ui->lineEditRxTemp->setText("0");
}

Q_DT3308_RF_F_Widget::~Q_DT3308_RF_F_Widget()
{
    delete ui;
}

void Q_DT3308_RF_F_Widget::on_pushButtonSetTxFreq_clicked()
{
    RD_TRY
    DT3308_RF_SetTxFrequency(freq_string_to<quint64>(ui->lineEditTxFreq->text().toStdString()));
    RD_LOG_ALL_EXCEPTION
}

void Q_DT3308_RF_F_Widget::on_lineEditTxFreq_textEdited(const QString &arg1)
{
    Q_UNUSED(arg1);
    on_pushButtonSetTxFreq_clicked();
}

void Q_DT3308_RF_F_Widget::on_pushButtonSetTxFilter_clicked()
{
    DT3308_RF_F->set_tx_filter_sw(rf_dt3308_f::tx_freq_section_t(ui->comboBoxTxFilter->currentIndex()));
}

void Q_DT3308_RF_F_Widget::on_pushButtonSetTxAtt0_clicked()
{
    DT3308_RF_F->set_tx_att0(ui->comboBoxTxAtt0->currentIndex());
}

void Q_DT3308_RF_F_Widget::on_pushButtonSetTxAtt1_clicked()
{
    DT3308_RF_F->set_tx_att1(ui->comboBoxTxAtt1->currentIndex());
}

void Q_DT3308_RF_F_Widget::on_pushButtonSetTxAtt2_clicked()
{
    DT3308_RF_F->set_tx_att2(ui->comboBoxTxAtt2->currentIndex());
}

void Q_DT3308_RF_F_Widget::on_pushButtonSetTxAtt3_clicked()
{
    DT3308_RF_F->set_tx_att3(ui->comboBoxTxAtt3->currentIndex());
}

void Q_DT3308_RF_F_Widget::on_pushButtonSetTxAtt4_clicked()
{
    DT3308_RF_F->set_tx_att4(ui->comboBoxTxAtt4->currentIndex());
}

void Q_DT3308_RF_F_Widget::on_pushButtonSetTxAtt5_clicked()
{
    DT3308_RF_F->set_tx_att5(ui->comboBoxTxAtt5->currentIndex());
}

void Q_DT3308_RF_F_Widget::on_pushButtonSetRxFreq_clicked()
{
    RD_TRY
    DT3308_RF_SetRxFrequency(freq_string_to<quint64>(ui->lineEditTxFreq->text().toStdString()));
    RD_LOG_ALL_EXCEPTION
}

void Q_DT3308_RF_F_Widget::on_pushButtonSetRxLNAAttSw0_clicked()
{
    DT3308_RF_F->set_rx_lna_att_0_sw(rx_lna_att_0_t(ui->comboBoxRxLNAAttSw0->currentIndex()));
}

void Q_DT3308_RF_F_Widget::on_pushButtonSetRxLNAAttSw1_clicked()
{
    DT3308_RF_F->set_rx_lna_att_1_sw(rx_lna_att_1_t(ui->comboBoxRxLNAAttSw1->currentIndex()));
}

void Q_DT3308_RF_F_Widget::on_pushButtonSetRxAtt0_clicked()
{
    DT3308_RF_F->set_rx_att0(ui->comboBoxRxAtt0->currentIndex());
}

void Q_DT3308_RF_F_Widget::on_pushButtonSetRxAtt1_clicked()
{
    DT3308_RF_F->set_rx_att1(ui->comboBoxRxAtt1->currentIndex());
}

void Q_DT3308_RF_F_Widget::on_pushButtonSetIFFilterSw_clicked()
{
    DT3308_RF_F->set_rx_if_filter_sw(rx_if_filter_t(ui->comboBoxIFFilterSw->currentIndex()));
}

void Q_DT3308_RF_F_Widget::on_pushButtonSetTxHMC832_clicked()
{
    DT3308_RF_F->set_tx_hmc832(freq_string_to<quint64>(ui->lineEditTxHMC832Freq->text().toStdString()));
}

void Q_DT3308_RF_F_Widget::on_pushButtonTxHMC832Adv_clicked()
{
    if (dlg_TX_HMC832 == nullptr) {
        dlg_TX_HMC832 = new Q_HMC832_Dlg(rf_dt3308_f::TX_HMC832);
        dlg_TX_HMC832->RF = DT3308_RF_F;
        dlg_TX_HMC832->setWindowTitle(QString("DT3308_F_HMC832_Tx"));
    }

    dlg_TX_HMC832->setVisible(true);
    dlg_TX_HMC832->showNormal();
    dlg_TX_HMC832->activateWindow();
}

void Q_DT3308_RF_F_Widget::on_pushButtonSetTxHMC833_clicked()
{
    DT3308_RF_F->set_tx_hmc833(freq_string_to<quint64>(ui->lineEditTxHMC833Freq->text().toStdString()));
}

void Q_DT3308_RF_F_Widget::on_lineEditTxHMC833Freq_textEdited(const QString &text)
{
    RD_TRY
    DT3308_RF_F->set_tx_hmc833(freq_string_to<quint64>(text.toStdString()));
    RD_LOG_ALL_EXCEPTION
}

void Q_DT3308_RF_F_Widget::on_pushButtonTxHMC833Adv_clicked()
{
    if (dlg_TX_HMC833 == nullptr) {
        dlg_TX_HMC833 = new Q_HMC832_Dlg(rf_dt3308_f::TX_HMC833);
        dlg_TX_HMC833->RF = DT3308_RF_F;
        dlg_TX_HMC833->setWindowTitle(QString("DT3308_F_HMC833_Tx"));
    }

    dlg_TX_HMC833->setVisible(true);
    dlg_TX_HMC833->showNormal();
    dlg_TX_HMC833->activateWindow();
}

void Q_DT3308_RF_F_Widget::on_pushButtonSetTxHMC833Filter_clicked()
{
    DT3308_RF_F->set_tx_hmc833_filter_sw(rf_dt3308_f::tx_hmc833_freq_section_t(ui->comboBoxTxHMC833Filter->currentIndex()));
}

void Q_DT3308_RF_F_Widget::on_checkBoxTxEn_clicked(bool checked)
{
    DT3308_RF_F->set_tx_en(checked);
}

void Q_DT3308_RF_F_Widget::on_checkBoxRxEn_clicked(bool checked)
{
    DT3308_RF_F->set_rx_en(checked);
}

void Q_DT3308_RF_F_Widget::on_comboBoxIOMode0_activated(int index)
{
    DT3308_RF_F->set_io_mode(rf_dt3308_f::PORT_0,io_mode_t(index));
}

void Q_DT3308_RF_F_Widget::on_comboBoxIOMode1_activated(int index)
{
    DT3308_RF_F->set_io_mode(rf_dt3308_f::PORT_1,io_mode_t(index));
}

void Q_DT3308_RF_F_Widget::on_comboBoxIOMode2_activated(int index)
{
    DT3308_RF_F->set_io_mode(rf_dt3308_f::PORT_2,io_mode_t(index));
}

void Q_DT3308_RF_F_Widget::on_comboBoxIOMode3_activated(int index)
{
    DT3308_RF_F->set_io_mode(rf_dt3308_f::PORT_3,io_mode_t(index));
}

void Q_DT3308_RF_F_Widget::on_comboBoxIOMode4_activated(int index)
{
    DT3308_RF_F->set_io_mode(rf_dt3308_f::PORT_4,io_mode_t(index));
}

void Q_DT3308_RF_F_Widget::on_comboBoxIOMode5_activated(int index)
{
    DT3308_RF_F->set_io_mode(rf_dt3308_f::PORT_5,io_mode_t(index));
}

void Q_DT3308_RF_F_Widget::on_comboBoxIOMode6_activated(int index)
{
    DT3308_RF_F->set_io_mode(rf_dt3308_f::PORT_6,io_mode_t(index));
}

void Q_DT3308_RF_F_Widget::on_comboBoxIOMode7_activated(int index)
{
    DT3308_RF_F->set_io_mode(rf_dt3308_f::PORT_7,io_mode_t(index));
}

void Q_DT3308_RF_F_Widget::on_comboBoxPortAtt0_activated(int index)
{
    DT3308_RF_F->set_tx_port_att0(index * 0.25);
}

void Q_DT3308_RF_F_Widget::on_comboBoxPortAtt1_activated(int index)
{
    DT3308_RF_F->set_tx_port_att1(index * 0.25);
}

void Q_DT3308_RF_F_Widget::on_comboBoxPortAtt2_activated(int index)
{
    DT3308_RF_F->set_tx_port_att2(index * 0.25);
}

void Q_DT3308_RF_F_Widget::on_comboBoxPortAtt3_activated(int index)
{
    DT3308_RF_F->set_tx_port_att3(index * 0.25);
}

void Q_DT3308_RF_F_Widget::on_comboBoxPortAtt4_activated(int index)
{
    DT3308_RF_F->set_tx_port_att4(index * 0.25);
}

void Q_DT3308_RF_F_Widget::on_comboBoxPortAtt5_activated(int index)
{
    DT3308_RF_F->set_tx_port_att5(index * 0.25);
}

void Q_DT3308_RF_F_Widget::on_comboBoxPortAtt6_activated(int index)
{
    DT3308_RF_F->set_tx_port_att6(index * 0.25);
}

void Q_DT3308_RF_F_Widget::on_comboBoxPortAtt7_activated(int index)
{
    DT3308_RF_F->set_tx_port_att7(index * 0.25);
}

void Q_DT3308_RF_F_Widget::on_checkBoxTxHMC832_clicked(bool checked)
{
    DT3308_RF_F->set_tx_hmc832_en(checked);
}

void Q_DT3308_RF_F_Widget::on_checkBoxTxMainPA0_0_3_stateChanged(int arg1)
{
    DT3308_RF_F->set_tx_main_pa_sw_0(rf_dt3308_f::PORT_0,arg1 == Qt::Checked ? true : false);
}

void Q_DT3308_RF_F_Widget::on_checkBoxTxMainPA1_0_3_stateChanged(int arg1)
{
    DT3308_RF_F->set_tx_main_pa_sw_1(rf_dt3308_f::PORT_0,arg1 == Qt::Checked ? true : false);
}

void Q_DT3308_RF_F_Widget::on_checkBoxTxMainPA0_4_7_stateChanged(int arg1)
{
    DT3308_RF_F->set_tx_main_pa_sw_0(rf_dt3308_f::PORT_4,arg1 == Qt::Checked ? true : false);
}

void Q_DT3308_RF_F_Widget::on_checkBoxTxMainPA1_4_7_stateChanged(int arg1)
{
    DT3308_RF_F->set_tx_main_pa_sw_1(rf_dt3308_f::PORT_4,arg1 == Qt::Checked ? true : false);
}
