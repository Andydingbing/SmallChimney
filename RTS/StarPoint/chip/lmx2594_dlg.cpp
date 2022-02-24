#include "lmx2594_dlg.h"
#include "ui_lmx2594.h"
#include "global.h"
#include "reg_def_lmx2594.h"
#include "algo_chip_pll.hpp"

using namespace ns_lmx2594;

Q_LMX2594_Dlg::Q_LMX2594_Dlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Q_LMX2594_Dlg)
{
    ui->setupUi(this);

    RF = nullptr;

    setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);
    setFixedSize(width(),height());

    LMX2594_REG_DECL_0x00;
    LMX2594_REG_DECL_0x04;
    LMX2594_REG_DECL_0x09;
    LMX2594_REG_DECL_0x0a;
    LMX2594_REG_DECL_0x0b;
    LMX2594_REG_DECL_0x0c;
    LMX2594_REG_DECL_0x0e;
    LMX2594_REG_DECL_0x1f;
    LMX2594_REG_DECL_0x25;
    LMX2594_REG_DECL_0x28;
    LMX2594_REG_DECL_0x29;
    LMX2594_REG_DECL_0x2c;
    LMX2594_REG_DECL_0x2d;
    LMX2594_REG_DECL_0x2e;
    LMX2594_REG_DECL_0x3b;
    LMX2594_REG_DECL_0x3c;
    LMX2594_REG_DECL_0x4b;

    ui->lineEditRefFreq->setText("100M");
    ui->checkBoxOSCinDoubler->setChecked(LMX2594_REG(0x09).osc_2x ? true : false);
    ui->lineEditPreRDiv->setText(QString("%1").arg(LMX2594_REG(0x0c).pll_r_pre));
    ui->comboBoxMultipiler->setCurrentIndex(LMX2594_REG(0x0a).mult);
    ui->lineEditPostRDiv->setText(QString("%1").arg(LMX2594_REG(0x0b).pll_r));
    ui->checkBoxOUT_MUTE->setChecked(LMX2594_REG(0x00).out_mute);
    ui->checkBoxFCAL->setChecked(LMX2594_REG(0x00).en_fcal);
    ui->comboBoxMUXOUT_LD->setCurrentIndex(LMX2594_REG(0x00).muxout_ld_sel);
    ui->checkBoxRESET->setChecked(LMX2594_REG(0x00).reset);
    ui->checkBoxPOWER_DOWN->setChecked(LMX2594_REG(0x00).pwr_down);
    ui->comboBoxCPG->setCurrentIndex(LMX2594_REG(0x0e).cpg);
    ui->lineEditACAL_CMP_DELAY->setText(QString("%1").arg(LMX2594_REG(0x04).acal_cmp_dly));
    ui->checkBoxMASH_SEED_EN->setChecked(LMX2594_REG(0x25).en_mash_seed);
    ui->lineEditMASH_SEED->setText(QString("%1").arg(mash_seed(LMX2594_REG_DATA(0x29),LMX2594_REG_DATA(0x28))));
    ui->checkBoxMASH_RESET_N->setChecked(LMX2594_REG(0x2c).mash_reset_n);
    ui->comboBoxMASH_ORDER->setCurrentIndex(LMX2594_REG(0x2c).mash_order);
    ui->lineEditOUTA_PWR->setText(QString("%1").arg(LMX2594_REG(0x2c).outa_pwr));
    ui->checkBoxOUTB_PD->setChecked(LMX2594_REG(0x2c).outb_pd);
    ui->checkBoxOUTA_PD->setChecked(LMX2594_REG(0x2c).outa_pd);
    ui->comboBoxOUTA_MUX->setCurrentIndex(LMX2594_REG(0x2d).outa_mux);
    ui->lineEditOUT_ISEL->setText(QString("%1").arg(LMX2594_REG(0x2d).out_iset));
    ui->lineEditOUTB_PWR->setText(QString("%1").arg(LMX2594_REG(0x2d).outb_pwr));
    ui->comboBoxOUTB_MUX->setCurrentIndex(LMX2594_REG(0x2e).outb_mux);
    ui->comboBoxLD_TYPE->setCurrentIndex(LMX2594_REG(0x3b).ld_type);
    ui->lineEditLD_DLY->setText(QString("%1").arg(LMX2594_REG(0x3c).ld_dly));
    ui->checkBoxSEG1->setChecked(LMX2594_REG(0x1f).chdiv_div2);
    ui->comboBoxCHDIV->setCurrentIndex(LMX2594_REG(0x4b).chdiv);

    if (RF != nullptr) {
        ui->lineEditRFFreq->setText(QString::fromStdString(RF->lo_freq_string(LO)));
        updateFreq();
    }

    ui->lineEditAddr->setText("0x00");
    ui->lineEditData->setText("0x0000");
}

