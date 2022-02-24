#include "sp1403_r1b_widget.h"
#include "ui_sp1403_r1b.h"
#include "lmx2594_dlg.h"
#include "reg_def_sp9500x.h"

using namespace ns_sp1403;
using namespace ns_sp1403::r1b;

Q_SP1403_R1B_Widget::Q_SP1403_R1B_Widget(QWidget *parent) :
    Q_RD_Widget(parent),
    ui(new Ui::Q_SP1403_R1B_Widget)
{
    ui->setupUi(this);

    dlg_TX_LMX2594_0 = nullptr;
    dlg_TX_LMX2594_1 = nullptr;
    dlg_RX_LMX2594_0 = nullptr;

    QString att;
    for (quint8 i = 0;i < SERIE_SIZE(0,31.5,0.5);i ++) {
        att = QString("%1").arg(i * 0.5);
        ui->comboBoxTX0Att0->addItem(att);
        ui->comboBoxTX0Att1->addItem(att);
        ui->comboBoxTX0Att2->addItem(att);
        ui->comboBoxTX0Att3->addItem(att);
        ui->comboBoxTX1Att0->addItem(att);
        ui->comboBoxTX1Att1->addItem(att);
        ui->comboBoxTX1Att2->addItem(att);
        ui->comboBoxTX1Att3->addItem(att);
        ui->comboBoxRXAtt0->addItem(att);
        ui->comboBoxRXAtt1->addItem(att);
    }

    COMBOBOX_MAP_FROM_BETTER_ENUM(ui->comboBoxTX0IOMode,io_mode);
    COMBOBOX_MAP_FROM_BETTER_ENUM(ui->comboBoxTX1IOMode,io_mode);
    COMBOBOX_MAP_FROM_BETTER_ENUM(ui->comboBoxTX0LED,led_t);
    COMBOBOX_MAP_FROM_BETTER_ENUM(ui->comboBoxTX1LED,led_t);
    COMBOBOX_MAP_FROM_BETTER_ENUM(ui->comboBoxTX0Sw1,sp1403_r1b::tx_sw1_t);
    COMBOBOX_MAP_FROM_BETTER_ENUM(ui->comboBoxTX1Sw1,sp1403_r1b::tx_sw1_t);
    COMBOBOX_MAP_FROM_BETTER_ENUM(ui->comboBoxTX0Sw2,sp1403_r1b::tx_sw2_t);
    COMBOBOX_MAP_FROM_BETTER_ENUM(ui->comboBoxTX1Sw2,sp1403_r1b::tx_sw2_t);
    COMBOBOX_MAP_FROM_BETTER_ENUM(ui->comboBoxTX0Sw3,sp1403_r1b::tx_sw3_t);
    COMBOBOX_MAP_FROM_BETTER_ENUM(ui->comboBoxTX1Sw3,sp1403_r1b::tx_sw3_t);
    COMBOBOX_MAP_FROM_BETTER_ENUM(ui->comboBoxTX0Sw4,sp1403_r1b::tx_sw4_t);
    COMBOBOX_MAP_FROM_BETTER_ENUM(ui->comboBoxTX1Sw4,sp1403_r1b::tx_sw4_t);
    COMBOBOX_MAP_FROM_BETTER_ENUM(ui->comboBoxTX0Sw5,sp1403_r1b::tx0_sw5_t);
    COMBOBOX_MAP_FROM_BETTER_ENUM(ui->comboBoxTX1Sw5,sp1403_r1b::tx1_sw5_t);
    COMBOBOX_MAP_FROM_BETTER_ENUM(ui->comboBoxTX0Sw6,sp1403_r1b::tx0_sw6_t);
    COMBOBOX_MAP_FROM_BETTER_ENUM(ui->comboBoxTX1Sw6,sp1403_r1b::tx1_sw6_t);
    COMBOBOX_MAP_FROM_BETTER_ENUM(ui->comboBoxTX0Sw7,sp1403_r1b::tx0_sw7_t);
    COMBOBOX_MAP_FROM_BETTER_ENUM(ui->comboBoxTX0Sw8,sp1403_r1b::tx0_sw8_t);
    COMBOBOX_MAP_FROM_BETTER_ENUM(ui->comboBoxTX0Sw9,sp1403_r1b::tx0_sw9_t);
    COMBOBOX_MAP_FROM_BETTER_ENUM(ui->comboBoxTX0Sw10,sp1403_r1b::tx0_sw10_t);
    COMBOBOX_MAP_FROM_BETTER_ENUM(ui->comboBoxTX0Sw11,sp1403_r1b::tx0_sw11_t);
    COMBOBOX_MAP_FROM_BETTER_ENUM(ui->comboBoxTX0Sw12,sp1403_r1b::tx0_sw12_t);

    COMBOBOX_MAP_FROM_BETTER_ENUM(ui->comboBoxRXLNAAttSw,sp1403_r1b::rx_lna_att_t);
    COMBOBOX_MAP_FROM_BETTER_ENUM(ui->comboBoxRXSw1,sp1403_r1b::rx_sw1_t);
    COMBOBOX_MAP_FROM_BETTER_ENUM(ui->comboBoxRXSw2,sp1403_r1b::rx_sw2_t);
    COMBOBOX_MAP_FROM_BETTER_ENUM(ui->comboBoxRXSw3,sp1403_r1b::rx_sw3_t);

    COMBOBOX_MAP_FROM_BETTER_ENUM(ui->comboBoxBWSw,sp1403_r1b::rx_bw_t);
    COMBOBOX_MAP_FROM_BETTER_ENUM(ui->comboBoxDet,sp1403_r1b::det_sw_t);
    COMBOBOX_MAP_FROM_BETTER_ENUM(ui->comboBoxRXSw7,sp1403_r1b::rx_sw7_t);

    ui->lineEditS6_Addr->setText("0x0");
}

