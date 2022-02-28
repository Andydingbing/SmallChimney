#include "starpoint_sp9500_sp1401_r1c_adv_widget.h"
#include "freq_string.hpp"

using namespace ns_sp1401::r1c;

Q_StarPoint_SP9500_SP1401_R1C_Adv_Widget::Q_StarPoint_SP9500_SP1401_R1C_Adv_Widget(QWidget *parent) :
    Q_Widget(parent),
    ui(new Ui::Q_StarPoint_SP9500_SP1401_R1C_Adv_Widget)
{
    ui->setupUi(this);

    dlg_ADF5355[0] = nullptr;
    dlg_ADF5355[1] = nullptr;
    dlg_ADF5355[2] = nullptr;

    double att[SERIE_SIZE(0.0,31.5,0.5)] = {0.0};
    for (uint16_t i = 0;i < ARRAY_SIZE(att);i ++) {
        att[i] = i * 0.5;
        QString strAtt = QString("%1").arg(att[i],2,'f',1);
        ui->comboBoxTXAtt0->addItem(strAtt);
        ui->comboBoxTXAtt1->addItem(strAtt);
        ui->comboBoxTXAtt2->addItem(strAtt);
        ui->comboBoxTXAtt3->addItem(strAtt);
        ui->comboBoxRXAtt1->addItem(strAtt);
        ui->comboBoxRXAtt2->addItem(strAtt);
        ui->comboBoxRXAtt3->addItem(strAtt);
    }
    ui->comboBoxTXAtt0->setMaxVisibleItems(20);
    ui->comboBoxTXAtt1->setMaxVisibleItems(20);
    ui->comboBoxTXAtt2->setMaxVisibleItems(20);
    ui->comboBoxTXAtt3->setMaxVisibleItems(20);
    ui->comboBoxTXAtt0->setCurrentIndex(int(5.0 / 0.5));
    ui->comboBoxTXAtt1->setCurrentIndex(int(20.0 / 0.5));
    ui->comboBoxTXAtt2->setCurrentIndex(int(0.0 / 0.5));
    ui->comboBoxTXAtt3->setCurrentIndex(int(1.0 / 0.5));

    ui->comboBoxRXAtt1->setMaxVisibleItems(20);
    ui->comboBoxRXAtt2->setMaxVisibleItems(20);
    ui->comboBoxRXAtt3->setMaxVisibleItems(20);
    ui->comboBoxRXAtt1->setCurrentIndex(int(5.0 / 0.5));
    ui->comboBoxRXAtt2->setCurrentIndex(int(15.0 / 0.5));
    ui->comboBoxRXAtt3->setCurrentIndex(int(10.0 / 0.5));
}

Q_StarPoint_SP9500_SP1401_R1C_Adv_Widget::~Q_StarPoint_SP9500_SP1401_R1C_Adv_Widget()
{
    delete ui;
}

void Q_StarPoint_SP9500_SP1401_R1C_Adv_Widget::on_comboBoxTXIOSw2_activated(int index)
{
    SP1401_R1C->set_tx_io_sw2(sp1401_r1c::tx_io_sw2_t(index));
}

void Q_StarPoint_SP9500_SP1401_R1C_Adv_Widget::on_comboBoxTXIOSw1_activated(int index)
{
    SP1401_R1C->set_tx_io_sw1(sp1401_r1c::tx_io_sw1_t(index));
}

void Q_StarPoint_SP9500_SP1401_R1C_Adv_Widget::on_comboBoxTXAtt3_activated(int index)
{
    SP1401_R1C->set_tx_att3(index * 0.5);
}

void Q_StarPoint_SP9500_SP1401_R1C_Adv_Widget::on_comboBoxTXAtt2_activated(int index)
{
    SP1401_R1C->set_tx_att2(index * 0.5);
}

void Q_StarPoint_SP9500_SP1401_R1C_Adv_Widget::on_comboBoxTXAtt1_activated(int index)
{
    SP1401_R1C->set_tx_att1(index * 0.5);
}

void Q_StarPoint_SP9500_SP1401_R1C_Adv_Widget::on_comboBoxTXFilterSw1_activated(int index)
{
    Q_UNUSED(index);

    sp1401_r1c::tx_filter_t filter1 =
            sp1401_r1c::tx_filter_t(ui->comboBoxTXFilterSw1->currentIndex());
    sp1401_r1c::tx_filter_t filter2 =
            sp1401_r1c::tx_filter_t(ui->comboBoxTXFilterSw2->currentIndex());
    SP1401_R1C->set_tx_filter_sw(filter1,filter2);
}

