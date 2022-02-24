#include "hmc832_dlg.h"
#include "reg_def_hmc832.h"
#include "algo_chip_pll.hpp"

using namespace ns_hmc83x;

Q_HMC832_Dlg::Q_HMC832_Dlg(rf_dt3308_f::lo_t lo, QWidget *parent) :
    QDialog(parent),
    RF(nullptr),
    LO(lo),
    ui(new Ui::Q_HMC832_Dlg)
{
    ui->setupUi(this);

    setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);
    setFixedSize(width(),height());

    HMC83X_PLL_REG_DECL_0x01;
    HMC83X_PLL_REG_DECL_0x02;
    HMC83X_PLL_REG_DECL_0x06;
    HMC83X_PLL_REG_DECL_0x07;
    HMC83X_PLL_REG_DECL_0x08;
    HMC83X_PLL_REG_DECL_0x09;
    HMC83X_PLL_REG_DECL_0x0a;
    HMC83X_PLL_REG_DECL_0x0b;
    HMC83X_PLL_REG_DECL_0x0c;
    HMC83X_PLL_REG_DECL_0x0f;
    HMC83X_PLL_REG_DECL_0x10;

    HMC83X_VCO_REG_DECL_0x01;
    HMC83X_VCO_REG_DECL_0x02;
    HMC83X_VCO_REG_DECL_0x03;
    HMC83X_VCO_REG_DECL_0x07;

    hmc83x_t LOType = rf_dt3308_f::lo_type(LO);
    if (LOType == HMC832) {

    } else if (LOType == HMC833) {
        HMC83X_PLL_REG_DATA(0x09) = 0x547fff;
        HMC83X_PLL_REG_DATA(0x0a) = 0x002046;
        HMC83X_PLL_REG_DATA(0x0b) = 0x0fc021;
        HMC83X_VCO_REG_DATA(0x01) = 0x001f;
        HMC83X_VCO_REG_DATA(0x02) = 0x00c1;
        HMC83X_VCO_REG_DATA(0x03) = 0x0051;
    }

    ui->lineEditRFFreq->setText("2G");
    ui->lineEditRefFreq->setText("100M");
    ui->lineEditRDiv->setText(QString("%1").arg(HMC83X_PLL_REG(0x02).r_div));
    ui->comboBoxRstPin->setCurrentIndex(HMC83X_PLL_REG(0x01).rst_pin_sel);
    ui->checkBoxPD->setChecked(HMC83X_PLL_REG(0x01).rst_from_spi ? true : false);
    ui->checkBoxKeepBiasOn->setChecked(HMC83X_PLL_REG(0x01).keep_bias_on ? true : false);
    ui->checkBoxKeepPDOn->setChecked(HMC83X_PLL_REG(0x01).keep_pd_on ? true : false);
    ui->checkBoxKeepCPOn->setChecked(HMC83X_PLL_REG(0x01).keep_cp_on ? true : false);
    ui->checkBoxKeepRefBuffOn->setChecked(HMC83X_PLL_REG(0x01).keep_ref_buff_on ? true : false);
    ui->checkBoxKeepVCOOn->setChecked(HMC83X_PLL_REG(0x01).keep_vco_on ? true : false);
    ui->checkBoxKeepGPOOn->setChecked(HMC83X_PLL_REG(0x01).keep_gpo_on ? true : false);

    ui->comboBoxSeed->setCurrentIndex(HMC83X_PLL_REG(0x06).seed);
    ui->comboBoxModOrder->setCurrentIndex(HMC83X_PLL_REG(0x06).order);
    ui->checkBoxFracBypass->setChecked(HMC83X_PLL_REG(0x06).frac_bypass ? true : false);
    ui->checkBoxAutoSeed->setChecked(HMC83X_PLL_REG(0x06).auto_seed ? true : false);
    ui->comboBoxModClk->setCurrentIndex(HMC83X_PLL_REG(0x06).mod_clk_src);
    ui->checkBoxSD->setChecked(HMC83X_PLL_REG(0x06).en_sd);
    ui->checkBoxBIST->setChecked(HMC83X_PLL_REG(0x06).en_bist);
    ui->comboBoxRDivBISTCycles->setCurrentIndex(HMC83X_PLL_REG(0x06).rdiv_bist_cycle);

    ui->comboBoxLDCycleCnt->setCurrentIndex(HMC83X_PLL_REG(0x07).ld_window);
    ui->checkBoxLD->setChecked(HMC83X_PLL_REG(0x07).en_inter_ld ? true : false);
    ui->comboBoxLDType->setCurrentIndex(HMC83X_PLL_REG(0x07).ld_window_type);
    ui->comboBoxLDDuration->setCurrentIndex(HMC83X_PLL_REG(0x07).ld_window_duration);
    ui->comboBoxLDFreq->setCurrentIndex(HMC83X_PLL_REG(0x07).ld_timer_freq);
    ui->checkBoxAutoRelock->setChecked(HMC83X_PLL_REG(0x07).auto_relock ? true : false);

    ui->checkBoxEnBIAS->setChecked(HMC83X_PLL_REG(0x08).en_bias ? true : false);
    ui->checkBoxEnCP->setChecked(HMC83X_PLL_REG(0x08).en_cp ? true : false);
    ui->checkBoxEnPD->setChecked(HMC83X_PLL_REG(0x08).en_pd ? true : false);
    ui->checkBoxEnRefBuff->setChecked(HMC83X_PLL_REG(0x08).en_ref_buf ? true : false);
    ui->checkBoxEnVCOBuff->setChecked(HMC83X_PLL_REG(0x08).en_vco_buf ? true : false);
    ui->checkBoxGPOPAD->setChecked(HMC83X_PLL_REG(0x08).en_gpo_pad);
    ui->checkBoxVCOBuffPSB->setChecked(HMC83X_PLL_REG(0x08).en_vco_buf_psb ? true : false);
    ui->checkBoxHighRef->setChecked(HMC83X_PLL_REG(0x08).high_ref ? true : false);

    ui->lineEditCPDnGain->setText(QString("%1").arg(HMC83X_PLL_REG(0x09).cp_dn_gain));
    ui->lineEditCPUpGain->setText(QString("%1").arg(HMC83X_PLL_REG(0x09).cp_up_gain));
    ui->lineEditCPOffset->setText(QString("%1").arg(HMC83X_PLL_REG(0x09).cp_offset));
    ui->checkBoxCPOffsetUp->setChecked(HMC83X_PLL_REG(0x09).en_offset_up ? true : false);
    ui->checkBoxCPOffsetDn->setChecked(HMC83X_PLL_REG(0x09).en_offset_dn ? true : false);
    ui->checkBoxHiKcp->setChecked(HMC83X_PLL_REG(0x09).high_cp ? true : false);

    ui->comboBoxVTUNERes->setCurrentIndex(HMC83X_PLL_REG(0x0a).vtune_res);
    ui->checkBoxAutoCal->setChecked(HMC83X_PLL_REG(0x0a).dis_auto_cal ? false : true);
    ui->checkBoxVSPI->setChecked(HMC83X_PLL_REG(0x0a).vspi_trig ? false : true);
    ui->comboBoxFSMVSPIClk->setCurrentIndex(HMC83X_PLL_REG(0x0a).fsm_vspi_clk);

    ui->checkBoxPDUp->setChecked(HMC83X_PLL_REG(0x0b).en_pd_up);
    ui->checkBoxPDDn->setChecked(HMC83X_PLL_REG(0x0b).en_pd_dn);
    ui->comboBoxCSPMode->setCurrentIndex(HMC83X_PLL_REG(0x0b).csp_mode);
    ui->checkBoxForceCPUp->setChecked(HMC83X_PLL_REG(0x0b).force_cp_up);
    ui->checkBoxForceCPDn->setChecked(HMC83X_PLL_REG(0x0b).force_cp_dn);

    ui->lineEditFPDChannels->setText(QString("%1").arg(HMC83X_PLL_REG(0x0c).chs_per_pd));

    ui->comboBoxGPO->setCurrentIndex(HMC83X_PLL_REG(0x0f).gpo_sel);
    ui->comboBoxGPOTest->setCurrentIndex(HMC83X_PLL_REG(0x0f).gpo_test);
    ui->comboBoxPreventAutomuxSDO->setCurrentIndex(HMC83X_PLL_REG(0x0f).sdo_automux);
    ui->comboBoxLDO->setCurrentIndex(HMC83X_PLL_REG(0x0f).ldo_on);

    ui->lineEditVCOBand->setText(QString("%1").arg(HMC83X_PLL_REG(0x10).vco_sw));

    ui->checkBoxVCOSubsystem->setChecked(HMC83X_VCO_REG(0x01).en_vco_subsys);
    ui->checkBoxVCO->setChecked(HMC83X_VCO_REG(0x01).en_vco);
    ui->checkBoxPLLBuff->setChecked(HMC83X_VCO_REG(0x01).en_pll_buf);
    ui->checkBoxOutputStageDiv->setChecked(HMC83X_VCO_REG(0x01).en_output);
    ui->checkBoxOutputStage->setChecked(HMC83X_VCO_REG(0x01).en_output_stage);

    ui->lineEditRFDiv->setText(QString("%1").arg(HMC83X_VCO_REG(0x02).div));
    if (LOType == HMC832) {
        ui->comboBoxRFGain->setEnabled(false);
        ui->comboBoxPerformance->setCurrentIndex(HMC832_VCO_REG(0x03).perf_mode);
        ui->checkBoxRF_N->setChecked(HMC832_VCO_REG(0x03).en_rf_n);
        ui->checkBoxRF_P->setChecked(HMC832_VCO_REG(0x03).en_rf_p);
        ui->comboBoxReturnLoss->setCurrentIndex(HMC832_VCO_REG(0x03).return_loss);
        ui->comboBoxMuteMode->setCurrentIndex(HMC832_VCO_REG(0x03).mute_mode);
        ui->comboBoxDividerGain->setCurrentIndex(HMC83X_VCO_REG(0x07).gain_ctrl);
    } else if (LOType == HMC833) {
        ui->comboBoxRFGain->setCurrentIndex(HMC83X_VCO_REG(0x02).rf_gain);
        ui->comboBoxDividerGain->setCurrentIndex(HMC83X_VCO_REG(0x02).div_stage_gain);
        ui->comboBoxPerformance->setEnabled(false);
        ui->checkBoxRF_N->setEnabled(false);
        ui->checkBoxRF_P->setEnabled(false);
        ui->comboBoxReturnLoss->setEnabled(false);
        ui->comboBoxMuteMode->setEnabled(false);
        ui->comboBoxOutDoubler->setCurrentIndex(HMC833_VCO_REG(0x03).out_doubler);
    }

    paramToReg();
}

