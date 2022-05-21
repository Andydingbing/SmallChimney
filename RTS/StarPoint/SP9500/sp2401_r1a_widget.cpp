#include "starpoint_sp9500_sp2401_r1a_widget.h"
#include "algo_math.hpp"

void QString2Coef(const QString strCoef,double *coef,qint32 order)
{
    int idxStar = 0;
    int idxStop = 0;
    int idxCoef = 0;

    while (-1 != (idxStop = strCoef.indexOf(',',idxStar))) {
        coef[idxCoef] = strCoef.mid(idxStar,idxStop - idxStar).toDouble();
        idxStar = idxStop + 1;
        idxCoef ++;
        if (order - 1 == idxCoef) {
            break;
        }
    }
    coef[idxCoef] = strCoef.mid(idxStar).toDouble();
}

Q_StarPoint_SP9500_SP2401_R1A_Widget::Q_StarPoint_SP9500_SP2401_R1A_Widget(QWidget *parent) :
    Q_Widget(parent),
    ui(new Ui::Q_StarPoint_SP9500_SP2401_R1A_Widget)
{
    ui->setupUi(this);

    ui->lineEditDAClock->setText("491.52");
    ui->comboBoxDASw->setCurrentIndex(sp2401_r1a::FROM_TO_RF);
    ui->comboBoxDDSSrc->setCurrentIndex(sp2401_r1a::SINGLE_TONE);

    ui->lineEditDDS1->stepDefault = 1;
    ui->lineEditDDS1->stepCtrl = 5;
    ui->lineEditDDS1->stepShift = 10;
    ui->lineEditDDS1->upper = 100;
    ui->lineEditDDS1->lower = -100;
    ui->lineEditDDS1->setText("20");

    ui->lineEditDDS2->stepDefault = 1;
    ui->lineEditDDS2->stepCtrl = 5;
    ui->lineEditDDS2->stepShift = 10;
    ui->lineEditDDS2->upper = 100;
    ui->lineEditDDS2->lower = -100;
    ui->lineEditDDS2->setText("0");

    ui->lineEditTxPowerComp->setText("0");

    ui->lineEditDUC->stepDefault = 1;
    ui->lineEditDUC->stepCtrl = 5;
    ui->lineEditDUC->stepShift = 10;
    ui->lineEditDUC->upper = 100;
    ui->lineEditDUC->lower = -100;
    ui->lineEditDUC->setText("0");

//<10,1,5,10,20,-20>

    ui->lineEditIQPhaseI->stepDefault = 0.1;
    ui->lineEditIQPhaseI->stepCtrl = 0.5;
    ui->lineEditIQPhaseI->stepShift = 1;
    ui->lineEditIQPhaseI->upper = 2;
    ui->lineEditIQPhaseI->lower = -2;
    ui->lineEditIQPhaseI->setText("0");
    ui->lineEditIQPhaseQ->setText("0");

    //<1,1,5,10,16384,-16384>
    ui->lineEditIQAmpI->stepDefault = 1;
    ui->lineEditIQAmpI->stepCtrl = 5;
    ui->lineEditIQAmpI->stepShift = 10;
    ui->lineEditIQAmpI->upper = 16384;
    ui->lineEditIQAmpI->lower = -16384;
    ui->lineEditIQAmpI->setText("8192");

    ui->lineEditIQAmpQ->stepDefault = 1;
    ui->lineEditIQAmpQ->stepCtrl = 5;
    ui->lineEditIQAmpQ->stepShift = 10;
    ui->lineEditIQAmpQ->upper = 16384;
    ui->lineEditIQAmpQ->lower = -16384;
    ui->lineEditIQAmpQ->setText("8192");

    ui->lineEditDCOffsetI->stepDefault = 1;
    ui->lineEditDCOffsetI->stepCtrl = 5;
    ui->lineEditDCOffsetI->stepShift = 10;
    ui->lineEditDCOffsetI->upper = 8192;
    ui->lineEditDCOffsetI->lower = -8192;
    ui->lineEditDCOffsetI->setText("0");

    ui->lineEditDCOffsetQ->stepDefault = 1;
    ui->lineEditDCOffsetQ->stepCtrl = 5;
    ui->lineEditDCOffsetQ->stepShift = 10;
    ui->lineEditDCOffsetQ->upper = 8192;
    ui->lineEditDCOffsetQ->lower = -8192;
    ui->lineEditDCOffsetQ->setText("0");

    ui->lineEditADClock->setText("368.64");
    ui->comboBoxADSw->setCurrentIndex(sp2401_r1a::FROM_TO_RF);
    ui->lineEditDDC->setText("-92.64M");
    ui->lineEditManualPower->setText("0");
    ui->lineEditADManual->setText("0");
    ui->lineEditADFPGA->setText("0");
    ui->lineEditRxPowerComp->setText("0");

    QString strRxCoefReal = "";
    QString strRxCoefImag = "";
    double rxCoefReal[ul_filter_tap] = {0.0};
    double rxCoefImag[ul_filter_tap] = {0.0};
    sp2401_r1a::rx_filter_coef_default(rxCoefReal,rxCoefImag,ul_filter_tap);
    for (qint32 i = 0;i < ul_filter_tap - 1;i ++) {
        strRxCoefReal.append(QString("%1,").arg(rxCoefReal[i]));
        strRxCoefImag.append(QString("%1,").arg(rxCoefImag[i]));
    }
    strRxCoefReal.append(QString("%1").arg(rxCoefReal[ul_filter_tap - 1]));
    strRxCoefImag.append(QString("%1").arg(rxCoefImag[ul_filter_tap - 1]));
    ui->textEditRxFilterReal->setText(strRxCoefReal);
    ui->textEditRxFilterImag->setText(strRxCoefImag);
}

