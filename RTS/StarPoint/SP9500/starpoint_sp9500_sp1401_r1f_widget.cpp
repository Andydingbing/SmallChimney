#include "starpoint_sp9500_sp1401_r1f_widget.h"

using namespace ns_sp1401::r1f;

Q_StarPoint_SP9500_SP1401_R1F_Widget::Q_StarPoint_SP9500_SP1401_R1F_Widget(QWidget *parent) :
    Q_Widget(parent),
    ui(new Ui::Q_StarPoint_SP9500_SP1401_R1F_Widget)
{
    ui->setupUi(this);

    double att[SERIE_SIZE(0.0,31.5,0.5)] = {0.0};
    for (quint8 i = 0;i < ARRAY_SIZE(att);i ++) {
        att[i] = i * 0.5;
        QString strAtt = QString("%1").arg(att[i],2,'f',1);
        ui->comboBoxTXAtt0->addItem(strAtt);
        ui->comboBoxTXAtt1->addItem(strAtt);
        ui->comboBoxTXAtt2->addItem(strAtt);
        ui->comboBoxRXAtt1->addItem(strAtt);
        ui->comboBoxRXAtt2->addItem(strAtt);
    }
    ui->comboBoxTXAtt0->setMaxVisibleItems(20);
    ui->comboBoxTXAtt1->setMaxVisibleItems(20);
    ui->comboBoxTXAtt2->setMaxVisibleItems(20);
    ui->comboBoxTXAtt0->setCurrentIndex(int(5.0 / 0.5));
    ui->comboBoxTXAtt1->setCurrentIndex(int(20.0 / 0.5));
    ui->comboBoxTXAtt2->setCurrentIndex(int(0.0 / 0.5));

    ui->comboBoxRXAtt1->setMaxVisibleItems(20);
    ui->comboBoxRXAtt2->setMaxVisibleItems(20);
    ui->comboBoxRXAtt1->setCurrentIndex(int(5.0 / 0.5));
    ui->comboBoxRXAtt2->setCurrentIndex(int(15.0 / 0.5));

    ui->comboBoxIOMode->addItem(QString("IO"));
    ui->comboBoxIOMode->addItem(QString("Output"));
    ui->comboBoxIOMode->addItem(QString("Loopback"));
    ui->comboBoxIOMode->setCurrentIndex(io_mode_t::OUTPUT);

    ui->comboBoxRXLNAAttSw->addItem(QString("Attenuation"));
    ui->comboBoxRXLNAAttSw->addItem(QString("LNA"));

    ui->comboBoxRXAtt019Sw->addItem(QString("Att 19"));
    ui->comboBoxRXAtt019Sw->addItem(QString("Att 0"));

    ui->comboBoxIFFilterSw->addItem(QString("100M"));
    ui->comboBoxIFFilterSw->addItem(QString("160M"));

    ui->lineEditTXFreq->setText("2G");
    ui->lineEditDGain->setText("0");

    ui->lineEditRXFreq->setText("2G");
    ui->comboBoxRXLNAAttSw->setCurrentIndex(RX_ATT);
    ui->comboBoxRXAtt019Sw->setCurrentIndex(RX_ATT_19);
    ui->comboBoxIFFilterSw->setCurrentIndex(sp1401::_160MHz);

    ui->lineEditTemp0->setText("0.0");
    ui->lineEditTemp1->setText("0.0");
    ui->lineEditTemp2->setText("0.0");

    ui->lineEditDetTXIF2->setText("0");
    ui->lineEditDetRXCom->setText("0");
    ui->lineEditDetRXIF1->setText("0");

    ui->lineEditOrdinal->setText("00000000");
}

Q_StarPoint_SP9500_SP1401_R1F_Widget::~Q_StarPoint_SP9500_SP1401_R1F_Widget()
{
    delete ui;
}

void Q_StarPoint_SP9500_SP1401_R1F_Widget::on_lineEditTXFreq_returnPressed()
{
    RD_TRY
    uint64_t freq = freq_string_to_uint64_t(ui->lineEditTXFreq->text().toStdString());
    SP1401_R1F->set_tx_freq(freq);
    RD_CATCH_ALL_EXCEPTION
}

void Q_StarPoint_SP9500_SP1401_R1F_Widget::on_comboBoxIOMode_activated(int index)
{
    SP1401_R1F->set_io_mode(io_mode_t::_from_index(index));
}

void Q_StarPoint_SP9500_SP1401_R1F_Widget::on_lineEditDGain_textChanged(const QString &arg1)
{
    double att0 = ui->comboBoxTXAtt0->currentIndex() * 0.5;
    double att1 = ui->comboBoxTXAtt1->currentIndex() * 0.5;
    double att2 = ui->comboBoxTXAtt2->currentIndex() * 0.5;
    double d_gain = arg1.toDouble();
//    SP1401_R1F->sync_set_tx_gain(att0,att1,att2,att2,d_gain);
}

