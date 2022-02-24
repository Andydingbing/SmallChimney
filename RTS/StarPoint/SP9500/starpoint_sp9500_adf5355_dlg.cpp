#include "starpoint_sp9500_adf5355_dlg.h"
#include "algo_chip_pll.hpp"
#include <QThread>

using namespace ns_adf5355;

Q_ADF5355_Dlg::Q_ADF5355_Dlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Q_ADF5355_Dlg)
{
    ui->setupUi(this);

    setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);
    setFixedSize(width(),height());

    _sp1401_r1a = nullptr;
    _sp1401_r1c = nullptr;

    ui->lineEditRefFreq->setText("100M");
    ui->checkBoxRefDoubler->setChecked(false);
    ui->checkBoxRefDivider->setChecked(false);
    ui->lineEditRCounter->setText("1");
    ui->comboBoxPrescaler->setCurrentIndex(0);
    ui->checkBoxAutoCal->setChecked(true);
    ui->lineEditPhaseValue->setText("1");
    ui->checkBoxPhaseAjust->setChecked(false);
    ui->checkBoxPhaseResync->setChecked(false);
    ui->comboBoxSDLoadReset->setCurrentIndex(0);
    ui->checkBoxCounterReset->setChecked(true);
    ui->checkBoxCPThreeState->setChecked(false);
    ui->checkBoxPowerDown->setChecked(false);
    ui->comboBoxPDPolarity->setCurrentIndex(1);
    ui->comboBoxMuxLogic->setCurrentIndex(1);
    ui->comboBoxRefMode->setCurrentIndex(1);
    ui->comboBoxCurrentSetting->setCurrentIndex(0);
    ui->checkBoxDoubleBuff->setChecked(false);
    ui->checkBoxRDIV2->setChecked(false);
    ui->comboBoxMuxOut->setCurrentIndex(6);
    ui->comboBoxRfOutputPower->setCurrentIndex(3);
    ui->checkBoxRfOutA->setChecked(true);
    ui->comboBoxRfOutB->setCurrentIndex(1);
    ui->comboBoxMTLD->setCurrentIndex(0);
    ui->lineEditCPBleedCurrent->setText("5");
    ui->comboBoxRfDividerSel->setCurrentIndex(0);
    ui->comboBoxFeedbackSel->setCurrentIndex(1);
    ui->checkBoxNegativeBleed->setChecked(true);
    ui->checkBoxGateBleed->setChecked(false);
    ui->comboBoxLDMode->setCurrentIndex(0);
    ui->comboBoxFracN_LDPrecision->setCurrentIndex(3);
    ui->checkBoxLOLMode->setChecked(false);
    ui->comboBoxLDCycleCount->setCurrentIndex(0);
    ui->comboBoxLeSync->setCurrentIndex(1);
    ui->comboBoxSynthesizerLockTimeout->setCurrentIndex(30);
    ui->comboBoxAutomaticLevelTimeout->setCurrentIndex(30);
    ui->lineEditTimeout->setText("200");
    ui->lineEditVCOBandDivision->setText("40");
    ui->checkBoxADCEnable->setChecked(true);
    ui->checkBoxADCConversion->setChecked(true);
    ui->lineEditADCClockDivider->setText("150");
    ui->lineEditResyncClock->setText("20000");

    ui->checkBoxUseHex->setChecked(false);
    ui->comboBoxApplyMode->setCurrentIndex(Update_Freq);

    param2RegR();
    on_pushButtonCopyTo0_clicked();
    on_pushButtonCopyTo1_clicked();
    on_pushButtonCopyTo2_clicked();
    on_pushButtonCopyTo3_clicked();
    on_pushButtonCopyTo4_clicked();
    on_pushButtonCopyTo5_clicked();
    on_pushButtonCopyTo6_clicked();
    on_pushButtonCopyTo7_clicked();
    on_pushButtonCopyTo8_clicked();
    on_pushButtonCopyTo9_clicked();
    on_pushButtonCopyTo10_clicked();
    on_pushButtonCopyTo11_clicked();
    on_pushButtonCopyTo12_clicked();
}