Q_LMX2594_Dlg::~Q_LMX2594_Dlg()
{
    delete ui;
}

void Q_LMX2594_Dlg::updateFreq()
{
    quint64 RFFreq = 0;
    quint64 refFreq = 0;

    LMX2594_REG_DECL_0x00;
    LMX2594_REG_DECL_0x14;
    LMX2594_REG_DECL_0x24;
    LMX2594_REG_DECL_0x26;
    LMX2594_REG_DECL_0x27;
    LMX2594_REG_DECL_0x2a;
    LMX2594_REG_DECL_0x2b;
    LMX2594_REG_DECL_0x2d;

    QT_TRY {
        if (!ui->lineEditRFFreq->text().isEmpty()) {
            RFFreq = freq_string_to<quint64>(ui->lineEditRFFreq->text().toStdString());
        }
        if (!ui->lineEditRefFreq->text().isEmpty()) {
            refFreq = freq_string_to<quint64>(ui->lineEditRefFreq->text().toStdString());
        }
    }
    QT_CATCH(...) {}

    freq_formula_in param_in;
    freq_formula_out param_out;

    quint32 OSC_2X = ui->checkBoxOSCinDoubler->isChecked() ? 2 : 1;
    quint32 MULT = quint32(ui->comboBoxMultipiler->currentIndex());
    quint32 PLL_R_PRE = ui->lineEditPreRDiv->text().toUInt();
    quint32 PLL_R = ui->lineEditPostRDiv->text().toUInt();

    param_in.freq_rf = RFFreq;
    param_in.freq_pd = refFreq * OSC_2X * MULT / (PLL_R_PRE * PLL_R);
    freq_formula(param_in,param_out);

    ui->lineEditN->setText(QString("%1").arg(param_out.N));
    ui->lineEditNUM->setText(QString("%1").arg(param_out.NUM));
    ui->lineEditDEN->setText(QString("%1").arg(param_out.DEN));

    if (RF == nullptr) {
        return;
    }

    LMX2594_REG(0x00).en_fcal = 1;
    LMX2594_REG(0x14).vco_sel = (RFFreq < FREQ_M(11900) || RFFreq > FREQ_M(12100)) ? 7 : 4;
    LMX2594_REG(0x24).pll_n = param_out.N;
    LMX2594_REG(0x26).pll_den = param_out.DEN >> 16;
    LMX2594_REG(0x27).pll_den = param_out.DEN & 0x0000ffff;
    LMX2594_REG(0x2a).pll_num = param_out.NUM >> 16;
    LMX2594_REG(0x2b).pll_num = param_out.NUM & 0x0000ffff;
    LMX2594_REG(0x2d).outa_mux = RFFreq < FREQ_M(7500) ? 0 : 1;

    RF->set_lo_reg(LO,0x14,LMX2594_REG_DATA(0x14));
    RF->set_lo_reg(LO,0x2d,LMX2594_REG_DATA(0x2d));
    RF->set_lo_reg(LO,0x24,LMX2594_REG_DATA(0x24));
    RF->set_lo_reg(LO,0x2a,LMX2594_REG_DATA(0x2a));
    RF->set_lo_reg(LO,0x2b,LMX2594_REG_DATA(0x2b));
    RF->set_lo_reg(LO,0x26,LMX2594_REG_DATA(0x26));
    RF->set_lo_reg(LO,0x27,LMX2594_REG_DATA(0x27));
    RF->set_lo_reg(LO,0x00,LMX2594_REG_DATA(0x00));
}

void Q_LMX2594_Dlg::updateReg0x00()
{
    LMX2594_REG_DECL_0x00;

    LMX2594_REG(0x00).out_mute = ui->checkBoxOUT_MUTE->isChecked() ? 1 : 0;
    LMX2594_REG(0x00).en_fcal = ui->checkBoxFCAL->isChecked() ? 1 : 0;
    LMX2594_REG(0x00).muxout_ld_sel = unsigned(ui->comboBoxMUXOUT_LD->currentIndex());
    LMX2594_REG(0x00).reset = ui->checkBoxRESET->isChecked() ? 1 : 0;
    LMX2594_REG(0x00).pwr_down = ui->checkBoxPOWER_DOWN->isChecked() ? 1 : 0;
    RF->set_lo_reg(LO,0x00,LMX2594_REG_DATA(0x00));
}

void Q_LMX2594_Dlg::updateReg0x04()
{
    LMX2594_REG_DECL_0x04;

    LMX2594_REG(0x04).acal_cmp_dly = ui->lineEditACAL_CMP_DELAY->text().toUInt();
    RF->set_lo_reg(LO,0x04,LMX2594_REG_DATA(0x04));
}