void Q_StarPoint_SP9500_SP1401_R1C_Adv_Widget::on_comboBoxTXFilterSw2_activated(int index)
{
    Q_UNUSED(index);

    sp1401_r1c::tx_filter_t filter1 =
            sp1401_r1c::tx_filter_t(ui->comboBoxTXFilterSw1->currentIndex());
    sp1401_r1c::tx_filter_t filter2 =
            sp1401_r1c::tx_filter_t(ui->comboBoxTXFilterSw2->currentIndex());
    SP1401_R1C->set_tx_filter_sw(filter1,filter2);
}

void Q_StarPoint_SP9500_SP1401_R1C_Adv_Widget::on_comboBoxTXAtt0_activated(int index)
{
    SP1401_R1C->set_tx_att0(index * 0.5);
}

void Q_StarPoint_SP9500_SP1401_R1C_Adv_Widget::on_pushButtonModEn_clicked()
{
    bool en = ui->pushButtonModEn->isChecked();
    SP1401_R1C->set_tx_modulator_en(en);
    ui->pushButtonModEn->setChecked(en);
    ui->pushButtonModEn->setText(en ? tr("En") : tr("Dis"));
}

void Q_StarPoint_SP9500_SP1401_R1C_Adv_Widget::on_pushButtonPowerOn_clicked()
{
    bool en = ui->pushButtonPowerOn->isChecked();
    SP1401_R1C->set_pwr_en(en);
    ui->pushButtonPowerOn->setChecked(en);
    ui->pushButtonPowerOn->setText(en ? tr("On") : tr("Off"));
}

void Q_StarPoint_SP9500_SP1401_R1C_Adv_Widget::on_comboBoxLoopSw_activated(int index)
{
    SP1401_R1C->set_loop_sw(sp1401_r1c::loop_sw_t(index));
}

void Q_StarPoint_SP9500_SP1401_R1C_Adv_Widget::on_pushButtonSetTXLO3_clicked()
{
    RD_TRY
    uint64_t freq = 0;
    quint64 freqLO1 = freq_string_to_uint64_t(ui->lineEditTXLO1->text().toStdString());
    quint64 freqLO2 = freq_string_to_uint64_t(ui->lineEditTXLO2->text().toStdString());
    quint64 freqLO3 = freq_string_to_uint64_t(ui->lineEditTXLO3->text().toStdString());
    bool isLock = false;

    sp1401_r1c::tx_lo2freq(freqLO1,freqLO2,freqLO3,freq);

    SP1401_R1C->set_adf5355(sp1401_r1c::TX_LO_3,freqLO3);
    SP1401_R1C->det_lo(sp1401_r1c::TX_LO_3,isLock);

    ui->lineEditTXFreq->setText(QString::fromStdString(freq_string_from_uint64_t(freq)));
    ui->checkBoxTXLO3->setChecked(isLock);
    RD_CATCH_ALL_EXCEPTION
}

void Q_StarPoint_SP9500_SP1401_R1C_Adv_Widget::on_pushButtonSetTXLO2_clicked()
{
    RD_TRY
    uint64_t freq = 0;
    quint64 freqLO1 = freq_string_to_uint64_t(ui->lineEditTXLO1->text().toStdString());
    quint64 freqLO2 = freq_string_to_uint64_t(ui->lineEditTXLO2->text().toStdString());
    quint64 freqLO3 = freq_string_to_uint64_t(ui->lineEditTXLO3->text().toStdString());
    bool isLock = false;

    sp1401_r1c::tx_lo2freq(freqLO1,freqLO2,freqLO3,freq);

    SP1401_R1C->set_adf5355(sp1401_r1c::TX_LO_2,freqLO2);
    SP1401_R1C->det_lo(sp1401_r1c::TX_LO_2,isLock);

    ui->lineEditTXFreq->setText(QString::fromStdString(freq_string_from_uint64_t(freq)));
    ui->checkBoxTXLO2->setChecked(isLock);
    RD_CATCH_ALL_EXCEPTION
}

void Q_StarPoint_SP9500_SP1401_R1C_Adv_Widget::on_pushButtonSetTXLO1_clicked()
{
    RD_TRY
    uint64_t freq = 0;
    quint64 freqLO1 = freq_string_to_uint64_t(ui->lineEditTXLO1->text().toStdString());
    quint64 freqLO2 = freq_string_to_uint64_t(ui->lineEditTXLO2->text().toStdString());
    quint64 freqLO3 = freq_string_to_uint64_t(ui->lineEditTXLO3->text().toStdString());
    bool isLock = false;

    sp1401_r1c::tx_lo2freq(freqLO1,freqLO2,freqLO3,freq);

    SP1401_R1C->set_adf5355(sp1401_r1c::TX_LO_1,freqLO1);
    SP1401_R1C->det_lo(sp1401_r1c::TX_LO_1,isLock);

    ui->lineEditTXFreq->setText(QString::fromStdString(freq_string_from_uint64_t(freq)));
    ui->checkBoxTXLO2->setChecked(isLock);
    RD_CATCH_ALL_EXCEPTION
}

