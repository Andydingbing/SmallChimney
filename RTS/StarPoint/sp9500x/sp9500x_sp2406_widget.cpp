#include "sp9500x_sp2406_widget.h"
#include "ui_sp9500x_sp2406.h"
#include "algo_math.hpp"
#include "utilities.hpp"
#include <QBitArray>

using namespace rd::ns_sp1403;
using namespace rd::ns_sp2406;
using namespace NS_SP9500X;

Q_SP2406_Widget::Q_SP2406_Widget(QWidget *parent) :
    Q_RD_Widget(parent),
    DL_ch(0),
    UL_ch(0),
    ui(new Ui::Q_SP9500X_SP2406_Widget)
{
    ui->setupUi(this);

    for (size_t i = 0;i < dl_src_t::_size();++i) {
        ui->comboBoxDL_Source->addItem(QString::fromUtf8(dl_src_t::_names()[i]));
    }

    for (size_t i = 0;i < 8;++i) {
        ui->comboBoxDL_CPRI->addItem(QString("%1").arg(i));
    }

    for (size_t i = 0;i < dl_cpri_sr_t::_size();++i) {
        ui->comboBoxDL_CPRI_sr->addItem(QString::fromUtf8(dl_cpri_sr_t::_names()[i]));
    }

    for (size_t i = 0;i < dl_jesd_src_t::_size();++i) {
        ui->comboBoxDL_JESD_Source_0->addItem(QString::fromUtf8(dl_jesd_src_t::_names()[i]));
        ui->comboBoxDL_JESD_Source_1->addItem(QString::fromUtf8(dl_jesd_src_t::_names()[i]));
    }

    for (size_t i = 0;i < dl_jesd_src_dds_mode_t::_size();++i) {
        ui->comboBoxDL_JESD_DDS_Mode->addItem(QString::fromUtf8(dl_jesd_src_dds_mode_t::_names()[i]));
    }

    for (size_t i = 0;i < dl_jesd_src_dds_auto_period_t::_size();++i) {
        ui->comboBoxDL_JESD_DDS_Period->addItem(QString::fromUtf8(dl_jesd_src_dds_auto_period_t::_names()[i]));
    }

    for (size_t i = 0;i < dl_jesd_src_dds_auto_interval_t::_size();++i) {
        ui->comboBoxDL_JESD_DDS_Interval->addItem(QString::fromUtf8(dl_jesd_src_dds_auto_interval_t::_names()[i]));
    }
}

Q_SP2406_Widget::~Q_SP2406_Widget()
{
    delete ui;
}

void Q_SP2406_Widget::init()
{
    ui->checkBoxDL_CH0->setChecked(true);
    updateDL(DL_ch);
    ui->checkBoxUL_CH0->setChecked(true);
    updateUL(UL_ch);
}

void Q_SP2406_Widget::on_comboBoxDL_Source_activated(int index)
{
    SP2406->set_dl_src(DL_ch,dl_src_t::_from_index(size_t(index)));
}

void Q_SP2406_Widget::on_lineEditDL_DDS0_textEdited(const QString &arg1)
{
    RD_TRY
    SP2406->set_dl_dds0(DL_ch,freq_string_to_double(arg1.toStdString()));
    RD_LOG_ALL_EXCEPTION
}

void Q_SP2406_Widget::on_lineEditDL_DDS1_textEdited(const QString &arg1)
{
    RD_TRY
    SP2406->set_dl_dds1(DL_ch,freq_string_to_double(arg1.toStdString()));
    RD_LOG_ALL_EXCEPTION
}