void Q_StarPoint_SP9500_SP1401_R1F_Widget::on_pushButtonSetTXAll_clicked()
{
    double att0 = ui->comboBoxTXAtt0->currentIndex() * 0.5;
    double att1 = ui->comboBoxTXAtt1->currentIndex() * 0.5;
    double att2 = ui->comboBoxTXAtt2->currentIndex() * 0.5;
    io_mode_t mode = io_mode_t::_from_index(ui->comboBoxIOMode->currentIndex());

    on_lineEditTXFreq_returnPressed();
    SP1401_R1F->set_tx_att(att0,att1,att2,0.0);
    SP1401_R1F->set_io_mode(mode);
}

void Q_StarPoint_SP9500_SP1401_R1F_Widget::on_lineEditRXFreq_returnPressed()
{
    RD_TRY
    uint64_t freq = freq_string_to_uint64_t(ui->lineEditRXFreq->text().toStdString());
    SP1401_R1F->set_rx_freq(freq);
    RD_CATCH_ALL_EXCEPTION
}

void Q_StarPoint_SP9500_SP1401_R1F_Widget::on_comboBoxRXLNAAttSw_activated(int index)
{
    SP1401_R1F->set_rx_lna_att_sw(rx_lna_att_t(index));
}

void Q_StarPoint_SP9500_SP1401_R1F_Widget::on_comboBoxRXAtt019Sw_activated(int index)
{
    SP1401_R1F->set_rx_att_019_sw(rx_att_019_t(index));
}

void Q_StarPoint_SP9500_SP1401_R1F_Widget::on_comboBoxRXAtt1_activated(int index)
{
    SP1401_R1F->set_rx_att1(index * 0.5);
}

void Q_StarPoint_SP9500_SP1401_R1F_Widget::on_comboBoxIFFilterSw_activated(int index)
{
    SP1401_R1F->set_rx_if_filter_sw(sp1401::rx_if_filter_t(index));
}

void Q_StarPoint_SP9500_SP1401_R1F_Widget::on_comboBoxRXAtt2_activated(int index)
{
    SP1401_R1F->set_rx_att2(index * 0.5);
}

void Q_StarPoint_SP9500_SP1401_R1F_Widget::on_pushButtonSetRXAll_clicked()
{
    rx_lna_att_t lnaAtt = rx_lna_att_t(ui->comboBoxRXLNAAttSw->currentIndex());
    rx_att_019_t att019 = rx_att_019_t(ui->comboBoxRXAtt019Sw->currentIndex());
    double att1 = ui->comboBoxRXAtt1->currentIndex() * 0.5;
    double att2 = ui->comboBoxRXAtt2->currentIndex() * 0.5;
    sp1401::rx_if_filter_t filter =
            sp1401::rx_if_filter_t(ui->comboBoxIFFilterSw->currentIndex());

    on_lineEditRXFreq_returnPressed();
    SP1401_R1F->set_rx_att(att1,att2,att2);
    SP1401_R1F->set_rx_lna_att_sw(lnaAtt);
    SP1401_R1F->set_rx_att_019_sw(att019);
    SP1401_R1F->set_rx_if_filter_sw(filter);
}

void Q_StarPoint_SP9500_SP1401_R1F_Widget::on_pushButtonGetTempDetAll_clicked()
{
    double temp[3] = {0.0};
    quint32 det[3] = {0};

    SP1401_R1F->get_temp(0,temp[0]);
    SP1401_R1F->get_temp(5,temp[1]);
    SP1401_R1F->get_temp(6,temp[2]);

    ui->lineEditTemp0->setText(QString("%1").arg(temp[0]));
    ui->lineEditTemp1->setText(QString("%1").arg(temp[1]));
    ui->lineEditTemp2->setText(QString("%1").arg(temp[2]));

    SP1401_R1F->get_ad7680_tx_if2(det[0]);
    SP1401_R1F->get_ad7680_rx_comb(det[1]);
    SP1401_R1F->get_ad7680_rx_if1(det[2]);

    ui->lineEditDetTXIF2->setText(QString("%1").arg(det[0]));
    ui->lineEditDetRXCom->setText(QString("%1").arg(det[1]));
    ui->lineEditDetRXIF1->setText(QString("%1").arg(det[2]));
}

void Q_StarPoint_SP9500_SP1401_R1F_Widget::on_pushButtonSetSN_clicked()
{
    quint32 ordinal = ui->lineEditOrdinal->text().toUInt();
    SP1401_R1F->set_sn_major(R1F,ordinal);
}

void Q_StarPoint_SP9500_SP1401_R1F_Widget::on_pushButtonGetSN_clicked()
{
    char sn[32] = {0};
    memset(sn,0,sizeof(sn));
    SP1401_R1F->get_sn_major(sn);
    ui->lineEditSN->setText(QString("%1").arg(sn));
}