void Q_StarPoint_SP9500_SP1401_R1C_Adv_Widget::on_pushButtonSetRXLO1_clicked()
{
    RD_TRY
    uint64_t freq = 0;
    quint64 freqLO1 = freq_string_to_uint64_t(ui->lineEditRXLO1->text().toStdString());
    quint64 freqLO2 = freq_string_to_uint64_t(ui->lineEditTXLO2->text().toStdString());
    bool isLock = false;

    sp1401_r1c::rx_lo2freq(freqLO1,freqLO2,freq);

    SP1401_R1C->set_adf5355(sp1401_r1c::RX_LO_1,freqLO1);
    SP1401_R1C->det_lo(sp1401_r1c::RX_LO_1,isLock);

    ui->lineEditRXFreq->setText(QString::fromStdString(freq_string_from_uint64_t(freq)));
    ui->checkBoxRXLO1->setChecked(isLock);
    RD_CATCH_ALL_EXCEPTION
}

void Q_StarPoint_SP9500_SP1401_R1C_Adv_Widget::on_pushButtonAdvTXLO3_clicked()
{
    quint8 dlgIdx = 1;
    if (dlg_ADF5355[dlgIdx] == nullptr) {
        dlg_ADF5355[dlgIdx] = new Q_ADF5355_Dlg;
        dlg_ADF5355[dlgIdx]->_sp1401_r1a = nullptr;
        dlg_ADF5355[dlgIdx]->_sp1401_r1c = SP1401_R1C;
        dlg_ADF5355[dlgIdx]->loIdx = sp1401_r1c::TX_LO_3;
        dlg_ADF5355[dlgIdx]->setWindowTitle(QString("R1C/D/E_ADF5355_RFU%1_RF%2_TX_LO3").arg(RFUIdx).arg(RFIdx));
    }

    dlg_ADF5355[dlgIdx]->setVisible(true);
    dlg_ADF5355[dlgIdx]->showNormal();
    dlg_ADF5355[dlgIdx]->activateWindow();
}

void Q_StarPoint_SP9500_SP1401_R1C_Adv_Widget::on_pushButtonAdvTXLO2_clicked()
{
    quint8 dlgIdx = 0;
    if (dlg_ADF5355[dlgIdx] == nullptr) {
        dlg_ADF5355[dlgIdx] = new Q_ADF5355_Dlg;
        dlg_ADF5355[dlgIdx]->_sp1401_r1a = nullptr;
        dlg_ADF5355[dlgIdx]->_sp1401_r1c = SP1401_R1C;
        dlg_ADF5355[dlgIdx]->loIdx = sp1401_r1c::TX_LO_2;
        dlg_ADF5355[dlgIdx]->setWindowTitle(QString("R1C/D/E_ADF5355_RFU%1_RF%2_TX_LO2").arg(RFUIdx).arg(RFIdx));
    }

    dlg_ADF5355[dlgIdx]->setVisible(true);
    dlg_ADF5355[dlgIdx]->showNormal();
    dlg_ADF5355[dlgIdx]->activateWindow();
}

void Q_StarPoint_SP9500_SP1401_R1C_Adv_Widget::on_pushButtonAdvTXLO1_clicked()
{

}

void Q_StarPoint_SP9500_SP1401_R1C_Adv_Widget::on_pushButtonAdvRXLO1_clicked()
{
    quint8 dlgIdx = 2;
    if (dlg_ADF5355[dlgIdx] == nullptr) {
        dlg_ADF5355[dlgIdx] = new Q_ADF5355_Dlg;
        dlg_ADF5355[dlgIdx]->_sp1401_r1a = nullptr;
        dlg_ADF5355[dlgIdx]->_sp1401_r1c = SP1401_R1C;
        dlg_ADF5355[dlgIdx]->loIdx = sp1401_r1c::RX_LO_1;
        dlg_ADF5355[dlgIdx]->setWindowTitle(QString("R1C/D/E_ADF5355_RFU%1_RF%2_RX_LO1").arg(RFUIdx).arg(RFIdx));
    }

    dlg_ADF5355[dlgIdx]->setVisible(true);
    dlg_ADF5355[dlgIdx]->showNormal();
    dlg_ADF5355[dlgIdx]->activateWindow();
}

void Q_StarPoint_SP9500_SP1401_R1C_Adv_Widget::on_comboBoxIOMode_activated(int index)
{
    SP1401_R1C->set_io_mode(io_mode_t::_from_index(index));
}

void Q_StarPoint_SP9500_SP1401_R1C_Adv_Widget::on_checkBoxTXLED_clicked(bool checked)
{
    Q_UNUSED(checked);

    bool txLed = ui->checkBoxTXLED->isChecked();
    bool rxLed = ui->checkBoxRXLED->isChecked();
    SP1401_R1C->set_led(txLed,rxLed);
}