Q_HMC832_Dlg::~Q_HMC832_Dlg()
{
    delete ui;
}

void Q_HMC832_Dlg::paramToReg()
{
    HMC83X_PLL_REG_DECL_0x00;
    HMC83X_PLL_REG_DECL_0x01;
    HMC83X_PLL_REG_DECL_0x02;
    HMC83X_PLL_REG_DECL_0x03;
    HMC83X_PLL_REG_DECL_0x04;
    HMC83X_PLL_REG_DECL_0x05;
    HMC83X_PLL_REG_DECL_0x06;
    HMC83X_PLL_REG_DECL_0x07;
    HMC83X_PLL_REG_DECL_0x08;
    HMC83X_PLL_REG_DECL_0x09;
    HMC83X_PLL_REG_DECL_0x0a;
    HMC83X_PLL_REG_DECL_0x0b;
    HMC83X_PLL_REG_DECL_0x0c;
    HMC83X_PLL_REG_DECL_0x0f;
    HMC83X_PLL_REG_DECL_0x10;
    HMC83X_PLL_REG_DECL_0x11;
    HMC83X_PLL_REG_DECL_0x12;
    HMC83X_PLL_REG_DECL_0x13;

    HMC83X_VCO_REG_DECL_0x00;
    HMC83X_VCO_REG_DECL_0x01;
    HMC83X_VCO_REG_DECL_0x02;
    HMC83X_VCO_REG_DECL_0x03;
    HMC83X_VCO_REG_DECL_0x04;
    HMC83X_VCO_REG_DECL_0x05;
    HMC83X_VCO_REG_DECL_0x06;
    HMC83X_VCO_REG_DECL_0x07;

    quint64 RFFreq = 0;
    quint64 refFreq = 0;
    quint32 RDiv = ui->lineEditRDiv->text().toUInt();

    RD_TRY
    if (!ui->lineEditRFFreq->text().isEmpty()) {
        RFFreq = freq_string_to<quint64>(ui->lineEditRFFreq->text().toStdString());
    }
    if (!ui->lineEditRefFreq->text().isEmpty()) {
        refFreq = freq_string_to<quint64>(ui->lineEditRefFreq->text().toStdString());
    }
    RD_LOG_ALL_EXCEPTION

    freq_formula_in param_in;
    freq_formula_out param_out;

    param_in.freq_vco = RFFreq;
    param_in.freq_ref = refFreq;
    param_in.r_div = RDiv;
    freq_formula(param_in,param_out);

    ui->lineEditINT->setText(QString("%1").arg(param_out._int));
    ui->lineEditFRAC->setText(QString("%1").arg(param_out._frac));

    HMC83X_PLL_REG(0x01).rst_pin_sel = ui->comboBoxRstPin->currentIndex();
    HMC83X_PLL_REG(0x01).rst_from_spi = ui->checkBoxPD->isChecked() ? 1 : 0;
    HMC83X_PLL_REG(0x01).keep_bias_on = ui->checkBoxKeepBiasOn->isChecked() ? 1 : 0;
    HMC83X_PLL_REG(0x01).keep_pd_on = ui->checkBoxKeepPDOn->isChecked() ? 1 : 0;
    HMC83X_PLL_REG(0x01).keep_cp_on = ui->checkBoxKeepCPOn->isChecked() ? 1 : 0;
    HMC83X_PLL_REG(0x01).keep_ref_buff_on = ui->checkBoxKeepRefBuffOn->isChecked() ? 1 : 0;
    HMC83X_PLL_REG(0x01).keep_vco_on = ui->checkBoxKeepVCOOn->isChecked() ? 1 : 0;
    HMC83X_PLL_REG(0x01).keep_gpo_on = ui->checkBoxKeepGPOOn->isChecked() ? 1 : 0;

    HMC83X_PLL_REG(0x02).r_div = ui->lineEditRDiv->text().toUInt();

    HMC83X_PLL_REG(0x03)._int = ui->lineEditINT->text().toUInt();

    HMC83X_PLL_REG(0x04)._frac = ui->lineEditFRAC->text().toUInt();

    HMC83X_PLL_REG(0x06).seed = ui->comboBoxSeed->currentIndex();
    HMC83X_PLL_REG(0x06).order = ui->comboBoxModOrder->currentIndex();
    HMC83X_PLL_REG(0x06).frac_bypass = ui->checkBoxFracBypass->isChecked() ? 1 : 0;
    HMC83X_PLL_REG(0x06).auto_seed = ui->checkBoxAutoSeed->isChecked() ? 1 : 0;
    HMC83X_PLL_REG(0x06).mod_clk_src = ui->comboBoxModClk->currentIndex();
    HMC83X_PLL_REG(0x06).en_sd = ui->checkBoxSD->isChecked() ? 1 : 0;
    HMC83X_PLL_REG(0x06).en_bist = ui->checkBoxBIST->isChecked() ? 1 : 0;
    HMC83X_PLL_REG(0x06).rdiv_bist_cycle = ui->comboBoxRDivBISTCycles->currentIndex();

    HMC83X_PLL_REG(0x07).ld_window = ui->comboBoxLDCycleCnt->currentIndex();
    HMC83X_PLL_REG(0x07).en_inter_ld = ui->checkBoxLD->isChecked() ? 1 : 0;
    HMC83X_PLL_REG(0x07).ld_window_type = ui->comboBoxLDType->currentIndex();
    HMC83X_PLL_REG(0x07).ld_window_duration = ui->comboBoxLDDuration->currentIndex();
    HMC83X_PLL_REG(0x07).ld_timer_freq = ui->comboBoxLDFreq->currentIndex();
    HMC83X_PLL_REG(0x07).auto_relock = ui->checkBoxAutoRelock->isChecked() ? 1 : 0;

    HMC83X_PLL_REG(0x08).en_bias = ui->checkBoxEnBIAS->isChecked() ? 1 : 0;
    HMC83X_PLL_REG(0x08).en_cp = ui->checkBoxEnCP->isChecked() ? 1 : 0;
    HMC83X_PLL_REG(0x08).en_pd = ui->checkBoxEnPD->isChecked() ? 1 : 0;
    HMC83X_PLL_REG(0x08).en_ref_buf = ui->checkBoxEnRefBuff->isChecked() ? 1 : 0;
    HMC83X_PLL_REG(0x08).en_vco_buf = ui->checkBoxEnVCOBuff->isChecked() ? 1 : 0;
    HMC83X_PLL_REG(0x08).en_gpo_pad = ui->checkBoxGPOPAD->isChecked() ? 1 : 0;
    HMC83X_PLL_REG(0x08).en_vco_buf_psb = ui->checkBoxVCOBuffPSB->isChecked() ? 1 : 0;
    HMC83X_PLL_REG(0x08).high_ref = ui->checkBoxHighRef->isChecked() ? 1 : 0;

    HMC83X_PLL_REG(0x09).cp_dn_gain = ui->lineEditCPDnGain->text().toUInt();
    HMC83X_PLL_REG(0x09).cp_up_gain = ui->lineEditCPUpGain->text().toUInt();
    HMC83X_PLL_REG(0x09).cp_offset = ui->lineEditCPOffset->text().toUInt();
    HMC83X_PLL_REG(0x09).en_offset_up = ui->checkBoxCPOffsetUp->isChecked() ? 1 : 0;
    HMC83X_PLL_REG(0x09).en_offset_dn = ui->checkBoxCPOffsetDn->isChecked() ? 1 : 0;
    HMC83X_PLL_REG(0x09).high_cp = ui->checkBoxHiKcp->isChecked() ? 1 : 0;

    HMC83X_PLL_REG(0x0a).vtune_res = ui->comboBoxVTUNERes->currentIndex();
    HMC83X_PLL_REG(0x0a).dis_auto_cal = ui->checkBoxAutoCal->isChecked() ? 0 : 1;
    HMC83X_PLL_REG(0x0a).vspi_trig = ui->checkBoxVSPI->isChecked() ? 0 : 1;
    HMC83X_PLL_REG(0x0a).fsm_vspi_clk = ui->comboBoxFSMVSPIClk->currentIndex();

    HMC83X_PLL_REG(0x0b).en_pd_up = ui->checkBoxPDUp->isChecked() ? 1 : 0;
    HMC83X_PLL_REG(0x0b).en_pd_dn = ui->checkBoxPDDn->isChecked() ? 1 : 0;
    HMC83X_PLL_REG(0x0b).csp_mode = ui->comboBoxCSPMode->currentIndex();
    HMC83X_PLL_REG(0x0b).force_cp_up = ui->checkBoxForceCPUp->isChecked() ? 1 : 0;
    HMC83X_PLL_REG(0x0b).force_cp_dn = ui->checkBoxForceCPDn->isChecked() ? 1 : 0;

    HMC83X_PLL_REG(0x0c).chs_per_pd = ui->lineEditFPDChannels->text().toUInt();

    HMC83X_PLL_REG(0x0f).gpo_sel = ui->comboBoxGPO->currentIndex();
    HMC83X_PLL_REG(0x0f).gpo_test = ui->comboBoxGPOTest->currentIndex();
    HMC83X_PLL_REG(0x0f).sdo_automux = ui->comboBoxPreventAutomuxSDO->currentIndex();
    HMC83X_PLL_REG(0x0f).ldo_on = ui->comboBoxLDO->currentIndex();

    HMC83X_PLL_REG(0x10).vco_sw = ui->lineEditVCOBand->text().toUInt();

    HMC83X_VCO_REG(0x01).en_vco_subsys = ui->checkBoxVCOSubsystem->isChecked() ? 1 : 0;
    HMC83X_VCO_REG(0x01).en_vco = ui->checkBoxVCO->isChecked() ? 1 : 0;
    HMC83X_VCO_REG(0x01).en_pll_buf = ui->checkBoxPLLBuff->isChecked() ? 1 : 0;
    HMC83X_VCO_REG(0x01).en_output = ui->checkBoxOutputStageDiv->isChecked() ? 1 : 0;
    HMC83X_VCO_REG(0x01).en_output_stage = ui->checkBoxOutputStage->isChecked() ? 1 : 0;

    HMC83X_VCO_REG(0x02).div = ui->lineEditRFDiv->text().toUInt();

    hmc83x_t LOType = rf_dt3308_f::lo_type(LO);

    if (LOType == HMC832) {
        HMC832_VCO_REG(0x03).perf_mode = ui->comboBoxPerformance->currentIndex();
        HMC832_VCO_REG(0x03).en_rf_n = ui->checkBoxRF_N->isChecked() ? 1 : 0;
        HMC832_VCO_REG(0x03).en_rf_p = ui->checkBoxRF_P->isChecked() ? 1 : 0;
        HMC832_VCO_REG(0x03).return_loss = ui->comboBoxReturnLoss->currentIndex();
        HMC832_VCO_REG(0x03).mute_mode = ui->comboBoxMuteMode->currentIndex();
        HMC83X_VCO_REG(0x07).gain_ctrl = ui->comboBoxDividerGain->currentIndex();
    } else if (LOType == HMC833) {
        HMC83X_VCO_REG(0x02).rf_gain = ui->comboBoxRFGain->currentIndex();
        HMC83X_VCO_REG(0x02).div_stage_gain = ui->comboBoxDividerGain->currentIndex();
        HMC833_VCO_REG(0x03).out_doubler = ui->comboBoxOutDoubler->currentIndex();
    }

    ui->lineEditPLLReg0x00->setText(FMTED_REG_DATA(HMC83X_PLL_REG_DATA(0x00)));
    ui->lineEditPLLReg0x01->setText(FMTED_REG_DATA(HMC83X_PLL_REG_DATA(0x01)));
    ui->lineEditPLLReg0x02->setText(FMTED_REG_DATA(HMC83X_PLL_REG_DATA(0x02)));
    ui->lineEditPLLReg0x03->setText(FMTED_REG_DATA(HMC83X_PLL_REG_DATA(0x03)));
    ui->lineEditPLLReg0x04->setText(FMTED_REG_DATA(HMC83X_PLL_REG_DATA(0x04)));
    ui->lineEditPLLReg0x05->setText(FMTED_REG_DATA(HMC83X_PLL_REG_DATA(0x05)));
    ui->lineEditPLLReg0x06->setText(FMTED_REG_DATA(HMC83X_PLL_REG_DATA(0x06)));
    ui->lineEditPLLReg0x07->setText(FMTED_REG_DATA(HMC83X_PLL_REG_DATA(0x07)));
    ui->lineEditPLLReg0x08->setText(FMTED_REG_DATA(HMC83X_PLL_REG_DATA(0x08)));
    ui->lineEditPLLReg0x09->setText(FMTED_REG_DATA(HMC83X_PLL_REG_DATA(0x09)));
    ui->lineEditPLLReg0x0a->setText(FMTED_REG_DATA(HMC83X_PLL_REG_DATA(0x0a)));
    ui->lineEditPLLReg0x0b->setText(FMTED_REG_DATA(HMC83X_PLL_REG_DATA(0x0b)));
    ui->lineEditPLLReg0x0c->setText(FMTED_REG_DATA(HMC83X_PLL_REG_DATA(0x0c)));
    ui->lineEditPLLReg0x0f->setText(FMTED_REG_DATA(HMC83X_PLL_REG_DATA(0x0f)));
    ui->lineEditPLLReg0x10->setText(FMTED_REG_DATA(HMC83X_PLL_REG_DATA(0x10)));
    ui->lineEditPLLReg0x11->setText(FMTED_REG_DATA(HMC83X_PLL_REG_DATA(0x11)));
    ui->lineEditPLLReg0x12->setText(FMTED_REG_DATA(HMC83X_PLL_REG_DATA(0x12)));
    ui->lineEditPLLReg0x13->setText(FMTED_REG_DATA(HMC83X_PLL_REG_DATA(0x13)));

    ui->lineEditVCOReg0x00->setText(FMTED_REG_DATA(HMC83X_VCO_REG_DATA(0x00)));
    ui->lineEditVCOReg0x01->setText(FMTED_REG_DATA(HMC83X_VCO_REG_DATA(0x01)));
    ui->lineEditVCOReg0x02->setText(FMTED_REG_DATA(HMC83X_VCO_REG_DATA(0x02)));
    ui->lineEditVCOReg0x03->setText(FMTED_REG_DATA(HMC83X_VCO_REG_DATA(0x03)));
    ui->lineEditVCOReg0x04->setText(FMTED_REG_DATA(HMC83X_VCO_REG_DATA(0x04)));
    ui->lineEditVCOReg0x05->setText(FMTED_REG_DATA(HMC83X_VCO_REG_DATA(0x05)));
    ui->lineEditVCOReg0x06->setText(FMTED_REG_DATA(HMC83X_VCO_REG_DATA(0x06)));
    ui->lineEditVCOReg0x07->setText(FMTED_REG_DATA(HMC83X_VCO_REG_DATA(0x07)));
}

