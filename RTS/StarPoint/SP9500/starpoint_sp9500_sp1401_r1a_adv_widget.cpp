#include "starpoint_sp9500_sp1401_r1a_adv_widget.h"

using namespace ns_sp1401;
using namespace ns_sp1401::r1a;

Q_StarPoint_SP9500_SP1401_R1A_Adv_Widget::Q_StarPoint_SP9500_SP1401_R1A_Adv_Widget(QWidget *parent) :
    Q_Widget(parent),
    ui(new Ui::Q_StarPoint_SP9500_SP1401_R1A_Adv_Widget)
{
    ui->setupUi(this);

    dlg_ADF5355[sp1401_r1a::TX_LO_1] = nullptr;
    dlg_ADF5355[sp1401_r1a::TX_LO_2] = nullptr;
    dlg_ADF5355[sp1401_r1a::RX_LO_1] = nullptr;
    dlg_ADF5355[sp1401_r1a::RX_LO_2] = nullptr;

    QString strAtt[4] = {"0","10","20","30"};
    for (quint32 i = 0;i < ARRAY_SIZE(strAtt);i ++) {
        ui->comboBoxTxAtt1->addItem(strAtt[i]);
        ui->comboBoxTxAtt2->addItem(strAtt[i]);
        ui->comboBoxTxAtt3->addItem(strAtt[i]);
        ui->comboBoxRxAtt2->addItem(strAtt[i]);
    }
    double rxAtt1[SERIE_SIZE(0.0,31.5,0.5)] = {0.0};
    for (quint32 i = 0;i < ARRAY_SIZE(rxAtt1);i ++) {
        rxAtt1[i] = i * 0.5;
        QString strRxAtt1 = QString("%1").arg(rxAtt1[i],2,'f',1);
        ui->comboBoxRxAtt1->addItem(strRxAtt1);
    }
    ui->comboBoxRxAtt1->setMaxVisibleItems(20);

    ui->lineEditTxFreq->setText("2G");
    ui->comboBoxTxAtt1->setCurrentIndex(1);
    ui->comboBoxTxAtt2->setCurrentIndex(0);
    ui->comboBoxTxPAAttSw->setCurrentIndex(TX_ATT);
    ui->comboBoxTxAtt3->setCurrentIndex(0);
    ui->lineEditTxDet->setText("0");
    ui->lineEditTxTemp->setText("0");
    ui->comboBoxIOMode->setCurrentIndex(io_mode_t::OUTPUT);

    ui->lineEditRxFreq->setText("2G");
    ui->comboBoxRxLNAAttSw->setCurrentIndex(RX_ATT);
    ui->comboBoxRxAtt1->setCurrentIndex(int(15.0 / 0.5));
    ui->comboBoxRxAtt2->setCurrentIndex(1);
    ui->comboBoxIFFilterSw->setCurrentIndex(int(sp1401::_160MHz));
    ui->lineEditRxDet->setText("0");
    ui->lineEditRxTemp->setText("0");
}

Q_StarPoint_SP9500_SP1401_R1A_Adv_Widget::~Q_StarPoint_SP9500_SP1401_R1A_Adv_Widget()
{
    delete ui;
}

void Q_StarPoint_SP9500_SP1401_R1A_Adv_Widget::on_comboBoxTxIOSw_activated(int index)
{
    SP1401_R1A->set_tx_io_sw(sp1401_r1a::tx_io_sw_t(ui->comboBoxTxIOSw->currentIndex()));
}

void Q_StarPoint_SP9500_SP1401_R1A_Adv_Widget::on_comboBoxTxAtt3_activated(int index)
{
    SP1401_R1A->set_tx_att3(ui->comboBoxTxAtt3->currentIndex() * 10);
}

void Q_StarPoint_SP9500_SP1401_R1A_Adv_Widget::on_comboBoxTxPAAttSw_activated(int index)
{
    SP1401_R1A->set_tx_pa_att_sw(tx_pa_att_t(ui->comboBoxTxPAAttSw->currentIndex()));
}

void Q_StarPoint_SP9500_SP1401_R1A_Adv_Widget::on_comboBoxTxAtt2_activated(int index)
{
    SP1401_R1A->set_tx_att2(ui->comboBoxTxAtt2->currentIndex() * 10);
}

void Q_StarPoint_SP9500_SP1401_R1A_Adv_Widget::on_comboBoxTxAtt1_activated(int index)
{
    SP1401_R1A->set_tx_att1(ui->comboBoxTxAtt1->currentIndex() * 10);
}