Q_StarPoint_SP9500_SP2401_R1A_Widget::~Q_StarPoint_SP9500_SP2401_R1A_Widget()
{
    delete ui;
}

void Q_StarPoint_SP9500_SP2401_R1A_Widget::on_pushButtonSetDAClock_clicked()
{
    double freq = ui->lineEditDAClock->text().toDouble();
    SP2401->set_da_freq_MHz(freq);
}

void Q_StarPoint_SP9500_SP2401_R1A_Widget::on_pushButtonSetDASw_clicked()
{
    sp2401_r1a::ad_da_port_t port = sp2401_r1a::ad_da_port_t(ui->comboBoxDASw->currentIndex());
    SP2401->set_da_sw(port);
}

void Q_StarPoint_SP9500_SP2401_R1A_Widget::on_pushButtonResetDA_clicked()
{
    INT_CHECKV(SP2401->da_reset());
    on_pushButtonSetDAClock_clicked();
}

void Q_StarPoint_SP9500_SP2401_R1A_Widget::on_pushButtonSetDDSSrc_clicked()
{
    sp2401_r1a::da_src_t src = sp2401_r1a::da_src_t(ui->comboBoxDDSSrc->currentIndex());
    SP2401->set_dds_src(src);
}

void Q_StarPoint_SP9500_SP2401_R1A_Widget::on_pushButtonSetDDS1_clicked()
{
    SP2401->set_dds1(ui->lineEditDDS1->text().toDouble()*1000000);
}

void Q_StarPoint_SP9500_SP2401_R1A_Widget::on_pushButtonSetDDS2_clicked()
{
    SP2401->set_dds2(freq_string_to_double(ui->lineEditDDS2->text().toStdString()));
}

void Q_StarPoint_SP9500_SP2401_R1A_Widget::on_pushButtonSetTxPowerComp_clicked()
{
    SP2401->set_tx_pwr_comp(ui->lineEditTxPowerComp->text().toDouble());
}

void Q_StarPoint_SP9500_SP2401_R1A_Widget::on_pushButtonSetDUC_clicked()
{
    SP2401->set_duc_dds(freq_string_to_double(ui->lineEditDUC->text().toStdString()));
}

void Q_StarPoint_SP9500_SP2401_R1A_Widget::on_checkBoxByPassTxFilter_clicked(bool checked)
{
    SP2401->set_tx_filter_sw(!checked);
}

void Q_StarPoint_SP9500_SP2401_R1A_Widget::on_pushButtonSetTxFilter_clicked()
{
    double coefReal[dl_filter_tap_2i] = {0.0};
    double coefImag[dl_filter_tap_2i] = {0.0};
    QString strCoefReal = ui->textEditTxFilterReal->toPlainText();
    QString strCoefImag = ui->textEditTxFilterImag->toPlainText();

    QString2Coef(strCoefReal,coefReal,dl_filter_tap_2i);
    QString2Coef(strCoefImag,coefImag,dl_filter_tap_2i);
    SP2401->set_tx_filter(coefReal,coefImag);
}

void Q_StarPoint_SP9500_SP2401_R1A_Widget::on_pushButtonSetADClock_clicked()
{
    double freq = ui->lineEditADClock->text().toDouble();
    SP2401->set_ad_freq_MHz(freq);
}

