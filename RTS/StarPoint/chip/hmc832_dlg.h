#ifndef Q_HMC832_DLG_H
#define Q_HMC832_DLG_H

#include <QDialog>
#include "ui_hmc832.h"
#include "global.h"

namespace Ui {
class Q_HMC832_Dlg;
}

class Q_HMC832_Dlg : public QDialog
{
    Q_OBJECT

public:
    explicit Q_HMC832_Dlg(rf_dt3308_f::lo_t lo, QWidget *parent = 0);
    ~Q_HMC832_Dlg();

    void paramToReg();

private slots:

#define FMTED_REG_DATA(data) QString("0x%1").arg(data,8,16,QLatin1Char('0'))

#define SLOT_SET_GET_PLL_REG(reg) \
    void on_pushButtonSetPLLReg##reg##_clicked() { \
        if (RF == nullptr) { return; } \
        bool ok = false; \
        RF->set_lo_reg_pll(LO,reg,ui->lineEditPLLReg##reg->text().toUInt(&ok,16)); \
    } \
    void on_pushButtonGetPLLReg##reg##_clicked() { \
        if (RF == nullptr) { return; } \
        quint32 data = 0; \
        RF->get_lo_reg_pll(LO,reg,data); \
        ui->lineEditPLLReg##reg->setText(FMTED_REG_DATA(data)); \
    }

#define SLOT_SET_VCO_REG(reg) \
    void on_pushButtonSetVCOReg##reg##_clicked() { \
        if (RF == nullptr) { return; } \
        bool ok = false; \
        RF->set_lo_reg_vco(LO,reg,ui->lineEditVCOReg##reg->text().toUInt(&ok,16)); \
    }

    SLOT_SET_GET_PLL_REG(0x01)
    SLOT_SET_GET_PLL_REG(0x02)
    SLOT_SET_GET_PLL_REG(0x03)
    SLOT_SET_GET_PLL_REG(0x04)
    SLOT_SET_GET_PLL_REG(0x05)
    SLOT_SET_GET_PLL_REG(0x06)
    SLOT_SET_GET_PLL_REG(0x07)
    SLOT_SET_GET_PLL_REG(0x08)
    SLOT_SET_GET_PLL_REG(0x09)
    SLOT_SET_GET_PLL_REG(0x0a)
    SLOT_SET_GET_PLL_REG(0x0b)
    SLOT_SET_GET_PLL_REG(0x0c)
    SLOT_SET_GET_PLL_REG(0x0f)
    SLOT_SET_GET_PLL_REG(0x10)
    SLOT_SET_GET_PLL_REG(0x11)
    SLOT_SET_GET_PLL_REG(0x12)
    SLOT_SET_GET_PLL_REG(0x13)

    SLOT_SET_VCO_REG(0x00)
    SLOT_SET_VCO_REG(0x01)
    SLOT_SET_VCO_REG(0x02)
    SLOT_SET_VCO_REG(0x03)
    SLOT_SET_VCO_REG(0x04)
    SLOT_SET_VCO_REG(0x05)
    SLOT_SET_VCO_REG(0x06)
    SLOT_SET_VCO_REG(0x07)

