#ifndef Q_SP2406_Widget_H
#define Q_SP2406_Widget_H

#include "global.h"

namespace NS_SP9500X {

class Q_SP2406_Widget : public Q_RD_Widget
{
    Q_OBJECT

public:
    explicit Q_SP2406_Widget(QWidget *parent = nullptr);
    ~Q_SP2406_Widget();

public slots:
    void init();

private slots:
    void on_checkBoxDL_CH0_clicked() { DL_ch = 0; updateDL(DL_ch); }

    void on_checkBoxDL_CH1_clicked() { DL_ch = 1; updateDL(DL_ch); }

    void on_checkBoxDL_CH2_clicked() { DL_ch = 2; updateDL(DL_ch); }

    void on_checkBoxDL_CH3_clicked() { DL_ch = 3; updateDL(DL_ch); }

    void on_checkBoxDL_CH4_clicked() { DL_ch = 4; updateDL(DL_ch); }

    void on_checkBoxDL_CH5_clicked() { DL_ch = 5; updateDL(DL_ch); }

    void on_checkBoxDL_CH6_clicked() { DL_ch = 6; updateDL(DL_ch); }

    void on_checkBoxDL_CH7_clicked() { DL_ch = 7; updateDL(DL_ch); }

    void on_lineEditDL_DDS0_textEdited(const QString &arg1);

    void on_lineEditDL_DDS1_textEdited(const QString &arg1);

    void on_comboBoxDL_Source_activated(int index);

    void on_comboBoxDL_CPRI_activated(int index)
    { SP2406->set_dl_cpri_map(DL_ch,uint8_t(index)); }

    void on_comboBoxDL_CPRI_sr_activated(int index);

    void on_lineEditDL_Source_CW_Freq_textEdited(const QString &arg1);

    void on_lineEditDL_Source_CW_Gain_textEdited(const QString &arg1)
    { SP2406->set_dl_src_dds0_gain(arg1.toDouble()); }

    void on_lineEditDL_SourceConstI_textEdited(const QString &arg1);

    void on_lineEditDL_SourceConstQ_textEdited(const QString &arg1);

    void on_checkBoxDL_LPF_clicked(bool checked)
    { SP2406->set_dl_lpf_en(DL_ch,checked); }

    void on_checkBoxDL_HBF0_clicked(bool checked)
    { SP2406->set_dl_hbf0_en(DL_ch,checked); }

    void on_checkBoxDL_HBF1_clicked(bool checked)
    { SP2406->set_dl_hbf1_en(DL_ch,checked); }

    void on_pushButtonSetDL_LPF_clicked();

    void on_checkBoxDL_CF_clicked(bool checked)
    { SP2406->set_dl_cf_en(checked); }

    void on_checkBoxDL_DUC_clicked(bool checked)
    { SP2406->set_dl_rc_en(checked); }

    void on_pushButtonSetDL_CF_clicked();

    void on_pushButtonJESD_Reset_clicked();

    void on_comboBoxDL_JESD_Source_0_activated(int index);

    void on_comboBoxDL_JESD_Source_1_activated(int index);

    void on_pushButtonSetDL_JESD_Source_clicked();

    void on_lineEditDL_JESD_ConstI_textEdited(const QString &arg1);

    void on_lineEditDL_JESD_ConstQ_textEdited(const QString &arg1);

    void on_pushButtonSetDL_JESD_Const_clicked();

    void on_comboBoxDL_JESD_DDS_Mode_activated(int index);

    void on_lineEditDL_JESD_DDS_Gain_textEdited(const QString &arg1);

    void on_pushButtonSetDL_JESD_DDS_Param0_clicked();

    void on_comboBoxDL_JESD_DDS_Period_activated(int index);

    void on_comboBoxDL_JESD_DDS_Interval_activated(int index);

    void on_pushButtonSetDL_JESD_DDS_Param1_clicked();

    void on_lineEditDL_JESD_DDS_Step_Freq_textEdited(const QString &arg1)
    { SP2406->set_dl_jesd_src_dds_coarse(arg1.toShort()); }

    void on_pushButtonSetDL_JESD_DDS_Step_Freq_clicked();

    void on_lineEditDL_JESD_DDS_Freq_textEdited(const QString &arg1);

    void on_pushButtonSetDL_JESD_DDS_Freq_clicked();

    void on_lineEditDL_Gain_textEdited(const QString &arg1)
    { SP2406->set_dl_pwr_comp(arg1.toDouble()); }

    void on_checkBoxUL_CF_clicked(bool checked)
    { SP2406->set_ul_cf_en(checked); }

    void on_pushButtonSetUL_CF_clicked();

    void on_checkBoxUL_LPF_clicked(bool checked)
    { SP2406->set_ul_lpf_en(UL_ch,checked); }

    void on_checkBoxUL_HBF1_clicked(bool checked)
    { SP2406->set_ul_hbf1_en(UL_ch,checked); }

    void on_checkBoxUL_HBF2_clicked(bool checked)
    { SP2406->set_ul_hbf2_en(UL_ch,checked); }

    void on_pushButtonSetUL_LPF_clicked();

    void on_lineEditUL_DDC0_textEdited(const QString &arg1);

    void on_lineEditUL_DDC1_textEdited(const QString &arg1);

    void on_pushButtonSetUL_DDC_clicked();

    void on_lineEditUL_Gain_textEdited(const QString &arg1)
    { SP2406->set_ul_pwr_comp(arg1.toDouble()); }

    void on_checkBoxUL_CH0_clicked() { UL_ch = 0; updateUL(UL_ch); }

    void on_checkBoxUL_CH1_clicked() { UL_ch = 1; updateUL(UL_ch); }

    void on_checkBoxUL_CH2_clicked() { UL_ch = 2; updateUL(UL_ch); }

    void on_checkBoxUL_CH3_clicked() { UL_ch = 3; updateUL(UL_ch); }

    void on_checkBoxUL_CH4_clicked() { UL_ch = 4; updateUL(UL_ch); }

    void on_checkBoxUL_CH5_clicked() { UL_ch = 5; updateUL(UL_ch); }

    void on_checkBoxUL_CH6_clicked() { UL_ch = 6; updateUL(UL_ch); }

    void on_checkBoxUL_CH7_clicked() { UL_ch = 7; updateUL(UL_ch); }

    void on_pushButtonGetUL_PwrManual_clicked();

    void on_pushButtonGetUL_PwrFPGA_clicked();

private:
    void updateDL(const quint8 ch);
    void updateUL(const quint8 ch);

private:
    quint8 DL_ch;
    quint8 UL_ch;
    Ui::Q_SP9500X_SP2406_Widget *ui;
};

} // namespace NS_SP9500X

#endif // Q_SP2406_Widget_H