void Q_LMX2594_Dlg::updateReg0x0e()
{
    LMX2594_REG_DECL_0x0e;

    LMX2594_REG(0x0e).cpg = unsigned(ui->comboBoxCPG->currentIndex());
    RF->set_lo_reg(LO,0x0e,LMX2594_REG_DATA(0x0e));
}

void Q_LMX2594_Dlg::updateReg0x1f()
{
    LMX2594_REG_DECL_0x1f;

    LMX2594_REG(0x1f).chdiv_div2 = ui->checkBoxSEG1->isChecked() ? 1 : 0;
    RF->set_lo_reg(LO,0x1f,LMX2594_REG_DATA(0x1f));
}

void Q_LMX2594_Dlg::updateReg0x25()
{
    LMX2594_REG_DECL_0x25;

    LMX2594_REG(0x25).en_mash_seed = ui->checkBoxMASH_SEED_EN->isChecked() ? 1 : 0;
    RF->set_lo_reg(LO,0x25,LMX2594_REG_DATA(0x25));
}

void Q_LMX2594_Dlg::updateReg0x28()
{
    LMX2594_REG_DECL_0x28;

    LMX2594_REG(0x28).mash_seed = ui->lineEditMASH_SEED->text().toUInt() >> 16;
    RF->set_lo_reg(LO,0x28,LMX2594_REG_DATA(0x28));
}

void Q_LMX2594_Dlg::updateReg0x29()
{
    LMX2594_REG_DECL_0x29;

    LMX2594_REG(0x29).mash_seed = ui->lineEditMASH_SEED->text().toUInt() & 0x0000ffff;
    RF->set_lo_reg(LO,0x29,LMX2594_REG_DATA(0x29));
}

void Q_LMX2594_Dlg::updateReg0x2c()
{
    LMX2594_REG_DECL_0x2c;

    LMX2594_REG(0x2c).mash_reset_n = ui->checkBoxMASH_RESET_N->isChecked() ? 1 : 0;
    LMX2594_REG(0x2c).mash_order = unsigned(ui->comboBoxMASH_ORDER->currentIndex());
    LMX2594_REG(0x2c).outa_pwr = ui->lineEditOUTA_PWR->text().toUInt();
    LMX2594_REG(0x2c).outb_pd = ui->checkBoxOUTB_PD->isChecked() ? 1 : 0;
    LMX2594_REG(0x2c).outa_pd = ui->checkBoxOUTA_PD->isChecked() ? 1 : 0;
    RF->set_lo_reg(LO,0x2c,LMX2594_REG_DATA(0x2c));
}

void Q_LMX2594_Dlg::updateReg0x2d()
{
    LMX2594_REG_DECL_0x2d;

    LMX2594_REG(0x2d).outa_mux = unsigned(ui->comboBoxOUTA_MUX->currentIndex());
    LMX2594_REG(0x2d).out_iset = ui->lineEditOUT_ISEL->text().toUInt();
    LMX2594_REG(0x2d).outb_pwr = ui->lineEditOUTB_PWR->text().toUInt();
    RF->set_lo_reg(LO,0x2d,LMX2594_REG_DATA(0x2d));
}

void Q_LMX2594_Dlg::updateReg0x2e()
{
    LMX2594_REG_DECL_0x2e;

    LMX2594_REG(0x2e).outb_mux = unsigned(ui->comboBoxOUTB_MUX->currentIndex());
    RF->set_lo_reg(LO,0x2e,LMX2594_REG_DATA(0x2e));
}

void Q_LMX2594_Dlg::updateReg0x3b()
{
    LMX2594_REG_DECL_0x3b;

    LMX2594_REG(0x3b).ld_type = unsigned(ui->comboBoxLD_TYPE->currentIndex());
    RF->set_lo_reg(LO,0x3b,LMX2594_REG_DATA(0x3b));
}

void Q_LMX2594_Dlg::updateReg0x3c()
{
    LMX2594_REG_DECL_0x3c;

    LMX2594_REG(0x3c).ld_dly = ui->lineEditLD_DLY->text().toUInt();
    RF->set_lo_reg(LO,0x3c,LMX2594_REG_DATA(0x3c));
}

void Q_LMX2594_Dlg::updateReg0x4b()
{
    LMX2594_REG_DECL_0x4b;

    LMX2594_REG(0x4b).chdiv = unsigned(ui->comboBoxCHDIV->currentIndex());
    RF->set_lo_reg(LO,0x4b,LMX2594_REG_DATA(0x4b));
}