void Q_StarPoint_SP9500_SP1401_R1A_Adv_Widget::on_comboBoxTxBandSw_activated(int index)
{
    SP1401_R1A->set_tx_band_sw(sp1401_r1a::tx_band_t(ui->comboBoxTxBandSw->currentIndex()));
}

void Q_StarPoint_SP9500_SP1401_R1A_Adv_Widget::on_pushButtonModEn_clicked()
{
    bool en = ui->pushButtonModEn->isChecked();
    if (SP1401_R1A->set_tx_modulator_en(en)) {
        ui->pushButtonModEn->setChecked(false);
        ui->pushButtonModEn->setText(tr("ADL5375 Dis"));
    } else {
        ui->pushButtonModEn->setText(QString("ADL5375 %1").arg(en ? "En" : "Dis"));
    }
}

void Q_StarPoint_SP9500_SP1401_R1A_Adv_Widget::on_pushButtonPowerOn_clicked()
{
    bool en = ui->pushButtonPowerOn->isChecked();
    if (SP1401_R1A->set_pwr_en(en)) {
        ui->pushButtonPowerOn->setChecked(false);
        ui->pushButtonPowerOn->setText(tr("Power Off"));
    } else {
        ui->pushButtonPowerOn->setText(QString("Power %1").arg(en ? "On" : "Off"));
    }
}

void Q_StarPoint_SP9500_SP1401_R1A_Adv_Widget::on_comboBoxIOMode_activated(int index)
{
    SP1401_R1A->set_io_mode(io_mode_t::_from_index(ui->comboBoxIOMode->currentIndex()));
}

void Q_StarPoint_SP9500_SP1401_R1A_Adv_Widget::on_checkBoxTxLED_clicked()
{
    bool txLED = ui->checkBoxTxLED->isChecked();
    bool rxLED = ui->checkBoxRxLED->isChecked();
    SP1401_R1A->set_led(txLED,rxLED);
}

void Q_StarPoint_SP9500_SP1401_R1A_Adv_Widget::on_checkBoxRxLED_clicked()
{
    bool txLED = ui->checkBoxTxLED->isChecked();
    bool rxLED = ui->checkBoxRxLED->isChecked();
    SP1401_R1A->set_led(txLED,rxLED);
}

void Q_StarPoint_SP9500_SP1401_R1A_Adv_Widget::on_pushButtonSetTxFreq_clicked()
{

}

void Q_StarPoint_SP9500_SP1401_R1A_Adv_Widget::on_pushButtonSetRxFreq_clicked()
{

}

void Q_StarPoint_SP9500_SP1401_R1A_Adv_Widget::on_pushButtonGetTxTemp_clicked()
{
    double temp = 0.0;
    INT_CHECKV(SP1401_R1A->get_tx_temp(temp));
    ui->lineEditTxTemp->setText(QString("%1").arg(temp));
}

void Q_StarPoint_SP9500_SP1401_R1A_Adv_Widget::on_pushButtonGetRxTemp_clicked()
{
    double temp = 0.0;
    INT_CHECKV(SP1401_R1A->get_rx_temp(temp));
    ui->lineEditRxTemp->setText(QString("%1").arg(temp));
}

void Q_StarPoint_SP9500_SP1401_R1A_Adv_Widget::on_pushButtonGetTxDet_clicked()
{
    uint32_t det = 0;
    INT_CHECKV(SP1401_R1A->set_det_sw(sp1401::CH_TX));
    INT_CHECKV(SP1401_R1A->get_ad7680(det));
    ui->lineEditTxDet->setText(QString("%1").arg(det));
}

void Q_StarPoint_SP9500_SP1401_R1A_Adv_Widget::on_pushButtonGetRxDet_clicked()
{
    uint32_t det = 0;
    INT_CHECKV(SP1401_R1A->set_det_sw(sp1401::CH_RX));
    INT_CHECKV(SP1401_R1A->get_ad7680(det));
    ui->lineEditRxDet->setText(QString("%1").arg(det));
}

void Q_StarPoint_SP9500_SP1401_R1A_Adv_Widget::on_comboBoxRxIOSw1_activated(int index)
{
    SP1401_R1A->set_rx_io_sw1(sp1401_r1a::rx_io_sw1_t(ui->comboBoxRxIOSw1->currentIndex()));
}

void Q_StarPoint_SP9500_SP1401_R1A_Adv_Widget::on_comboBoxRxIOSw2_activated(int index)
{
    SP1401_R1A->set_rx_io_sw2(sp1401_r1a::rx_io_sw2_t(ui->comboBoxRxIOSw2->currentIndex()));
}