Q_ADF5355_Dlg::~Q_ADF5355_Dlg()
{
    delete ui;
}

void Q_ADF5355_Dlg::param2Reg(uint32_t *reg)
{
    quint64 vcoFreq = 0;
    quint64 refFreq = 0;

    if (!ui->lineEditVCOFreq->text().isEmpty()) {
        vcoFreq = freq_string_to<quint64>(ui->lineEditVCOFreq->text().toStdString());
    }
    if (!ui->lineEditRefFreq->text().isEmpty()) {
        refFreq = freq_string_to<quint64>(ui->lineEditRefFreq->text().toStdString());
    }

    freq_formula_in param_in;
    freq_formula_out param_out;

    bool refDoubler = ui->checkBoxRefDoubler->isChecked();
    bool refDivider = ui->checkBoxRefDivider->isChecked();
    uint32_t rCounter = ui->lineEditRCounter->text().toUInt();

    if (!(vcoFreq <=0 || refFreq <= 0)) {
        param_in.freq_vco = vcoFreq;
        param_in.freq_ref = refFreq;
        param_in.ref_doubler = refDoubler;
        param_in.ref_divider = refDivider;
        param_in.r_counter = rCounter;
        ns_adf5355::freq_formula(param_in,param_out);
    }

    ui->lineEditINT->setText(QString("%1").arg(param_out._int));
    ui->lineEditFRAC1->setText(QString("%1").arg(param_out._frac1));
    ui->lineEditFRAC2->setText(QString("%1").arg(param_out._frac2));
    ui->lineEditMOD1->setText(QString("%1").arg(param_out._mod1));
    ui->lineEditMOD2->setText(QString("%1").arg(param_out._mod2));

    uint32_t prescaler = ui->comboBoxPrescaler->currentIndex();
    uint32_t autoCal = ui->checkBoxAutoCal->isChecked() ? 1 : 0;
    reg[0] = ns_adf5355::reg0(param_out._int,prescaler,autoCal);
    reg[1] = ns_adf5355::reg1(param_out._frac1);
    reg[2] = ns_adf5355::reg2(param_out._mod2,param_out._frac2);

    uint32_t phaseValue = ui->lineEditPhaseValue->text().toUInt();
    uint32_t phaseAjust = ui->checkBoxPhaseAjust->isChecked();
    uint32_t phaseResync = ui->checkBoxPhaseResync->isChecked();
    uint32_t sdLoadReset = ui->comboBoxSDLoadReset->currentIndex();
    reg[3] = ns_adf5355::reg3(phaseValue,phaseAjust,phaseResync,sdLoadReset);

    uint32_t counterReset = ui->checkBoxCounterReset->isChecked() ? 1 : 0;
    uint32_t cpThreeState = ui->checkBoxCPThreeState->isChecked() ? 1 : 0;
    uint32_t powerDown = ui->checkBoxPowerDown->isChecked() ? 1 : 0;
    uint32_t pdPolarity = ui->comboBoxPDPolarity->currentIndex();
    uint32_t muxLogic = ui->comboBoxMuxLogic->currentIndex();
    uint32_t refMode = ui->comboBoxRefMode->currentIndex();
    uint32_t currentSet = ui->comboBoxCurrentSetting->currentIndex();
    uint32_t doubleBuff = ui->checkBoxDoubleBuff->isChecked();
    uint32_t RDIV2 = ui->checkBoxRDIV2->isChecked() ? 1 : 0;
    uint32_t muxOut = ui->comboBoxMuxOut->currentIndex();
    reg[4] = ns_adf5355::reg4(
                counterReset,
                cpThreeState,
                powerDown,
                pdPolarity,
                muxLogic,
                refMode,
                currentSet,
                doubleBuff,
                rCounter,
                RDIV2,
                refDoubler,
                muxOut);

    reg[5] = ns_adf5355::reg5();

    uint32_t rfOutputPower = ui->comboBoxRfOutputPower->currentIndex();
    uint32_t rfOutA = ui->checkBoxRfOutA->isChecked() ? 1 : 0;
    uint32_t rfOutB = ui->comboBoxRfOutB->currentIndex();
    uint32_t MTLD = ui->comboBoxMTLD->currentIndex();
    uint32_t cpBC = ui->lineEditCPBleedCurrent->text().toUInt();
    uint32_t rfDividerSel = ui->comboBoxRfDividerSel->currentIndex();
    uint32_t feedbackSel = ui->comboBoxFeedbackSel->currentIndex();
    uint32_t negativeBleed = ui->checkBoxNegativeBleed->isChecked() ? 1 : 0;
    uint32_t gatedBleed = ui->checkBoxGateBleed->isChecked() ? 1 : 0;
    reg[6] = ns_adf5355::reg6(
                rfOutputPower,
                rfOutA,
                rfOutB,
                MTLD,
                cpBC,
                rfDividerSel,
                feedbackSel,
                negativeBleed,
                gatedBleed);

    uint32_t ldMode = ui->comboBoxLDMode->currentIndex();
    uint32_t fracN_LDPrecision = ui->comboBoxFracN_LDPrecision->currentIndex();
    uint32_t lolMode = ui->checkBoxLOLMode->isChecked() ? 1 : 0;
    uint32_t ldCycleCount = ui->comboBoxLDCycleCount->currentIndex();
    uint32_t leSync = ui->comboBoxLeSync->currentIndex();
    reg[7] = ns_adf5355::reg7(ldMode,fracN_LDPrecision,lolMode,ldCycleCount,leSync);

    reg[8] = ns_adf5355::reg8();

    uint32_t synthesizerLockTimeout = ui->comboBoxSynthesizerLockTimeout->currentIndex();
    uint32_t autoLevelTimeout = ui->comboBoxAutomaticLevelTimeout->currentIndex();
    uint32_t timeout = ui->lineEditTimeout->text().toUInt();
    uint32_t vcoBandDiv = ui->lineEditVCOBandDivision->text().toUInt();
    reg[9] = ns_adf5355::reg9(synthesizerLockTimeout,autoLevelTimeout,timeout,vcoBandDiv);

    uint32_t adcEnable = ui->checkBoxADCEnable->isChecked() ? 1 : 0;
    uint32_t adcConversion = ui->checkBoxADCConversion->isChecked() ? 1 : 0;
    uint32_t adcLockDiv = ui->lineEditADCClockDivider->text().toUInt();
    reg[10] = ns_adf5355::reg10(adcEnable,adcConversion,adcLockDiv);

    reg[11] = ns_adf5355::reg11();

    uint32_t resyncClock = ui->lineEditResyncClock->text().toUInt();
    reg[12] = ns_adf5355::reg12(resyncClock);
}

