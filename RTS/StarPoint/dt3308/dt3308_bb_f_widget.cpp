#include "dt3308_bb_f_widget.h"
#include "ui_dt3308_bb_f.h"
#include "algo_math.hpp"

using namespace rd::ns_dt3308;

Q_DT3308_BB_F_Widget::Q_DT3308_BB_F_Widget(QWidget *parent) :
    Q_RD_Widget(parent),
    ui(new Ui::Q_DT3308_BB_F_Widget)
{
    ui->setupUi(this);

    ui->lineEditDAClock->setText("491.52");
    ui->comboBoxDLSrc->setCurrentIndex(bb_dt3308_f::CW);

    ui->lineEditTxPowerComp->setText("0");

    ui->lineEditDUC->stepDefault = 1;
    ui->lineEditDUC->stepCtrl = 5;
    ui->lineEditDUC->stepShift = 10;
    ui->lineEditDUC->upper = 100;
    ui->lineEditDUC->lower = -100;
    ui->lineEditDUC->setText("0");

    ui->lineEditIQPhaseI->stepDefault = 7.0 / 2048;
    ui->lineEditIQPhaseI->stepCtrl = 35.0 / 2048;
    ui->lineEditIQPhaseI->stepShift = 70.0 / 2048;
    ui->lineEditIQPhaseI->upper = 140.0 / 2048;
    ui->lineEditIQPhaseI->lower = -140.0 / 2048;
    ui->lineEditIQPhaseI->setText("0");

    ui->lineEditIQPhaseQ->stepDefault = 7.0 / 2048;
    ui->lineEditIQPhaseQ->stepCtrl = 35.0 / 2048;
    ui->lineEditIQPhaseQ->stepShift = 70.0 / 2048;
    ui->lineEditIQPhaseQ->upper = 140.0 / 2048;
    ui->lineEditIQPhaseQ->lower = -140.0 / 2048;
    ui->lineEditIQPhaseQ->setText("0");

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
    ui->lineEditDDC->setText("-92.64M");
    ui->lineEditManualPower->setText("0");
    ui->lineEditADManual->setText("0");
    ui->lineEditADFPGA->setText("0");
    ui->lineEditRxPowerComp->setText("0");
}

Q_DT3308_BB_F_Widget::~Q_DT3308_BB_F_Widget()
{
    delete ui;
}

void Q_DT3308_BB_F_Widget::on_pushButtonSetDAClock_clicked()
{
}

void Q_DT3308_BB_F_Widget::on_pushButtonResetDA_clicked()
{
    on_pushButtonSetDAClock_clicked();
}

void Q_DT3308_BB_F_Widget::on_pushButtonSetDLSrc_clicked()
{
    DT3308_BB_F->set_dl_src(bb_dt3308_f::dl_src_t(ui->comboBoxDLSrc->currentIndex()));
}

void Q_DT3308_BB_F_Widget::on_pushButtonSetDDS2_clicked()
{
}

void Q_DT3308_BB_F_Widget::on_pushButtonSetTxPowerComp_clicked()
{
    DT3308_BB_F->set_dl_pwr_comp(qint32(ui->lineEditTxPowerComp->text().toDouble() * 1000));
}

void Q_DT3308_BB_F_Widget::on_pushButtonSetDUC_clicked()
{
    RD_TRY
    DT3308_BB_F->set_duc(freq_string_to_double(ui->lineEditDUC->text().toStdString()));
    RD_LOG_ALL_EXCEPTION
}

void Q_DT3308_BB_F_Widget::on_lineEditDUC_textEdited(const QString &arg1)
{
    Q_UNUSED(arg1);
    on_pushButtonSetDUC_clicked();
}

void Q_DT3308_BB_F_Widget::on_pushButtonSetDDC_clicked()
{
    DT3308_BB_F->set_ddc(freq_string_to_double(ui->lineEditDDC->text().toStdString()));
}

void Q_DT3308_BB_F_Widget::on_pushButtonGetManualPower_clicked()
{
    on_pushButtonGetADManual_clicked();
}

void Q_DT3308_BB_F_Widget::on_pushButtonGetADManual_clicked()
{
    qint64 ad = 0;
    ui->lineEditADManual->setText(QString("%1").arg(ad));
    ui->lineEditManualPower->setText(QString("%1").arg(round(ad_to_dBc(g_0dBFS,ad),3)));
    ui->lineEditRxPowerComp->setText(QString("%1").arg(qint64(g_0dBFS) - ad));
}

void Q_DT3308_BB_F_Widget::on_pushButtonGetADFPGA_clicked()
{
    int64_t ad = 0;
    ui->lineEditADFPGA->setText(QString("%1").arg(ad));
}

void Q_DT3308_BB_F_Widget::on_pushButtonSetRxPowerComp_clicked()
{
    DT3308_BB_F->set_ul_pwr_comp(qint32(ui->lineEditRxPowerComp->text().toDouble() * 1000));
}

void Q_DT3308_BB_F_Widget::on_lineEditIQPhaseI_textEdited(const QString &arg1)
{
    DT3308_BB_F->set_da_phase_adj_i(arg1.toDouble());
}

void Q_DT3308_BB_F_Widget::on_lineEditIQPhaseQ_textEdited(const QString &arg1)
{
    DT3308_BB_F->set_da_phase_adj_q(arg1.toDouble());
}

void Q_DT3308_BB_F_Widget::on_lineEditDCOffsetI_textEdited(const QString &arg1)
{
    DT3308_BB_F->set_da_dc_offset_i(arg1.toUShort());
}

void Q_DT3308_BB_F_Widget::on_lineEditDCOffsetQ_textEdited(const QString &arg1)
{
    DT3308_BB_F->set_da_dc_offset_q(arg1.toUShort());
}