void Q_StarPoint_SP9500_SP1401_R1A_Adv_Widget::on_comboBoxRxIOSw_activated(int index)
{
    SP1401_R1A->set_rx_io_sw(sp1401_r1a::rx_io_sw_t(ui->comboBoxRxIOSw->currentIndex()));
}

void Q_StarPoint_SP9500_SP1401_R1A_Adv_Widget::on_comboBoxRxLNAAttSw_activated(int index)
{
    SP1401_R1A->set_rx_lna_att_sw(rx_lna_att_t(ui->comboBoxRxLNAAttSw->currentIndex()));
}

void Q_StarPoint_SP9500_SP1401_R1A_Adv_Widget::on_comboBoxRxAtt1_activated(int index)
{
    SP1401_R1A->set_rx_att1(ui->comboBoxRxAtt1->currentIndex() * 0.5);
}

void Q_StarPoint_SP9500_SP1401_R1A_Adv_Widget::on_comboBoxRxAtt2_activated(int index)
{
    SP1401_R1A->set_rx_att2(ui->comboBoxRxAtt2->currentIndex() * 10);
}

void Q_StarPoint_SP9500_SP1401_R1A_Adv_Widget::on_comboBoxIFFilterSw_activated(int index)
{
    SP1401_R1A->set_rx_if_filter_sw(sp1401::rx_if_filter_t(ui->comboBoxIFFilterSw->currentIndex()));
}

void Q_StarPoint_SP9500_SP1401_R1A_Adv_Widget::on_pushButtonAdvTxLO1_clicked()
{
    qint8 loIdx = sp1401_r1a::TX_LO_1;
    if (!dlg_ADF5355[loIdx]) {
        dlg_ADF5355[loIdx] = new Q_ADF5355_Dlg;
        dlg_ADF5355[loIdx]->_sp1401_r1a = SP1401_R1A;
        dlg_ADF5355[loIdx]->_sp1401_r1c = nullptr;
        dlg_ADF5355[loIdx]->loIdx = loIdx;
        dlg_ADF5355[loIdx]->setWindowTitle(QString("R1A/B_ADF5355_RFU%1_RF%2_TX_LO1").arg(SP3301->get_rfu_idx()).arg(RFIdx));
    }

    dlg_ADF5355[loIdx]->setVisible(true);
    dlg_ADF5355[loIdx]->showNormal();
    dlg_ADF5355[loIdx]->activateWindow();
}

void Q_StarPoint_SP9500_SP1401_R1A_Adv_Widget::on_pushButtonAdvTxLO2_clicked()
{
    qint8 loIdx = sp1401_r1a::TX_LO_2;
    if (!dlg_ADF5355[loIdx]) {
        dlg_ADF5355[loIdx] = new Q_ADF5355_Dlg;
        dlg_ADF5355[loIdx]->_sp1401_r1a = SP1401_R1A;
        dlg_ADF5355[loIdx]->_sp1401_r1c = nullptr;
        dlg_ADF5355[loIdx]->loIdx = loIdx;
        dlg_ADF5355[loIdx]->setWindowTitle(QString("R1A/B_ADF5355_RFU%1_RF%2_TX_LO2").arg(SP3301->get_rfu_idx()).arg(RFIdx));
    }

    dlg_ADF5355[loIdx]->setVisible(true);
    dlg_ADF5355[loIdx]->showNormal();
    dlg_ADF5355[loIdx]->activateWindow();
}

void Q_StarPoint_SP9500_SP1401_R1A_Adv_Widget::on_pushButtonAdvRxLO1_clicked()
{
    qint8 loIdx = sp1401_r1a::RX_LO_1;
    if (!dlg_ADF5355[loIdx]) {
        dlg_ADF5355[loIdx] = new Q_ADF5355_Dlg;
        dlg_ADF5355[loIdx]->_sp1401_r1a = SP1401_R1A;
        dlg_ADF5355[loIdx]->_sp1401_r1c = nullptr;
        dlg_ADF5355[loIdx]->loIdx = loIdx;
        dlg_ADF5355[loIdx]->setWindowTitle(QString("R1A/B_ADF5355_RFU%1_RF%2_RX_LO1").arg(SP3301->get_rfu_idx()).arg(RFIdx));
    }

    dlg_ADF5355[loIdx]->setVisible(true);
    dlg_ADF5355[loIdx]->showNormal();
    dlg_ADF5355[loIdx]->activateWindow();
}