Q_SP1403_R1B_Widget::~Q_SP1403_R1B_Widget()
{
    delete ui;
    SAFE_DEL(dlg_TX_LMX2594_0);
    SAFE_DEL(dlg_TX_LMX2594_1);
    SAFE_DEL(dlg_RX_LMX2594_0);
}

void Q_SP1403_R1B_Widget::init()
{
    RD_TRY
    ui->lineEditTXFreq->setText(QString::fromStdString(SP1403_R1B->tx_freq_string()));
    ui->lineEditRXFreq->setText(QString::fromStdString(SP1403_R1B->rx_freq_string()));
    RD_LOG_ALL_EXCEPTION

    updateIOMode();
    updateAtt();
    updateTXSw();
    updateTXLO();
    updateRXSw();
    updateRXLO();

    r1b::det_sw_t detSw = r1b::det_sw_t::TX;
    INT_CHECKV(SP1403_R1B->get_det_sw(detSw));
    ui->comboBoxDet->setCurrentIndex(detSw);
}

void Q_SP1403_R1B_Widget::updateIOMode()
{
    led_t ledTX0 = led_t::Red;
    led_t ledTX1 = led_t::Red;
    INT_CHECKV(SP1403_R1B->get_led(port_t::TX0,ledTX0));
    INT_CHECKV(SP1403_R1B->get_led(port_t::TX1,ledTX1));
    ui->comboBoxTX0IOMode->setCurrentIndex(SP1403_R1B->io_mode(TX0));
    ui->comboBoxTX1IOMode->setCurrentIndex(SP1403_R1B->io_mode(TX1));
    ui->comboBoxTX0LED->setCurrentIndex(ledTX0);
    ui->comboBoxTX1LED->setCurrentIndex(ledTX1);
}

void Q_SP1403_R1B_Widget::updateAtt()
{
    double att = 0.0;
    INT_CHECKV(SP1403_R1B->get_att(sp1403_r1b::att_t::TX0_ATT0,att));
    ui->comboBoxTX0Att0->setCurrentIndex(int(att * 2));

    INT_CHECKV(SP1403_R1B->get_att(sp1403_r1b::att_t::TX0_ATT1,att));
    ui->comboBoxTX0Att1->setCurrentIndex(int(att * 2));

    INT_CHECKV(SP1403_R1B->get_att(sp1403_r1b::att_t::TX0_ATT2,att));
    ui->comboBoxTX0Att2->setCurrentIndex(int(att * 2));

    INT_CHECKV(SP1403_R1B->get_att(sp1403_r1b::att_t::TX0_ATT3,att));
    ui->comboBoxTX0Att3->setCurrentIndex(int(att * 2));

    INT_CHECKV(SP1403_R1B->get_att(sp1403_r1b::att_t::TX1_ATT0,att));
    ui->comboBoxTX1Att0->setCurrentIndex(int(att * 2));

    INT_CHECKV(SP1403_R1B->get_att(sp1403_r1b::att_t::TX1_ATT1,att));
    ui->comboBoxTX1Att1->setCurrentIndex(int(att * 2));

    INT_CHECKV(SP1403_R1B->get_att(sp1403_r1b::att_t::TX1_ATT2,att));
    ui->comboBoxTX1Att2->setCurrentIndex(int(att * 2));

    INT_CHECKV(SP1403_R1B->get_att(sp1403_r1b::att_t::TX1_ATT3,att));
    ui->comboBoxTX1Att3->setCurrentIndex(int(att * 2));

    INT_CHECKV(SP1403_R1B->get_att(sp1403_r1b::att_t::RX_ATT0,att));
    ui->comboBoxRXAtt0->setCurrentIndex(int(att * 2));

    INT_CHECKV(SP1403_R1B->get_att(sp1403_r1b::att_t::RX_ATT1,att));
    ui->comboBoxRXAtt1->setCurrentIndex(int(att * 2));
}