void Q_SP2406_Widget::updateDL(const quint8 ch)
{
    QList<QCheckBox *> chList;
    chList.push_back(ui->checkBoxDL_CH0);
    chList.push_back(ui->checkBoxDL_CH1);
    chList.push_back(ui->checkBoxDL_CH2);
    chList.push_back(ui->checkBoxDL_CH3);
    chList.push_back(ui->checkBoxDL_CH4);
    chList.push_back(ui->checkBoxDL_CH5);
    chList.push_back(ui->checkBoxDL_CH6);
    chList.push_back(ui->checkBoxDL_CH7);

    for (int i = 0;i < chList.size();++i) {
        if (i != ch) {
            chList[i]->setChecked(false);
        }
    }

    double DL_DDS0_Freq = 0.0;
    double DL_DDS1_Freq = 0.0;
    dl_src_t DLSrc = dl_src_t::CPRI;
    quint8 DL_CPRI = 0;
    dl_cpri_sr_t DL_CPRI_SR = dl_cpri_sr_t::_12288;
    double DLSrcDDS0_Freq = 0.0;
    double DLSrcDDS0_Gain = 0.0;
    qint16 DLSrcConstI = 0;
    qint16 DLSrcConstQ = 0;
    std::vector<int16_t> DL_LPF_Coef;
    bool DL_LPF_En = true;
    bool DL_HBF0_En = true;
    bool DL_HBF1_En = true;
    std::vector<int16_t> DL_CF_Coef_Real;
    std::vector<int16_t> DL_CF_Coef_Imag;
    bool DL_CF_En = true;
    bool DL_DUC_En = true;
    dl_jesd_src_t DL_JESD_Src0 = dl_jesd_src_t::DUC;
    dl_jesd_src_t DL_JESD_Src1 = dl_jesd_src_t::DUC;
    qint16 DL_JESD_SrcConstI = 0;
    qint16 DL_JESD_SrcConstQ = 0;
    dl_jesd_src_dds_mode_t DL_JESD_DDS_Mode = dl_jesd_src_dds_mode_t::Fine;
    dl_jesd_src_dds_auto_period_t DL_JESD_DDS_AutoPeriod = dl_jesd_src_dds_auto_period_t::_1ms;
    dl_jesd_src_dds_auto_interval_t DL_JESD_DDS_AutoInterval = dl_jesd_src_dds_auto_interval_t::_1MHz;
    qint16 DL_JESD_DDS_Coarse_Freq_MHz = 0;
    double DL_JESD_DDS_Fine_Freq = 0.0;
    double DL_JESD_DDS_Gain = 0.0;

    SP2406->get_dl_dds0(ch,DL_DDS0_Freq);
    SP2406->get_dl_dds1(ch,DL_DDS1_Freq);
    SP2406->get_dl_src(ch,DLSrc);
    SP2406->get_dl_cpri_map(ch,DL_CPRI);
    SP2406->get_dl_cpri_sr(ch,DL_CPRI_SR);
    SP2406->get_dl_src_dds0_freq(DLSrcDDS0_Freq);
    SP2406->get_dl_src_dds0_gain(DLSrcDDS0_Gain);
    SP2406->get_dl_src_const(DLSrcConstI,DLSrcConstQ);
    SP2406->get_dl_lpf_coef(ch,DL_LPF_Coef);
    SP2406->get_dl_lpf_en(ch,DL_LPF_En);
    SP2406->get_dl_hbf0_en(ch,DL_HBF0_En);
    SP2406->get_dl_hbf1_en(ch,DL_HBF1_En);
    SP2406->get_dl_cf_coef(DL_CF_Coef_Real,DL_CF_Coef_Imag);
    SP2406->get_dl_cf_en(DL_CF_En);
    SP2406->get_dl_rc_en(DL_DUC_En);
    SP2406->get_dl_jesd_src(TX0,DL_JESD_Src0);
    SP2406->get_dl_jesd_src(TX1,DL_JESD_Src1);
    SP2406->get_dl_jesd_src_const(DL_JESD_SrcConstI,DL_JESD_SrcConstQ);
    SP2406->get_dl_jesd_src_dds(DL_JESD_DDS_Mode);
    SP2406->get_dl_jesd_src_dds(DL_JESD_DDS_AutoPeriod);
    SP2406->get_dl_jesd_src_dds(DL_JESD_DDS_AutoInterval);
    SP2406->get_dl_jesd_src_dds_coarse(DL_JESD_DDS_Coarse_Freq_MHz);
    SP2406->get_dl_jesd_src_dds_fine(DL_JESD_DDS_Fine_Freq);
    SP2406->get_dl_jesd_src_dds_gain(DL_JESD_DDS_Gain);

    ui->lineEditDL_DDS0->setText(freqStringFrom(DL_DDS0_Freq));
    ui->lineEditDL_DDS1->setText(freqStringFrom(DL_DDS1_Freq));
    ui->comboBoxDL_Source->setCurrentIndex(int(DLSrc._to_index()));
    ui->comboBoxDL_CPRI->setCurrentIndex(DL_CPRI);
    ui->comboBoxDL_CPRI_sr->setCurrentIndex(int(DL_CPRI_SR._to_index()));
    ui->lineEditDL_Source_CW_Freq->setText(freqStringFrom(DLSrcDDS0_Freq));
    ui->lineEditDL_Source_CW_Gain->setText(QString("%1").arg(DLSrcDDS0_Gain));
    ui->lineEditDL_SourceConstI->setText(QString("%1").arg(DLSrcConstI));
    ui->lineEditDL_SourceConstQ->setText(QString("%1").arg(DLSrcConstQ));
    ui->textEditDL_LPF_Coef->setText(string_of<QString>(DL_LPF_Coef,","));
    ui->checkBoxDL_LPF->setChecked(DL_LPF_En);
    ui->checkBoxDL_HBF0->setChecked(DL_HBF0_En);
    ui->checkBoxDL_HBF1->setChecked(DL_HBF1_En);
    ui->textEditDL_CF_Coef_Real->setText(string_of<QString>(DL_CF_Coef_Real,","));
    ui->textEditDL_CF_Coef_Imag->setText(string_of<QString>(DL_CF_Coef_Imag,","));
    ui->checkBoxDL_CF->setChecked(DL_CF_En);
    ui->checkBoxDL_DUC->setChecked(DL_DUC_En);
    ui->comboBoxDL_JESD_Source_0->setCurrentIndex(int(DL_JESD_Src0._to_index()));
    ui->comboBoxDL_JESD_Source_1->setCurrentIndex(int(DL_JESD_Src1._to_index()));
    ui->lineEditDL_JESD_ConstI->setText(QString("%1").arg(DL_JESD_SrcConstI));
    ui->lineEditDL_JESD_ConstQ->setText(QString("%1").arg(DL_JESD_SrcConstQ));
    ui->comboBoxDL_JESD_DDS_Mode->setCurrentIndex(int(DL_JESD_DDS_Mode._to_index()));
    ui->comboBoxDL_JESD_DDS_Period->setCurrentIndex(int(DL_JESD_DDS_AutoPeriod._to_index()));
    ui->comboBoxDL_JESD_DDS_Interval->setCurrentIndex(int(DL_JESD_DDS_AutoInterval._to_index()));
    ui->lineEditDL_JESD_DDS_Step_Freq->setText(QString("%1").arg(DL_JESD_DDS_Coarse_Freq_MHz));
    ui->lineEditDL_JESD_DDS_Freq->setText(freqStringFrom(DL_JESD_DDS_Fine_Freq));
    ui->lineEditDL_JESD_DDS_Gain->setText(QString("%1").arg(DL_JESD_DDS_Gain));
}

