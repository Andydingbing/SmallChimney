#include "starpoint_sp9500_sp1401_r1a_widget.h"

using namespace ns_sp1401;
using namespace ns_sp1401::r1a;

Q_StarPoint_SP9500_SP1401_R1A_Widget::Q_StarPoint_SP9500_SP1401_R1A_Widget(QWidget *parent) :
    Q_Widget(parent),
    ui(new Ui::Q_StarPoint_SP9500_SP1401_R1A_Widget)
{
    ui->setupUi(this);

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
    ui->lineEditDGain->setText("0");

    ui->lineEditRxFreq->setText("2G");
    ui->comboBoxRxLNAAttSw->setCurrentIndex(int(RX_ATT));
    ui->comboBoxRxAtt1->setCurrentIndex(int(15.0 / 0.5));
    ui->comboBoxRxAtt2->setCurrentIndex(1);
    ui->comboBoxIFFilterSw->setCurrentIndex(int(sp1401::_160MHz));
    ui->lineEditRxDet->setText("0");
    ui->lineEditRxTemp->setText("0");
    ui->comboBoxHwVer->setCurrentIndex(R1A);
    ui->lineEditOrdinal->setText("00000000");
    ui->lineEditSN->setText("");
}

Q_StarPoint_SP9500_SP1401_R1A_Widget::~Q_StarPoint_SP9500_SP1401_R1A_Widget()
{
    delete ui;
}

void Q_StarPoint_SP9500_SP1401_R1A_Widget::on_pushButtonSetTxFreq_clicked()
{
    quint64 freq = freq_string_to<quint64>(ui->lineEditTxFreq->text().toStdString());
    SP1401_R1A->set_tx_freq(freq);
}

void Q_StarPoint_SP9500_SP1401_R1A_Widget::on_pushButtonSetTxAtt1_clicked()
{
    int32_t att = ui->comboBoxTxAtt1->currentIndex() * 10;
    SP1401_R1A->set_tx_att1(att);
}

void Q_StarPoint_SP9500_SP1401_R1A_Widget::on_pushButtonSetTxAtt2_clicked()
{
    int32_t att = ui->comboBoxTxAtt2->currentIndex() * 10;
    SP1401_R1A->set_tx_att2(att);
}

void Q_StarPoint_SP9500_SP1401_R1A_Widget::on_pushButtonSetTxPAAttSw_clicked()
{
    tx_pa_att_t pa_att = tx_pa_att_t(ui->comboBoxTxPAAttSw->currentIndex());
    SP1401_R1A->set_tx_pa_att_sw(pa_att);
}

void Q_StarPoint_SP9500_SP1401_R1A_Widget::on_pushButtonSetTxAtt3_clicked()
{
    int32_t att = ui->comboBoxTxAtt3->currentIndex() * 10;
    SP1401_R1A->set_tx_att3(att);
}

void Q_StarPoint_SP9500_SP1401_R1A_Widget::on_pushButtonGetTxDet_clicked()
{
    uint32_t det = 0;
    INT_CHECKV(SP1401_R1A->set_det_sw(sp1401::CH_TX));
    INT_CHECKV(SP1401_R1A->get_ad7680(det));
    ui->lineEditTxDet->setText(QString("%1").arg(det));
}

void Q_StarPoint_SP9500_SP1401_R1A_Widget::on_pushButtonGetTxTemp_clicked()
{
    double temp = 0.0;
    INT_CHECKV(SP1401_R1A->get_tx_temp(temp));
    ui->lineEditTxTemp->setText(QString("%1").arg(temp));
}

void Q_StarPoint_SP9500_SP1401_R1A_Widget::on_pushButtonSetIOMode_clicked()
{
    SP1401_R1A->set_io_mode(io_mode_t::_from_index(ui->comboBoxIOMode->currentIndex()));
}

void Q_StarPoint_SP9500_SP1401_R1A_Widget::on_pushButtonSyncSetTxGain_clicked()
{
    int32_t att1 = ui->comboBoxTxAtt1->currentIndex() * 10;
    int32_t att2 = ui->comboBoxTxAtt2->currentIndex() * 10;
    int32_t att3 = ui->comboBoxTxAtt3->currentIndex() * 10;
    double d_gain = ui->lineEditDGain->text().toDouble();
    SP1401_R1A->sync_set_tx_gain(att1,att2,att3,d_gain);
}

void Q_StarPoint_SP9500_SP1401_R1A_Widget::on_pushButtonSetRxFreq_clicked()
{
    quint64 freq = freq_string_to<quint64>(ui->lineEditRxFreq->text().toStdString());
    SP1401_R1A->set_rx_freq(freq);
}

void Q_StarPoint_SP9500_SP1401_R1A_Widget::on_pushButtonSetRxLNAAttSw_clicked()
{
    rx_lna_att_t lna_att = rx_lna_att_t(ui->comboBoxRxLNAAttSw->currentIndex());
    SP1401_R1A->set_rx_lna_att_sw(lna_att);
}

void Q_StarPoint_SP9500_SP1401_R1A_Widget::on_pushButtonSetRxAtt1_clicked()
{
    double att = ui->comboBoxRxAtt1->currentIndex() * 0.5;
    SP1401_R1A->set_rx_att1(att);
}

void Q_StarPoint_SP9500_SP1401_R1A_Widget::on_pushButtonSetRxAtt2_clicked()
{
    int32_t att = ui->comboBoxRxAtt2->currentIndex() * 10;
    SP1401_R1A->set_rx_att2(att);
}

void Q_StarPoint_SP9500_SP1401_R1A_Widget::on_pushButtonSetIFFilterSw_clicked()
{
    sp1401::rx_if_filter_t filter = (sp1401::rx_if_filter_t)(ui->comboBoxIFFilterSw->currentIndex());
    SP1401_R1A->set_rx_if_filter_sw(filter);
}

void Q_StarPoint_SP9500_SP1401_R1A_Widget::on_pushButtonGetRxDet_clicked()
{
    uint32_t det = 0;
    INT_CHECKV(SP1401_R1A->set_det_sw(sp1401::CH_RX));
    INT_CHECKV(SP1401_R1A->get_ad7680(det));
    ui->lineEditRxDet->setText(QString("%1").arg(det));
}

void Q_StarPoint_SP9500_SP1401_R1A_Widget::on_pushButtonGetRxTemp_clicked()
{
    double temp = 0.0;
    INT_CHECKV(SP1401_R1A->get_rx_temp(temp));
    ui->lineEditRxTemp->setText(QString("%1").arg(temp));
}

void Q_StarPoint_SP9500_SP1401_R1A_Widget::on_pushButtonSetSN_clicked()
{
    quint32 ordinal = ui->lineEditOrdinal->text().toUInt();
    hw_ver_t ver = hw_ver_t(ui->comboBoxHwVer->currentIndex());
    SP1401_R1A->set_sn_major(ver,ordinal);
}

void Q_StarPoint_SP9500_SP1401_R1A_Widget::on_pushButtonGetSN_clicked()
{
    char sn[32] = {0};
    memset(sn,0,sizeof(sn));
    INT_CHECKV(SP1401_R1A->get_sn_major(sn));
    ui->lineEditSN->setText(QString("%1").arg(sn));
}