void Q_SP1403_R1B_Widget::updateTXSw()
{
    SP1403_S6_REG_DECL(0x1);
    SP1403_S6_REG_DECL(0x2);
    INT_CHECKV(SP1403_R1B->get_s6_reg(0x1,SP1403_S6_REG_DATA(0x1)));
    INT_CHECKV(SP1403_R1B->get_s6_reg(0x2,SP1403_S6_REG_DATA(0x2)));

    ui->comboBoxTX0Sw1->setCurrentIndex(SP1403_S6_REG(0x1).tx0_sw1);
    ui->comboBoxTX0Sw2->setCurrentIndex(SP1403_S6_REG(0x1).tx0_sw2);
    ui->comboBoxTX0Sw3->setCurrentIndex(SP1403_S6_REG(0x1).tx0_sw3);
    ui->comboBoxTX0Sw4->setCurrentIndex(SP1403_S6_REG(0x1).tx0_sw4);
    ui->comboBoxTX0Sw5->setCurrentIndex(SP1403_S6_REG(0x1).tx0_sw5);
    ui->comboBoxTX0Sw6->setCurrentIndex(SP1403_S6_REG(0x1).tx0_sw6);
    ui->comboBoxTX0Sw7->setCurrentIndex(SP1403_S6_REG(0x1).tx0_sw7);
    ui->comboBoxTX0Sw8->setCurrentIndex(SP1403_S6_REG(0x1).tx0_sw8);
    ui->comboBoxTX0Sw9->setCurrentIndex(SP1403_S6_REG(0x1).tx0_sw9);
    ui->comboBoxTX0Sw10->setCurrentIndex(SP1403_S6_REG(0x1).tx0_sw10);
    ui->comboBoxTX0Sw11->setCurrentIndex(SP1403_S6_REG(0x1).tx0_sw11);
    ui->comboBoxTX0Sw12->setCurrentIndex(SP1403_S6_REG(0x1).tx0_sw12);

    ui->comboBoxTX1Sw1->setCurrentIndex(SP1403_S6_REG(0x2).tx1_sw1);
    ui->comboBoxTX1Sw2->setCurrentIndex(SP1403_S6_REG(0x2).tx1_sw2);
    ui->comboBoxTX1Sw3->setCurrentIndex(SP1403_S6_REG(0x2).tx1_sw3);
    ui->comboBoxTX1Sw4->setCurrentIndex(SP1403_S6_REG(0x2).tx1_sw4);
    ui->comboBoxTX1Sw5->setCurrentIndex(SP1403_S6_REG(0x2).tx1_sw5);
    ui->comboBoxTX1Sw6->setCurrentIndex(SP1403_S6_REG(0x2).tx1_sw6);
}

void Q_SP1403_R1B_Widget::updateTXLO()
{
    RD_TRY
    ui->lineEditTXLMX2594_0_Freq->setText(QString::fromStdString(SP1403_R1B->tx_lmx2594_0_freq_string()));
    ui->lineEditTXLMX2594_1_Freq->setText(QString::fromStdString(SP1403_R1B->tx_lmx2594_1_freq_string()));
    RD_LOG_ALL_EXCEPTION
}