void Q_SP2406_Widget::updateUL(const quint8 ch)
{
    QList<QCheckBox *> chList;
    chList.push_back(ui->checkBoxUL_CH0);
    chList.push_back(ui->checkBoxUL_CH1);
    chList.push_back(ui->checkBoxUL_CH2);
    chList.push_back(ui->checkBoxUL_CH3);
    chList.push_back(ui->checkBoxUL_CH4);
    chList.push_back(ui->checkBoxUL_CH5);
    chList.push_back(ui->checkBoxUL_CH6);
    chList.push_back(ui->checkBoxUL_CH7);

    for (int i = 0;i < chList.size();++i) {
        if (i != ch) {
            chList[i]->setChecked(false);
        }
    }

    bool UL_CF_En = true;
    std::vector<int16_t> UL_CF_CoefReal;
    std::vector<int16_t> UL_CF_CoefImag;
    bool UL_LPF_En = true;
    bool UL_HBF1_En = true;
    bool UL_HBF2_En = true;
    std::vector<int16_t> UL_LPF_Coef;
    double UL_DDC0_Freq = 0.0;
    double UL_DDC1_Freq = 0.0;
    quint8 UL_CPRI = 0;

    SP2406->get_ul_cf_coef(UL_CF_CoefReal,UL_CF_CoefImag);
    SP2406->get_ul_cf_en(UL_CF_En);
    SP2406->get_ul_lpf_en(ch,UL_LPF_En);
    SP2406->get_ul_hbf1_en(ch,UL_HBF1_En);
    SP2406->get_ul_hbf2_en(ch,UL_HBF2_En);
    SP2406->get_ul_lpf_coef(ch,UL_LPF_Coef);
    SP2406->get_ul_ddc0(ch,UL_DDC0_Freq);
    SP2406->get_ul_ddc1(ch,UL_DDC1_Freq);
    SP2406->get_ul_cpri_map(ch,UL_CPRI);

    ui->checkBoxUL_CF->setChecked(UL_CF_En);
    ui->textEditUL_CF_CoefReal->setText(string_of<QString>(UL_CF_CoefReal,","));
    ui->textEditUL_CF_CoefImag->setText(string_of<QString>(UL_CF_CoefImag,","));
    ui->checkBoxUL_LPF->setChecked(UL_LPF_En);
    ui->checkBoxUL_HBF1->setChecked(UL_HBF1_En);
    ui->checkBoxUL_HBF2->setChecked(UL_HBF2_En);
    ui->textEditUL_LPF_Coef->setText(string_of<QString>(UL_LPF_Coef,","));
    ui->lineEditUL_DDC0->setText(freqStringFrom(UL_DDC0_Freq));
    ui->lineEditUL_DDC1->setText(freqStringFrom(UL_DDC1_Freq));
    ui->lineEditUL_PwrManualSamples->setText("2048");
    ui->lineEditUL_PwrFPGASamples->setText("2048");
}