void Q_LMX2594_Dlg::on_checkBoxOSCinDoubler_clicked(bool checked)
{
    LMX2594_REG_DECL_0x09;

    LMX2594_REG(0x09).osc_2x = checked ? 2 : 1;
    RF->set_lo_reg(LO,0x09,LMX2594_REG_DATA(0x09));
}

void Q_LMX2594_Dlg::on_lineEditPreRDiv_textEdited(const QString &arg1)
{
    Q_UNUSED(arg1);
    updateFreq();
}

void Q_LMX2594_Dlg::on_comboBoxMultipiler_activated(int index)
{
    LMX2594_REG_DECL_0x0a;

    LMX2594_REG(0x0a).mult = unsigned(index);
    RF->set_lo_reg(LO,0x0a,LMX2594_REG_DATA(0x0a));
}

void Q_LMX2594_Dlg::on_lineEditPostRDiv_textEdited(const QString &arg1)
{
    Q_UNUSED(arg1);
    updateFreq();
}

void Q_LMX2594_Dlg::on_checkBoxOUT_MUTE_clicked(bool checked)
{
    updateReg0x00();
}

void Q_LMX2594_Dlg::on_checkBoxFCAL_clicked(bool checked)
{
    updateReg0x00();
}

void Q_LMX2594_Dlg::on_comboBoxMUXOUT_LD_activated(int index)
{
    updateReg0x00();
}

void Q_LMX2594_Dlg::on_checkBoxRESET_clicked(bool checked)
{
    updateReg0x00();
}

void Q_LMX2594_Dlg::on_checkBoxPOWER_DOWN_clicked(bool checked)
{
    updateReg0x00();
}

void Q_LMX2594_Dlg::on_comboBoxCPG_activated(int index)
{
    updateReg0x0e();
}

void Q_LMX2594_Dlg::on_lineEditACAL_CMP_DELAY_textEdited(const QString &arg1)
{
    updateReg0x04();
}

void Q_LMX2594_Dlg::on_checkBoxMASH_SEED_EN_clicked(bool checked)
{
    updateReg0x25();
}

void Q_LMX2594_Dlg::on_lineEditMASH_SEED_textEdited(const QString &arg1)
{
    updateReg0x28();
    updateReg0x29();
}

void Q_LMX2594_Dlg::on_checkBoxMASH_RESET_N_clicked(bool checked)
{
    updateReg0x2c();
}

void Q_LMX2594_Dlg::on_comboBoxMASH_ORDER_activated(int index)
{
    updateReg0x2c();
}

void Q_LMX2594_Dlg::on_lineEditOUTA_PWR_textEdited(const QString &arg1)
{
    updateReg0x2c();
}

void Q_LMX2594_Dlg::on_checkBoxOUTB_PD_clicked(bool checked)
{
    updateReg0x2c();
}

void Q_LMX2594_Dlg::on_checkBoxOUTA_PD_clicked(bool checked)
{
    updateReg0x2c();
}

void Q_LMX2594_Dlg::on_comboBoxOUTA_MUX_activated(int index)
{
    updateReg0x2d();
}

void Q_LMX2594_Dlg::on_lineEditOUT_ISEL_textEdited(const QString &arg1)
{
    updateReg0x2d();
}

void Q_LMX2594_Dlg::on_lineEditOUTB_PWR_textEdited(const QString &arg1)
{
    updateReg0x2d();
}

void Q_LMX2594_Dlg::on_comboBoxOUTB_MUX_activated(int index)
{
    updateReg0x2e();
}

void Q_LMX2594_Dlg::on_comboBoxLD_TYPE_activated(int index)
{
    updateReg0x3b();
}

void Q_LMX2594_Dlg::on_lineEditLD_DLY_textEdited(const QString &arg1)
{
    updateReg0x3c();
}

void Q_LMX2594_Dlg::on_checkBoxSEG1_clicked(bool checked)
{
    updateReg0x1f();
}

void Q_LMX2594_Dlg::on_comboBoxCHDIV_activated(int index)
{
    updateReg0x4b();
}

void Q_LMX2594_Dlg::on_pushButtonWrite_clicked()
{
    quint8 addr = quint8(ui->lineEditAddr->text().toUShort(nullptr,16));
    quint16 data = ui->lineEditData->text().toUShort(nullptr,16);
    RF->set_lo_reg(LO,addr,data);
}

void Q_LMX2594_Dlg::on_pushButtonRead_clicked()
{
    quint16 data = 0;
    quint8 addr = quint8(ui->lineEditAddr->text().toUShort(nullptr,16));
    RF->get_lo_reg(LO,addr,data);
    ui->lineEditData->setText(DecimalToHexString(data));
}