void Q_SP1403_R1B_Widget::updateRXSw()
{
    sp1403_r1b::rx_lna_att_t RX_LNAAttSw = sp1403_r1b::rx_lna_att_t::RX_ATT_0;
    r1b::rx_bw_t RX_BW = r1b::rx_bw_t::_800M;
    sp1403_r1b::rx_sw1_t RX_Sw1 = sp1403_r1b::rx_sw1_t::TO_ATT;
    sp1403_r1b::rx_sw2_t RX_Sw2 = sp1403_r1b::rx_sw2_t::TO_ATT_0;
    sp1403_r1b::rx_sw3_t RX_Sw3 = sp1403_r1b::rx_sw3_t::_300_1000;
    sp1403_r1b::rx_sw7_t RX_Sw7 = sp1403_r1b::rx_sw7_t::_300_1000;

    INT_CHECKV(SP1403_R1B->get_rx_lna_att_sw(RX_LNAAttSw));
    INT_CHECKV(SP1403_R1B->get_rx_bw(RX_BW));
    INT_CHECKV(SP1403_R1B->get_rx_sw1(RX_Sw1));
    INT_CHECKV(SP1403_R1B->get_rx_sw2(RX_Sw2));
    INT_CHECKV(SP1403_R1B->get_rx_sw3(RX_Sw3));
    INT_CHECKV(SP1403_R1B->get_rx_sw7(RX_Sw7));

    ui->comboBoxRXLNAAttSw->setCurrentIndex(RX_LNAAttSw._to_integral());
    ui->comboBoxBWSw->setCurrentIndex(RX_BW._to_integral());
    ui->comboBoxRXSw1->setCurrentIndex(RX_Sw1._to_index());
    ui->comboBoxRXSw2->setCurrentIndex(RX_Sw2._to_index());
    ui->comboBoxRXSw3->setCurrentIndex(RX_Sw3._to_index());
    ui->comboBoxRXSw7->setCurrentIndex(RX_Sw7._to_index());
}

void Q_SP1403_R1B_Widget::updateRXLO()
{
    RD_TRY
    ui->lineEditRXLMX2594_0_Freq->setText(QString::fromStdString(SP1403_R1B->rx_lmx2594_0_freq_string()));
    RD_LOG_ALL_EXCEPTION
}

void Q_SP1403_R1B_Widget::on_lineEditTXFreq_textEdited(const QString &arg1)
{
    RD_TRY
    SP1403_R1B->set_tx_freq(freq_string_to<quint64>(arg1.toStdString()));
    RD_LOG_ALL_EXCEPTION

    updateTXSw();
    updateTXLO();
}

void Q_SP1403_R1B_Widget::on_lineEditTXLMX2594_0_Freq_textEdited(const QString &arg1)
{
    RD_TRY
    SP1403_R1B->set_lo(TX_LMX2594_0,freq_string_to<quint64>(arg1.toStdString()));
    RD_LOG_ALL_EXCEPTION
}

void Q_SP1403_R1B_Widget::on_pushButtonTXLMX2594_0_Adv_clicked()
{
    if (dlg_TX_LMX2594_0 == nullptr) {
        dlg_TX_LMX2594_0 = new Q_LMX2594_Dlg();
        dlg_TX_LMX2594_0->RF = SP1403_R1B;
        dlg_TX_LMX2594_0->LO = TX_LMX2594_0;
        dlg_TX_LMX2594_0->setWindowTitle(QString(""));
    }
    dlg_TX_LMX2594_0->setVisible(true);
    dlg_TX_LMX2594_0->showNormal();
    dlg_TX_LMX2594_0->activateWindow();
}

void Q_SP1403_R1B_Widget::on_lineEditTXLMX2594_1_Freq_textEdited(const QString &arg1)
{
    RD_TRY
    SP1403_R1B->set_lo(TX_LMX2594_1,freq_string_to<quint64>(arg1.toStdString()));
    RD_LOG_ALL_EXCEPTION
}

void Q_SP1403_R1B_Widget::on_pushButtonTXLMX2594_1_Adv_clicked()
{
    if (dlg_TX_LMX2594_1 == nullptr) {
        dlg_TX_LMX2594_1 = new Q_LMX2594_Dlg();
        dlg_TX_LMX2594_1->RF = SP1403_R1B;
        dlg_TX_LMX2594_1->LO = TX_LMX2594_1;
        dlg_TX_LMX2594_1->setWindowTitle(QString(""));
    }
    dlg_TX_LMX2594_1->setVisible(true);
    dlg_TX_LMX2594_1->showNormal();
    dlg_TX_LMX2594_1->activateWindow();
}

void Q_SP1403_R1B_Widget::on_lineEditRXFreq_textEdited(const QString &arg1)
{
    RD_TRY
    SP1403_R1B->set_rx_freq(freq_string_to<quint64>(arg1.toStdString()));
    RD_LOG_ALL_EXCEPTION

    updateRXSw();
    updateRXLO();
}

void Q_SP1403_R1B_Widget::on_comboBoxRXLNAAttSw_activated(int index)
{
    SP1403_R1B->set_rx_lna_att_sw(sp1403_r1b::rx_lna_att_t::_from_index(index));
}