void Q_SP2406_Widget::on_comboBoxDL_CPRI_sr_activated(int index)
{
    SP2406->set_dl_cpri_sr(DL_ch,dl_cpri_sr_t::_from_index(size_t(index)));
}

void Q_SP2406_Widget::on_lineEditDL_Source_CW_Freq_textEdited(const QString &arg1)
{
    RD_TRY
    SP2406->set_dl_src_dds0_freq(freq_string_to_double(arg1.toStdString()));
    RD_LOG_ALL_EXCEPTION
}

void Q_SP2406_Widget::on_lineEditDL_SourceConstI_textEdited(const QString &arg1)
{
    SP2406->set_dl_src_const(arg1.toShort(),ui->lineEditDL_SourceConstQ->text().toShort());
}

void Q_SP2406_Widget::on_lineEditDL_SourceConstQ_textEdited(const QString &arg1)
{
    SP2406->set_dl_src_const(ui->lineEditDL_SourceConstI->text().toShort(),arg1.toShort());
}

void Q_SP2406_Widget::on_pushButtonSetDL_LPF_clicked()
{
    std::vector<int16_t> coef;

    string_to_container<int16_t>(ui->textEditDL_LPF_Coef->toPlainText().toStdString(),',',coef);
    SP2406->set_dl_lpf_coef(DL_ch,coef);
}

void Q_SP2406_Widget::on_pushButtonSetDL_CF_clicked()
{
    std::vector<int16_t> real;
    std::vector<int16_t> imag;

    string_to_container<int16_t>(ui->textEditDL_CF_Coef_Real->toPlainText().toStdString(),',',real);
    string_to_container<int16_t>(ui->textEditDL_CF_Coef_Imag->toPlainText().toStdString(),',',imag);
    SP2406->set_dl_cf_coef(real,imag);
}

void Q_SP2406_Widget::on_pushButtonJESD_Reset_clicked()
{
    SP2406->jesd_reset();
}

void Q_SP2406_Widget::on_comboBoxDL_JESD_Source_0_activated(int index)
{
    SP2406->set_dl_jesd_src(ns_sp1403::TX0,dl_jesd_src_t::_from_index(size_t(index)));
}

void Q_SP2406_Widget::on_comboBoxDL_JESD_Source_1_activated(int index)
{
    SP2406->set_dl_jesd_src(ns_sp1403::TX1,dl_jesd_src_t::_from_index(size_t(index)));
}

void Q_SP2406_Widget::on_pushButtonSetDL_JESD_Source_clicked()
{
    on_comboBoxDL_JESD_Source_0_activated(ui->comboBoxDL_JESD_Source_0->currentIndex());
    on_comboBoxDL_JESD_Source_1_activated(ui->comboBoxDL_JESD_Source_1->currentIndex());
}

void Q_SP2406_Widget::on_lineEditDL_JESD_ConstI_textEdited(const QString &arg1)
{
    qint16 i = arg1.toShort();
    qint16 q = ui->lineEditDL_JESD_ConstQ->text().toShort();
    SP2406->set_dl_jesd_src_const(i,q);
}

void Q_SP2406_Widget::on_lineEditDL_JESD_ConstQ_textEdited(const QString &arg1)
{
    qint16 i = ui->lineEditDL_JESD_ConstI->text().toShort();
    qint16 q = arg1.toShort();
    SP2406->set_dl_jesd_src_const(i,q);
}

void Q_SP2406_Widget::on_pushButtonSetDL_JESD_Const_clicked()
{
    qint16 i = ui->lineEditDL_JESD_ConstI->text().toShort();
    qint16 q = ui->lineEditDL_JESD_ConstQ->text().toShort();
    SP2406->set_dl_jesd_src_const(i,q);
}

void Q_SP2406_Widget::on_comboBoxDL_JESD_DDS_Mode_activated(int index)
{
    SP2406->set_dl_jesd_src_dds(dl_jesd_src_dds_mode_t::_from_index(size_t(index)));
}