void Q_ADF5355_Dlg::param2RegR()
{
    uint32_t reg[13] = {0};
    for (quint32 i = 0;i < ARRAY_SIZE(reg);i ++) {
        reg[i] = 0;
    }

    param2Reg(reg);

    ui->lineEditRegR0->setText(QString("0x%1").arg(reg[0],8,16,QLatin1Char('0')));
    ui->lineEditRegR1->setText(QString("0x%1").arg(reg[1],8,16,QLatin1Char('0')));
    ui->lineEditRegR2->setText(QString("0x%1").arg(reg[2],8,16,QLatin1Char('0')));
    ui->lineEditRegR3->setText(QString("0x%1").arg(reg[3],8,16,QLatin1Char('0')));
    ui->lineEditRegR4->setText(QString("0x%1").arg(reg[4],8,16,QLatin1Char('0')));
    ui->lineEditRegR5->setText(QString("0x%1").arg(reg[5],8,16,QLatin1Char('0')));
    ui->lineEditRegR6->setText(QString("0x%1").arg(reg[6],8,16,QLatin1Char('0')));
    ui->lineEditRegR7->setText(QString("0x%1").arg(reg[7],8,16,QLatin1Char('0')));
    ui->lineEditRegR8->setText(QString("0x%1").arg(reg[8],8,16,QLatin1Char('0')));
    ui->lineEditRegR9->setText(QString("0x%1").arg(reg[9],8,16,QLatin1Char('0')));
    ui->lineEditRegR10->setText(QString("0x%1").arg(reg[10],8,16,QLatin1Char('0')));
    ui->lineEditRegR11->setText(QString("0x%1").arg(reg[11],8,16,QLatin1Char('0')));
    ui->lineEditRegR12->setText(QString("0x%1").arg(reg[12],8,16,QLatin1Char('0')));
}