void Q_StarPoint_SP9500_SP1401_R1C_Adv_Widget::on_checkBoxRXLED_clicked(bool checked)
{
    Q_UNUSED(checked);

    bool txLed = ui->checkBoxTXLED->isChecked();
    bool rxLed = ui->checkBoxRXLED->isChecked();
    SP1401_R1C->set_led(txLed,rxLed);
}

void Q_StarPoint_SP9500_SP1401_R1C_Adv_Widget::on_pushButtonSetTXFreq_clicked()
{
    RD_TRY
    SP1401_R1C->set_tx_freq(freq_string_to_uint64_t(ui->lineEditTXFreq->text().toStdString()));
    RD_CATCH_ALL_EXCEPTION
}

void Q_StarPoint_SP9500_SP1401_R1C_Adv_Widget::on_pushButtonSetRXFreq_clicked()
{
    RD_TRY
    SP1401_R1C->set_rx_freq(freq_string_to_uint64_t(ui->lineEditRXFreq->text().toStdString()));
    RD_CATCH_ALL_EXCEPTION
}

void Q_StarPoint_SP9500_SP1401_R1C_Adv_Widget::on_pushButtonGetAllTemp_clicked()
{
    double temp[8] = {0.0};
    QLineEdit *lineEditTemp[8] = {
        ui->lineEditTemp0,
        ui->lineEditTemp1,
        ui->lineEditTemp2,
        ui->lineEditTemp3,
        ui->lineEditTemp4,
        ui->lineEditTemp5,
        ui->lineEditTemp6,
        ui->lineEditTemp7
    };
    for (quint8 i = 0;i < 8;i ++) {
        SP1401_R1C->get_temp(i,temp[i]);
        lineEditTemp[i]->setText(QString("%1").arg(temp[i]));
    }
}

void Q_StarPoint_SP9500_SP1401_R1C_Adv_Widget::on_pushButtonGetAllDet_clicked()
{
    DECLARE_R1C_GETAD7680_FUNCTION_MAP

    uint32_t det[4] = {0};
    QLineEdit *lineEditDet[4] = {
        ui->lineEditDetRXComb,
        ui->lineEditDetRXIF1,
        ui->lineEditDetRXIF2,
        ui->lineEditDetTXIF2
    };
    for (quint8 i = 0;i < 4;i ++) {
        (SP1401_R1C->*get_r1c_ad7680[i])(det[i]);
        lineEditDet[i]->setText(QString("%1").arg(det[i]));
    }
}

void Q_StarPoint_SP9500_SP1401_R1C_Adv_Widget::on_comboBoxRXIOSw1_activated(int index)
{
    SP1401_R1C->set_rx_io_sw1(sp1401_r1c::rx_io_sw1_t(index));
}

void Q_StarPoint_SP9500_SP1401_R1C_Adv_Widget::on_comboBoxRXIOSw2_activated(int index)
{
    SP1401_R1C->set_rx_io_sw2(sp1401_r1c::rx_io_sw2_t(index));
}

void Q_StarPoint_SP9500_SP1401_R1C_Adv_Widget::on_comboBoxRXIOSw3_activated(int index)
{
    SP1401_R1C->set_rx_io_sw3(sp1401_r1c::rx_io_sw3_t(index));
}

void Q_StarPoint_SP9500_SP1401_R1C_Adv_Widget::on_comboBoxLNAAttSw_activated(int index)
{
    SP1401_R1C->set_rx_lna_att_sw(rx_lna_att_t(index));
}

void Q_StarPoint_SP9500_SP1401_R1C_Adv_Widget::on_comboBoxAtt019Sw_activated(int index)
{
    SP1401_R1C->set_rx_att_019_sw(rx_att_019_t(index));
}

void Q_StarPoint_SP9500_SP1401_R1C_Adv_Widget::on_comboBoxRXAtt1_activated(int index)
{
    SP1401_R1C->set_rx_att1(index * 0.5);
}

void Q_StarPoint_SP9500_SP1401_R1C_Adv_Widget::on_comboBoxRXAtt2_activated(int index)
{
    SP1401_R1C->set_rx_att2(index * 0.5);
}

void Q_StarPoint_SP9500_SP1401_R1C_Adv_Widget::on_comboBoxIFFilterSw_activated(int index)
{
    SP1401_R1C->set_rx_if_filter_sw(sp1401_r1c::rx_if_filter_t(index));
}

void Q_StarPoint_SP9500_SP1401_R1C_Adv_Widget::on_comboBoxRXAtt3_activated(int index)
{
    SP1401_R1C->set_rx_att3(index * 0.5);
}