void Q_SP2406_Widget::on_lineEditDL_JESD_DDS_Gain_textEdited(const QString &arg1)
{
    SP2406->set_dl_jesd_src_dds_gain(arg1.toDouble());
}

void Q_SP2406_Widget::on_pushButtonSetDL_JESD_DDS_Param0_clicked()
{
    on_comboBoxDL_JESD_DDS_Mode_activated(ui->comboBoxDL_JESD_DDS_Mode->currentIndex());
    on_lineEditDL_JESD_DDS_Gain_textEdited(ui->lineEditDL_JESD_DDS_Gain->text());
}

void Q_SP2406_Widget::on_comboBoxDL_JESD_DDS_Period_activated(int index)
{
    SP2406->set_dl_jesd_src_dds(dl_jesd_src_dds_auto_period_t::_from_index(size_t(index)));
}

void Q_SP2406_Widget::on_comboBoxDL_JESD_DDS_Interval_activated(int index)
{
    SP2406->set_dl_jesd_src_dds(dl_jesd_src_dds_auto_interval_t::_from_index(size_t(index)));
}

void Q_SP2406_Widget::on_pushButtonSetDL_JESD_DDS_Param1_clicked()
{
    on_comboBoxDL_JESD_DDS_Period_activated(ui->comboBoxDL_JESD_DDS_Period->currentIndex());
    on_comboBoxDL_JESD_DDS_Interval_activated(ui->comboBoxDL_JESD_DDS_Interval->currentIndex());
}

void Q_SP2406_Widget::on_pushButtonSetDL_JESD_DDS_Step_Freq_clicked()
{
    on_lineEditDL_JESD_DDS_Step_Freq_textEdited(ui->lineEditDL_JESD_DDS_Step_Freq->text());
}

void Q_SP2406_Widget::on_lineEditDL_JESD_DDS_Freq_textEdited(const QString &arg1)
{
    SP2406->set_dl_jesd_src_dds_fine(freq_string_to_double(arg1.toStdString()));
}

void Q_SP2406_Widget::on_pushButtonSetDL_JESD_DDS_Freq_clicked()
{
    on_lineEditDL_JESD_DDS_Freq_textEdited(ui->lineEditDL_JESD_DDS_Freq->text());
}

void Q_SP2406_Widget::on_pushButtonSetUL_CF_clicked()
{
    std::vector<int16_t> real;
    std::vector<int16_t> imag;

    string_to_container<int16_t>(ui->textEditUL_CF_CoefReal->toPlainText().toStdString(),',',real);
    string_to_container<int16_t>(ui->textEditUL_CF_CoefImag->toPlainText().toStdString(),',',imag);
    SP2406->set_ul_cf_coef(real,imag);
}

void Q_SP2406_Widget::on_pushButtonSetUL_LPF_clicked()
{
    std::vector<int16_t> coef;

    string_to_container<int16_t>(ui->textEditUL_LPF_Coef->toPlainText().toStdString(),',',coef);
    SP2406->set_ul_lpf_coef(UL_ch,coef);
}

void Q_SP2406_Widget::on_lineEditUL_DDC0_textEdited(const QString &arg1)
{
    RD_TRY
    SP2406->set_ul_ddc0(UL_ch,freq_string_to_double(arg1.toStdString()));
    RD_LOG_ALL_EXCEPTION
}

void Q_SP2406_Widget::on_lineEditUL_DDC1_textEdited(const QString &arg1)
{
    RD_TRY
    SP2406->set_ul_ddc1(UL_ch,freq_string_to_double(arg1.toStdString()));
    RD_LOG_ALL_EXCEPTION
}

void Q_SP2406_Widget::on_pushButtonSetUL_DDC_clicked()
{
    on_lineEditUL_DDC0_textEdited(ui->lineEditUL_DDC0->text());
    on_lineEditUL_DDC1_textEdited(ui->lineEditUL_DDC1->text());
}

void Q_SP2406_Widget::on_pushButtonGetUL_PwrManual_clicked()
{
    double pwr = 0.0;
    SP2406->get_ul_pwr_manual(pwr,ui->lineEditUL_PwrManualSamples->text().toULong());
    ui->lineEditUL_PwrManual->setText(QString("%1").arg(pwr));
}

void Q_SP2406_Widget::on_pushButtonGetUL_PwrFPGA_clicked()
{
    double pwr = 0.0;
    SP2406->get_ul_pwr_fpga(pwr,ui->lineEditUL_PwrFPGASamples->text().toULong());
    ui->lineEditUL_PwrFPGA->setText(QString("%1").arg(pwr));
}