//    void on_pushButtonSetAllRegs_clicked();

    void on_pushButtonGetAllRegs_clicked();

    void on_lineEditRFFreq_textEdited(const QString &arg1);

    void on_lineEditRefFreq_textEdited(const QString &arg1) { paramToReg(); }

    void on_lineEditRDiv_textEdited(const QString &arg1) { paramToReg(); }

    void on_comboBoxRstPin_currentIndexChanged(int index) { paramToReg(); }

    void on_checkBoxPD_clicked() { paramToReg(); }

    void on_checkBoxKeepBiasOn_clicked() { paramToReg(); }

    void on_checkBoxKeepPDOn_clicked() { paramToReg(); }

    void on_checkBoxKeepCPOn_clicked() { paramToReg(); }

    void on_checkBoxKeepRefBuffOn_clicked() { paramToReg(); }

    void on_checkBoxKeepVCOOn_clicked() { paramToReg(); }

    void on_checkBoxKeepGPOOn_clicked() { paramToReg(); }

    void on_comboBoxSeed_currentIndexChanged(int index) { paramToReg(); }

    void on_comboBoxModOrder_currentIndexChanged(int index) { paramToReg(); }

    void on_checkBoxFracBypass_clicked() { paramToReg(); }

    void on_checkBoxAutoSeed_clicked() { paramToReg(); }

    void on_comboBoxModClk_currentIndexChanged(int index) { paramToReg(); }

    void on_checkBoxSD_clicked() { paramToReg(); }

    void on_checkBoxBIST_clicked() { paramToReg(); }

    void on_comboBoxRDivBISTCycles_currentIndexChanged(int index) { paramToReg(); }

    void on_comboBoxLDCycleCnt_currentIndexChanged(int index) { paramToReg(); }

    void on_checkBoxLD_clicked() { paramToReg(); }

    void on_comboBoxLDType_currentIndexChanged(int index) { paramToReg(); }

    void on_comboBoxLDDuration_currentIndexChanged(int index) { paramToReg(); }

    void on_comboBoxLDFreq_currentIndexChanged(int index) { paramToReg(); }

    void on_checkBoxAutoRelock_clicked() { paramToReg(); }

    void on_checkBoxEnBIAS_clicked() { paramToReg(); }

    void on_checkBoxEnCP_clicked() { paramToReg(); }

    void on_checkBoxEnPD_clicked() { paramToReg(); }

    void on_checkBoxEnRefBuff_clicked() { paramToReg(); }

    void on_checkBoxEnVCOBuff_clicked() { paramToReg(); }

    void on_checkBoxGPOPAD_clicked() { paramToReg(); }

    void on_checkBoxVCOBuffPSB_clicked() { paramToReg(); }

    void on_checkBoxHighRef_clicked() { paramToReg(); }

    void on_lineEditCPDnGain_textEdited(const QString &arg1) { paramToReg(); }

    void on_lineEditCPUpGain_textEdited(const QString &arg1) { paramToReg(); }

    void on_lineEditCPOffset_textEdited(const QString &arg1) { paramToReg(); }

    void on_checkBoxCPOffsetUp_clicked() { paramToReg(); }

    void on_checkBoxCPOffsetDn_clicked() { paramToReg(); }

    void on_checkBoxHiKcp_clicked() { paramToReg(); }

    void on_comboBoxVTUNERes_currentIndexChanged(int index) { paramToReg(); }

    void on_checkBoxAutoCal_clicked() { paramToReg(); }

    void on_checkBoxVSPI_clicked() { paramToReg(); }

    void on_comboBoxFSMVSPIClk_currentIndexChanged(int index) { paramToReg(); }

    void on_checkBoxPDUp_clicked() { paramToReg(); }

    void on_checkBoxPDDn_clicked() { paramToReg(); }

    void on_comboBoxCSPMode_currentIndexChanged(int index) { paramToReg(); }

    void on_checkBoxForceCPUp_clicked() { paramToReg(); }

    void on_checkBoxForceCPDn_clicked() { paramToReg(); }

    void on_lineEditFPDChannels_textEdited(const QString &arg1) { paramToReg(); }

    void on_comboBoxGPO_currentIndexChanged(int index) { paramToReg(); }

    void on_comboBoxGPOTest_currentIndexChanged(int index) { paramToReg(); }

    void on_comboBoxPreventAutomuxSDO_currentIndexChanged(int index) { paramToReg(); }

    void on_comboBoxLDO_currentIndexChanged(int index) { paramToReg(); }

    void on_lineEditVCOBand_textEdited(const QString &arg1) { paramToReg(); }

public:
    rf_dt3308_f *RF;
    rf_dt3308_f::lo_t LO;

private:
    Ui::Q_HMC832_Dlg *ui;
};

#endif // Q_HMC832_DLG_H