void Q_SP1403_R1B_Widget::on_comboBoxRXAtt0_activated(int index)
{
    SP1403_R1B->set_att(sp1403_r1b::att_t::RX_ATT0,index / 2.0);
}

void Q_SP1403_R1B_Widget::on_comboBoxRXAtt1_activated(int index)
{
    SP1403_R1B->set_att(sp1403_r1b::att_t::RX_ATT1,index / 2.0);
}

void Q_SP1403_R1B_Widget::on_comboBoxBWSw_activated(int index)
{
    SP1403_R1B->set_rx_bw(sp1403_r1b::rx_bw_t::_from_integral(index));
}

void Q_SP1403_R1B_Widget::on_lineEditRXLMX2594_0_Freq_textEdited(const QString &arg1)
{
    RD_TRY
    SP1403_R1B->set_lo(RX_LMX2594_0,freq_string_to<quint64>(arg1.toStdString()));
    RD_LOG_ALL_EXCEPTION
}

void Q_SP1403_R1B_Widget::on_pushButtonRXLMX2594_0_Adv_clicked()
{
    if (dlg_RX_LMX2594_0 == nullptr) {
        dlg_RX_LMX2594_0 = new Q_LMX2594_Dlg();
        dlg_RX_LMX2594_0->RF = SP1403_R1B;
        dlg_RX_LMX2594_0->LO = RX_LMX2594_0;
        dlg_RX_LMX2594_0->setWindowTitle(QString(""));
    }
    dlg_RX_LMX2594_0->setVisible(true);
    dlg_RX_LMX2594_0->showNormal();
    dlg_RX_LMX2594_0->activateWindow();
}

void Q_SP1403_R1B_Widget::on_comboBoxDet_activated(int index)
{
    SP1403_R1B->set_det_sw(r1b::det_sw_t::_from_index(index));
}

void Q_SP1403_R1B_Widget::on_pushButtonGetDet_clicked()
{
    uint16_t det = 0;
    SP1403_R1B->get_ad7680(det);
    ui->lineEditDet->setText(QString("%1").arg(det));
}

void Q_SP1403_R1B_Widget::on_pushButtonGetSN_clicked()
{
//    RF_DT3308_PTR_CHECK
//    char sn[32] = {0};
//    memset(sn,0,sizeof(sn));
//    INT_CHECKV(SP1401R1A()->get_sn_major(sn));
//    ui->lineEditSN->setText(QString("%1").arg(sn));
}

void Q_SP1403_R1B_Widget::on_pushButtonGetTemp0_clicked()
{
    double tempPA0 = 0.0;
    double tempPA1 = 0.0;
    double tempLNA = 0.0;

    SP1403_R1B->get_temp(sp1403_r1b::temp_t::TX0_PA,tempPA0);
    SP1403_R1B->get_temp(sp1403_r1b::temp_t::TX1_PA,tempPA1);
    SP1403_R1B->get_temp(sp1403_r1b::temp_t::RX_LNA,tempLNA);
    ui->lineEditTempPA0->setText(QString("%1").arg(tempPA0));
    ui->lineEditTempPA1->setText(QString("%1").arg(tempPA1));
    ui->lineEditTempLNA->setText(QString("%1").arg(tempLNA));
}

void Q_SP1403_R1B_Widget::on_pushButtonGetTemp1_clicked()
{
    double tempLO = 0.0;
    double tempSw = 0.0;

    SP1403_R1B->get_temp(sp1403_r1b::temp_t::TX_LO1,tempLO);
    SP1403_R1B->get_temp(sp1403_r1b::temp_t::TX0_SW,tempSw);
    ui->lineEditTempLO->setText(QString("%1").arg(tempLO));
    ui->lineEditTempSw->setText(QString("%1").arg(tempSw));
}

void Q_SP1403_R1B_Widget::on_pushButtonS6_Write_clicked()
{
    quint8 addr = quint8(ui->lineEditS6_Addr->text().toUShort(nullptr,16));
    quint16 data = ui->lineEditS6_Data->text().toUShort(nullptr,16);
    SP1403_R1B->set_s6_reg(addr,data);
}

void Q_SP1403_R1B_Widget::on_pushButtonS6_Read_clicked()
{
    quint16 data = 0;
    quint8 addr = quint8(ui->lineEditS6_Addr->text().toUShort(nullptr,16));
    SP1403_R1B->get_s6_reg(addr,data);
    ui->lineEditS6_Data->setText(DecimalToHexString(data));
}