void Q_ADF5355_Dlg::regW2Reg(uint32_t *reg)
{
    reg[0] = ui->lineEditRegW0->text().toUInt();
    reg[1] = ui->lineEditRegW1->text().toUInt();
    reg[2] = ui->lineEditRegW2->text().toUInt();
    reg[3] = ui->lineEditRegW3->text().toUInt();
    reg[4] = ui->lineEditRegW4->text().toUInt();
    reg[5] = ui->lineEditRegW5->text().toUInt();
    reg[6] = ui->lineEditRegW6->text().toUInt();
    reg[7] = ui->lineEditRegW7->text().toUInt();
    reg[8] = ui->lineEditRegW8->text().toUInt();
    reg[9] = ui->lineEditRegW9->text().toUInt();
    reg[10] = ui->lineEditRegW10->text().toUInt();
    reg[11] = ui->lineEditRegW11->text().toUInt();
    reg[12] = ui->lineEditRegW12->text().toUInt();
}

void Q_ADF5355_Dlg::on_pushButton_14_clicked()
{
    uint32_t reg[13] = {0};
    for (quint8 i = 0;i < ARRAY_SIZE(reg);i ++) {
        reg[i] = 0;
    }

    if (ui->checkBoxUseHex->isChecked()) {
        regW2Reg(reg);
    } else {
        param2Reg(reg);
    }

    if (_sp1401_r1a) {
        DECLARE_R1A_SETLOREG_FUNCTION_MAP

        if (Write_Once == ui->comboBoxApplyMode->currentIndex()) {
            for (qint8 i = 0;i < 13;i ++) {
                (_sp1401_r1a->*set_r1a_lo_reg[loIdx])(reg[12 - i]);
            }
        }
        else {
            for (qint8 i = 0;i < 13;i ++) {
                (_sp1401_r1a->*set_r1a_lo_reg[loIdx])(reg[12 - i]);
            }
            reg[4] = reg[4] | (1 << 4);
            (_sp1401_r1a->*set_r1a_lo_reg[loIdx])(reg[4]);
            (_sp1401_r1a->*set_r1a_lo_reg[loIdx])(reg[2]);
            (_sp1401_r1a->*set_r1a_lo_reg[loIdx])(reg[1]);
            reg[0] = reg[0] & (~(1 << 21));
            (_sp1401_r1a->*set_r1a_lo_reg[loIdx])(reg[0]);
            reg[4] = reg[4] & (~(1 << 4));
            (_sp1401_r1a->*set_r1a_lo_reg[loIdx])(reg[4]);
            QThread::msleep(10);
            reg[0] = reg[0] | (1 << 21);
            (_sp1401_r1a->*set_r1a_lo_reg[loIdx])(reg[0]);
        }
    }

    if (_sp1401_r1c) {
        DECLARE_R1C_SETLOREG_FUNCTION_MAP

        if (Write_Once == ui->comboBoxApplyMode->currentIndex()) {
            for (qint8 i = 0;i < 13;i ++) {
                (_sp1401_r1c->*set_r1c_lo_reg[loIdx])(reg[12 - i]);
            }
        }
        else {
            for (qint8 i = 0;i < 13;i ++) {
                (_sp1401_r1c->*set_r1c_lo_reg[loIdx])(reg[12 - i]);
            }
            reg[4] = reg[4] | (1 << 4);
            (_sp1401_r1c->*set_r1c_lo_reg[loIdx])(reg[4]);
            (_sp1401_r1c->*set_r1c_lo_reg[loIdx])(reg[2]);
            (_sp1401_r1c->*set_r1c_lo_reg[loIdx])(reg[1]);
            reg[0] = reg[0] & (~(1 << 21));
            (_sp1401_r1c->*set_r1c_lo_reg[loIdx])(reg[0]);
            reg[4] = reg[4] & (~(1 << 4));
            (_sp1401_r1c->*set_r1c_lo_reg[loIdx])(reg[4]);
            QThread::msleep(10);
            reg[0] = reg[0] | (1 << 21);
            (_sp1401_r1c->*set_r1c_lo_reg[loIdx])(reg[0]);
        }
    }

    setFocus();
}

