#ifndef Q_ADF5355_DLG_H
#define Q_ADF5355_DLG_H

#include "global.h"
#include <QDialog>

namespace Ui {
class Q_ADF5355_Dlg;
}

class Q_ADF5355_Dlg : public QDialog
{
    Q_OBJECT

public:
    enum applyMode {
        Write_Once = 0,
        Update_Freq
    };

public:
    sp1401_r1a *_sp1401_r1a;
    sp1401_r1c *_sp1401_r1c;
    qint8 loIdx;
public:
    void param2Reg(uint32_t *reg);
    void param2RegR();
    void regW2Reg(uint32_t *reg);
public:
    explicit Q_ADF5355_Dlg(QWidget *parent = nullptr);
    ~Q_ADF5355_Dlg();

private slots:
    void on_pushButton_14_clicked();

    void on_pushButtonCopyTo0_clicked();

    void on_pushButtonCopyTo1_clicked();

    void on_pushButtonCopyTo2_clicked();

    void on_pushButtonCopyTo3_clicked();

    void on_pushButtonCopyTo4_clicked();

    void on_pushButtonCopyTo5_clicked();

    void on_pushButtonCopyTo6_clicked();

    void on_pushButtonCopyTo7_clicked();

    void on_pushButtonCopyTo8_clicked();

    void on_pushButtonCopyTo9_clicked();

    void on_pushButtonCopyTo10_clicked();

    void on_pushButtonCopyTo11_clicked();

    void on_pushButtonCopyTo12_clicked();

    void on_lineEditVCOFreq_textChanged(const QString &arg1);

    void on_lineEditRefFreq_textChanged(const QString &arg1);

    void on_checkBoxRefDoubler_clicked();

    void on_checkBoxRefDivider_clicked();

    void on_lineEditRCounter_textChanged(const QString &arg1);

    void on_comboBoxPrescaler_currentIndexChanged(int index);

    void on_checkBoxAutoCal_clicked();

    void on_lineEditPhaseValue_textChanged(const QString &arg1);

    void on_checkBoxPhaseAjust_clicked();

    void on_checkBoxPhaseResync_clicked();

    void on_comboBoxSDLoadReset_currentIndexChanged(int index);

    void on_checkBoxCounterReset_clicked();

    void on_checkBoxCPThreeState_clicked();

    void on_checkBoxPowerDown_clicked();

    void on_comboBoxPDPolarity_currentIndexChanged(int index);

    void on_comboBoxMuxLogic_currentIndexChanged(int index);

    void on_comboBoxRefMode_currentIndexChanged(int index);

    void on_comboBoxCurrentSetting_currentIndexChanged(int index);

    void on_checkBoxDoubleBuff_clicked();

    void on_checkBoxRDIV2_clicked();

    void on_comboBoxMuxOut_currentIndexChanged(int index);

    void on_comboBoxRfOutputPower_currentIndexChanged(int index);

    void on_checkBoxRfOutA_clicked();

    void on_comboBoxRfOutB_currentIndexChanged(int index);

    void on_comboBoxMTLD_currentIndexChanged(int index);

    void on_lineEditCPBleedCurrent_textChanged(const QString &arg1);

    void on_comboBoxRfDividerSel_currentIndexChanged(int index);

    void on_comboBoxFeedbackSel_currentIndexChanged(int index);

    void on_checkBoxNegativeBleed_clicked();

    void on_checkBoxGateBleed_clicked();

    void on_comboBoxLDMode_currentIndexChanged(int index);

    void on_comboBoxFracN_LDPrecision_currentIndexChanged(int index);

    void on_checkBoxLOLMode_clicked();

    void on_comboBoxLDCycleCount_currentIndexChanged(int index);

    void on_comboBoxLeSync_currentIndexChanged(int index);

    void on_comboBoxSynthesizerLockTimeout_currentIndexChanged(int index);

    void on_comboBoxAutomaticLevelTimeout_currentIndexChanged(int index);

    void on_lineEditTimeout_textChanged(const QString &arg1);

    void on_lineEditVCOBandDivision_textChanged(const QString &arg1);

    void on_checkBoxADCEnable_clicked();

    void on_checkBoxADCConversion_clicked();

    void on_lineEditADCClockDivider_textChanged(const QString &arg1);

    void on_lineEditResyncClock_textChanged(const QString &arg1);

private:
    Ui::Q_ADF5355_Dlg *ui;
};

#endif