void Q_StarPoint_SP9500_SP2401_R1A_Widget::on_pushButtonSetADSw_clicked()
{
    sp2401_r1a::ad_da_port_t port = (sp2401_r1a::ad_da_port_t)(ui->comboBoxDASw->currentIndex());
    SP2401->set_ad_sw(port);
}

void Q_StarPoint_SP9500_SP2401_R1A_Widget::on_pushButtonSetDDC_clicked()
{
    double freq = freq_string_to_double(ui->lineEditDDC->text().toStdString());
    SP2401->set_ddc(freq);
}

void Q_StarPoint_SP9500_SP2401_R1A_Widget::on_pushButtonGetManualPower_clicked()
{
    on_pushButtonGetADManual_clicked();
}

void Q_StarPoint_SP9500_SP2401_R1A_Widget::on_pushButtonGetADManual_clicked()
{
    int64_t ad = 0;
    INT_CHECKV(SP1401->get_ads5474_manual(ad));
    ui->lineEditADManual->setText(QString("%1").arg(ad));
    ui->lineEditManualPower->setText(QString("%1").arg(round(ad_to_dBc(g_0dBFS,ad),3)));
    ui->lineEditRxPowerComp->setText(QString("%1").arg(g_0dBFS - ad));
}

void Q_StarPoint_SP9500_SP2401_R1A_Widget::on_pushButtonGetADFPGA_clicked()
{
    int64_t ad = 0;
    INT_CHECKV(SP1401->get_ads5474(ad));
    ui->lineEditADFPGA->setText(QString("%1").arg(ad));
}

void Q_StarPoint_SP9500_SP2401_R1A_Widget::on_pushButtonSetRxPowerComp_clicked()
{
    SP2401->set_rx_pwr_comp(ui->lineEditRxPowerComp->text().toInt());
}

void Q_StarPoint_SP9500_SP2401_R1A_Widget::on_pushButtonSetRxFilter_clicked()
{
    double coefReal[ul_filter_tap] = {0.0};
    double coefImag[ul_filter_tap] = {0.0};
    QString strCoefReal = ui->textEditRxFilterReal->toPlainText();
    QString strCoefImag = ui->textEditRxFilterImag->toPlainText();

    QString2Coef(strCoefReal,coefReal,ul_filter_tap);
    QString2Coef(strCoefImag,coefImag,ul_filter_tap);
    SP2401->set_rx_filter(coefReal,coefImag);
}

void Q_StarPoint_SP9500_SP2401_R1A_Widget::on_lineEditIQPhaseI_textEdited(const QString &arg1)
{
    SP2401->set_tx_phase_rotate_I(ui->lineEditIQPhaseI->text().toDouble());
}

void Q_StarPoint_SP9500_SP2401_R1A_Widget::on_lineEditIQAmpI_textEdited(const QString &arg1)
{
    SP2401->set_tx_amplitude_balance(
                ui->lineEditIQAmpI->text().toUShort(),
                ui->lineEditIQAmpQ->text().toUShort());
}

void Q_StarPoint_SP9500_SP2401_R1A_Widget::on_lineEditIQAmpQ_textEdited(const QString &arg1)
{
    SP2401->set_tx_amplitude_balance(
                ui->lineEditIQAmpI->text().toUShort(),
                ui->lineEditIQAmpQ->text().toUShort());
}

void Q_StarPoint_SP9500_SP2401_R1A_Widget::on_lineEditDCOffsetI_textEdited(const QString &arg1)
{
    SP2401->set_tx_dc_offset(
                ui->lineEditDCOffsetI->text().toShort(),
                ui->lineEditDCOffsetQ->text().toShort());
}

void Q_StarPoint_SP9500_SP2401_R1A_Widget::on_lineEditDCOffsetQ_textEdited(const QString &arg1)
{
    SP2401->set_tx_dc_offset(
                ui->lineEditDCOffsetI->text().toUShort(),
                ui->lineEditDCOffsetQ->text().toUShort());
}

void Q_StarPoint_SP9500_SP2401_R1A_Widget::on_lineEditDUC_textEdited(const QString &arg1)
{
    SP2401->set_duc_dds( ui->lineEditDUC->text().toDouble()*1000000);
}

void Q_StarPoint_SP9500_SP2401_R1A_Widget::on_lineEditDDS1_textEdited(const QString &arg1)
{
    SP2401->set_dds1(ui->lineEditDDS1->text().toDouble()*1000000);
}