void Q_StarPoint_SP9500_SP1401_R1A_Adv_Widget::on_pushButtonAdvRxLO2_clicked()
{
    qint8 loIdx = sp1401_r1a::RX_LO_2;
    if (!dlg_ADF5355[loIdx]) {
        dlg_ADF5355[loIdx] = new Q_ADF5355_Dlg;
        dlg_ADF5355[loIdx]->_sp1401_r1a = SP1401_R1A;
        dlg_ADF5355[loIdx]->_sp1401_r1c = nullptr;
        dlg_ADF5355[loIdx]->loIdx = loIdx;
        dlg_ADF5355[loIdx]->setWindowTitle(QString("R1A/B_ADF5355_RFU%1_RF%2_RX_LO2").arg(SP3301->get_rfu_idx()).arg(RFIdx));
    }

    dlg_ADF5355[loIdx]->setVisible(true);
    dlg_ADF5355[loIdx]->showNormal();
    dlg_ADF5355[loIdx]->activateWindow();
}

void Q_StarPoint_SP9500_SP1401_R1A_Adv_Widget::on_pushButtonSetTxLO1_clicked()
{
    uint64_t freq = 0;
    quint64 freqLO1 = freq_string_to<quint64>(ui->lineEditTxLO1->text().toStdString());
    quint64 freqLO2 = freq_string_to<quint64>(ui->lineEditTxLO2->text().toStdString());
    sp1401_r1a::tx_band_t band = sp1401_r1a::tx_band_t(ui->comboBoxTxBandSw->currentIndex());
    bool isLock = false;

    SP1401_R1A->tx_lo2freq(freqLO1,freqLO2,band,freq);

    INT_CHECKV(SP1401_R1A->set_lo(sp1401_r1a::TX_LO_1,freqLO1));
    INT_CHECKV(SP1401_R1A->det_lo(sp1401_r1a::TX_LO_1,isLock));

    ui->lineEditTxFreq->setText(QString::fromStdString(freq_string_from_uint64_t(freq)));
    ui->checkBoxTxLO1->setChecked(isLock);
}

void Q_StarPoint_SP9500_SP1401_R1A_Adv_Widget::on_pushButtonSetTxLO2_clicked()
{
    uint64_t freq = 0;
    quint64 freqLO1 = freq_string_to<quint64>(ui->lineEditTxLO1->text().toStdString());
    quint64 freqLO2 = freq_string_to<quint64>(ui->lineEditTxLO2->text().toStdString());
    sp1401_r1a::tx_band_t band = sp1401_r1a::tx_band_t(ui->comboBoxTxBandSw->currentIndex());
    bool isLock = false;

    SP1401_R1A->tx_lo2freq(freqLO1,freqLO2,band,freq);

    INT_CHECKV(SP1401_R1A->set_lo(sp1401_r1a::TX_LO_2,freqLO2));
    INT_CHECKV(SP1401_R1A->det_lo(sp1401_r1a::TX_LO_2,isLock));

    ui->lineEditTxFreq->setText(QString::fromStdString(freq_string_from_uint64_t(freq)));
    ui->checkBoxTxLO2->setChecked(isLock);
}

void Q_StarPoint_SP9500_SP1401_R1A_Adv_Widget::on_pushButtonSetRxLO1_clicked()
{
    uint64_t freq = 0;
    quint64 freqLO1 = freq_string_to<quint64>(ui->lineEditRxLO1->text().toStdString());
    quint64 freqLO2 = freq_string_to<quint64>(ui->lineEditRxLO2->text().toStdString());
    bool isLock = false;

    SP1401_R1A->rx_lo2freq(freqLO1,freqLO2,freq);

    INT_CHECKV(SP1401_R1A->set_lo(sp1401_r1a::RX_LO_1,freqLO1));
    INT_CHECKV(SP1401_R1A->det_lo(sp1401_r1a::RX_LO_1,isLock));

    ui->lineEditRxFreq->setText(QString::fromStdString(freq_string_from_uint64_t(freq)));
    ui->checkBoxRxLO1->setChecked(isLock);
}

void Q_StarPoint_SP9500_SP1401_R1A_Adv_Widget::on_pushButtonSetRxLO2_clicked()
{
    uint64_t freq = 0;
    quint64 freqLO1 = freq_string_to<quint64>(ui->lineEditRxLO1->text().toStdString());
    quint64 freqLO2 = freq_string_to<quint64>(ui->lineEditRxLO2->text().toStdString());
    bool isLock = false;

    SP1401_R1A->rx_lo2freq(freqLO1,freqLO2,freq);

    INT_CHECKV(SP1401_R1A->set_lo(sp1401_r1a::RX_LO_2,freqLO2));
    INT_CHECKV(SP1401_R1A->det_lo(sp1401_r1a::RX_LO_2,isLock));

    ui->lineEditRxFreq->setText(QString::fromStdString(freq_string_from_uint64_t(freq)));
    ui->checkBoxRxLO2->setChecked(isLock);
}