//void Q_HMC832_Dlg::on_pushButtonSetAllRegs_clicked()
//{
//    on_pushButtonSetPLLReg0x01_clicked();
//    on_pushButtonSetPLLReg0x02_clicked();
//    on_pushButtonSetPLLReg0x03_clicked();
//    on_pushButtonSetPLLReg0x04_clicked();
//    on_pushButtonSetPLLReg0x05_clicked();
//    on_pushButtonSetPLLReg0x06_clicked();
//    on_pushButtonSetPLLReg0x07_clicked();
//    on_pushButtonSetPLLReg0x08_clicked();
//    on_pushButtonSetPLLReg0x09_clicked();
//    on_pushButtonSetPLLReg0x0a_clicked();
//    on_pushButtonSetPLLReg0x0b_clicked();
//    on_pushButtonSetPLLReg0x0c_clicked();
//    on_pushButtonSetPLLReg0x0f_clicked();

//    on_pushButtonSetVCOReg0x00_clicked();
//    on_pushButtonSetVCOReg0x01_clicked();
//    on_pushButtonSetVCOReg0x02_clicked();
//    on_pushButtonSetVCOReg0x03_clicked();
//    on_pushButtonSetVCOReg0x04_clicked();
//    on_pushButtonSetVCOReg0x05_clicked();
//    on_pushButtonSetVCOReg0x06_clicked();
//    on_pushButtonSetVCOReg0x07_clicked();
//}