void Q_ADF5355_Dlg::on_pushButtonCopyTo0_clicked()
{
    ui->lineEditRegW0->setText(ui->lineEditRegR0->text());
}

void Q_ADF5355_Dlg::on_pushButtonCopyTo1_clicked()
{
    ui->lineEditRegW1->setText(ui->lineEditRegR1->text());
}

void Q_ADF5355_Dlg::on_pushButtonCopyTo2_clicked()
{
    ui->lineEditRegW2->setText(ui->lineEditRegR2->text());
}

void Q_ADF5355_Dlg::on_pushButtonCopyTo3_clicked()
{
    ui->lineEditRegW3->setText(ui->lineEditRegR3->text());
}

void Q_ADF5355_Dlg::on_pushButtonCopyTo4_clicked()
{
    ui->lineEditRegW4->setText(ui->lineEditRegR4->text());
}

void Q_ADF5355_Dlg::on_pushButtonCopyTo5_clicked()
{
    ui->lineEditRegW5->setText(ui->lineEditRegR5->text());
}

void Q_ADF5355_Dlg::on_pushButtonCopyTo6_clicked()
{
    ui->lineEditRegW6->setText(ui->lineEditRegR6->text());
}

void Q_ADF5355_Dlg::on_pushButtonCopyTo7_clicked()
{
    ui->lineEditRegW7->setText(ui->lineEditRegR7->text());
}

void Q_ADF5355_Dlg::on_pushButtonCopyTo8_clicked()
{
    ui->lineEditRegW8->setText(ui->lineEditRegR8->text());
}

void Q_ADF5355_Dlg::on_pushButtonCopyTo9_clicked()
{
    ui->lineEditRegW9->setText(ui->lineEditRegR9->text());
}

void Q_ADF5355_Dlg::on_pushButtonCopyTo10_clicked()
{
    ui->lineEditRegW10->setText(ui->lineEditRegR10->text());
}

void Q_ADF5355_Dlg::on_pushButtonCopyTo11_clicked()
{
    ui->lineEditRegW11->setText(ui->lineEditRegR11->text());
}

void Q_ADF5355_Dlg::on_pushButtonCopyTo12_clicked()
{
    ui->lineEditRegW12->setText(ui->lineEditRegR12->text());
}

void Q_ADF5355_Dlg::on_lineEditVCOFreq_textChanged(const QString &arg1)
{
    param2RegR();
}

void Q_ADF5355_Dlg::on_lineEditRefFreq_textChanged(const QString &arg1)
{
    param2RegR();
}

void Q_ADF5355_Dlg::on_checkBoxRefDoubler_clicked()
{
    param2RegR();
}

void Q_ADF5355_Dlg::on_checkBoxRefDivider_clicked()
{
    param2RegR();
}

void Q_ADF5355_Dlg::on_lineEditRCounter_textChanged(const QString &arg1)
{
    param2RegR();
}

void Q_ADF5355_Dlg::on_comboBoxPrescaler_currentIndexChanged(int index)
{
    param2RegR();
}

void Q_ADF5355_Dlg::on_checkBoxAutoCal_clicked()
{
    param2RegR();
}

void Q_ADF5355_Dlg::on_lineEditPhaseValue_textChanged(const QString &arg1)
{
    param2RegR();
}

void Q_ADF5355_Dlg::on_checkBoxPhaseAjust_clicked()
{
    param2RegR();
}

void Q_ADF5355_Dlg::on_checkBoxPhaseResync_clicked()
{
    param2RegR();
}

void Q_ADF5355_Dlg::on_comboBoxSDLoadReset_currentIndexChanged(int index)
{
    param2RegR();
}