void Q_HMC832_Dlg::on_pushButtonGetAllRegs_clicked()
{
    on_pushButtonGetPLLReg0x01_clicked();
    on_pushButtonGetPLLReg0x02_clicked();
    on_pushButtonGetPLLReg0x03_clicked();
    on_pushButtonGetPLLReg0x04_clicked();
    on_pushButtonGetPLLReg0x05_clicked();
    on_pushButtonGetPLLReg0x06_clicked();
    on_pushButtonGetPLLReg0x07_clicked();
    on_pushButtonGetPLLReg0x08_clicked();
    on_pushButtonGetPLLReg0x09_clicked();
    on_pushButtonGetPLLReg0x0a_clicked();
    on_pushButtonGetPLLReg0x0b_clicked();
    on_pushButtonGetPLLReg0x0c_clicked();
    on_pushButtonGetPLLReg0x0f_clicked();
    on_pushButtonGetPLLReg0x10_clicked();
    on_pushButtonGetPLLReg0x11_clicked();
    on_pushButtonGetPLLReg0x12_clicked();
    on_pushButtonGetPLLReg0x13_clicked();
}

void Q_HMC832_Dlg::on_lineEditRFFreq_textEdited(const QString &arg1)
{
    paramToReg();
//    RF->set_lo(LO,freq_string_to<quint64>(ui->lineEditRFFreq->text().toStdString()));
}