void Q_ADF5355_Dlg::on_checkBoxCounterReset_clicked()
{
    param2RegR();
}

void Q_ADF5355_Dlg::on_checkBoxCPThreeState_clicked()
{
    param2RegR();
}

void Q_ADF5355_Dlg::on_checkBoxPowerDown_clicked()
{
    param2RegR();
}

void Q_ADF5355_Dlg::on_comboBoxPDPolarity_currentIndexChanged(int index)
{
    param2RegR();
}

void Q_ADF5355_Dlg::on_comboBoxMuxLogic_currentIndexChanged(int index)
{
    param2RegR();
}

void Q_ADF5355_Dlg::on_comboBoxRefMode_currentIndexChanged(int index)
{
    param2RegR();
}

void Q_ADF5355_Dlg::on_comboBoxCurrentSetting_currentIndexChanged(int index)
{
    param2RegR();
}

void Q_ADF5355_Dlg::on_checkBoxDoubleBuff_clicked()
{
    param2RegR();
}

void Q_ADF5355_Dlg::on_checkBoxRDIV2_clicked()
{
    param2RegR();
}

void Q_ADF5355_Dlg::on_comboBoxMuxOut_currentIndexChanged(int index)
{
    param2RegR();
}

void Q_ADF5355_Dlg::on_comboBoxRfOutputPower_currentIndexChanged(int index)
{
    param2RegR();
}

void Q_ADF5355_Dlg::on_checkBoxRfOutA_clicked()
{
    param2RegR();
}

void Q_ADF5355_Dlg::on_comboBoxRfOutB_currentIndexChanged(int index)
{
    param2RegR();
}

void Q_ADF5355_Dlg::on_comboBoxMTLD_currentIndexChanged(int index)
{
    param2RegR();
}

void Q_ADF5355_Dlg::on_lineEditCPBleedCurrent_textChanged(const QString &arg1)
{
    param2RegR();
}

void Q_ADF5355_Dlg::on_comboBoxRfDividerSel_currentIndexChanged(int index)
{
    param2RegR();
}

void Q_ADF5355_Dlg::on_comboBoxFeedbackSel_currentIndexChanged(int index)
{
    param2RegR();
}

void Q_ADF5355_Dlg::on_checkBoxNegativeBleed_clicked()
{
    param2RegR();
}

void Q_ADF5355_Dlg::on_checkBoxGateBleed_clicked()
{
    param2RegR();
}

void Q_ADF5355_Dlg::on_comboBoxLDMode_currentIndexChanged(int index)
{
    param2RegR();
}

void Q_ADF5355_Dlg::on_comboBoxFracN_LDPrecision_currentIndexChanged(int index)
{
    param2RegR();
}

void Q_ADF5355_Dlg::on_checkBoxLOLMode_clicked()
{
    param2RegR();
}

void Q_ADF5355_Dlg::on_comboBoxLDCycleCount_currentIndexChanged(int index)
{
    param2RegR();
}

void Q_ADF5355_Dlg::on_comboBoxLeSync_currentIndexChanged(int index)
{
    param2RegR();
}

void Q_ADF5355_Dlg::on_comboBoxSynthesizerLockTimeout_currentIndexChanged(int index)
{
    param2RegR();
}

void Q_ADF5355_Dlg::on_comboBoxAutomaticLevelTimeout_currentIndexChanged(int index)
{
    param2RegR();
}

void Q_ADF5355_Dlg::on_lineEditTimeout_textChanged(const QString &arg1)
{
    param2RegR();
}

void Q_ADF5355_Dlg::on_lineEditVCOBandDivision_textChanged(const QString &arg1)
{
    param2RegR();
}

void Q_ADF5355_Dlg::on_checkBoxADCEnable_clicked()
{
    param2RegR();
}

void Q_ADF5355_Dlg::on_checkBoxADCConversion_clicked()
{
    param2RegR();
}

void Q_ADF5355_Dlg::on_lineEditADCClockDivider_textChanged(const QString &arg1)
{
    param2RegR();
}

void Q_ADF5355_Dlg::on_lineEditResyncClock_textChanged(const QString &arg1)
{
    param2RegR();
}
