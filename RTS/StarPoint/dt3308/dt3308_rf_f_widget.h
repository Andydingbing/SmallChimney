#ifndef Q_DT3308_RF_F_Widget_H
#define Q_DT3308_RF_F_Widget_H

#include "global.h"
#include "hmc832_dlg.h"

namespace Ui {
class Q_DT3308_RF_F_Widget;
}

class Q_DT3308_RF_F_Widget : public Q_RD_Widget
{
    Q_OBJECT

public:
    explicit Q_DT3308_RF_F_Widget(QWidget *parent = nullptr);
    ~Q_DT3308_RF_F_Widget();

private slots:
    void on_pushButtonSetTxFreq_clicked();

    void on_lineEditTxFreq_textEdited(const QString &arg1);

    void on_pushButtonSetTxFilter_clicked();

    void on_pushButtonSetTxAtt0_clicked();

    void on_pushButtonSetTxAtt1_clicked();

    void on_pushButtonSetTxAtt2_clicked();

    void on_pushButtonSetTxAtt3_clicked();

    void on_pushButtonSetTxAtt4_clicked();

    void on_pushButtonSetTxAtt5_clicked();

    void on_comboBoxIOMode0_activated(int index);

    void on_comboBoxIOMode1_activated(int index);

    void on_comboBoxIOMode2_activated(int index);

    void on_comboBoxIOMode3_activated(int index);

    void on_comboBoxIOMode4_activated(int index);

    void on_comboBoxIOMode5_activated(int index);

    void on_comboBoxIOMode6_activated(int index);

    void on_comboBoxIOMode7_activated(int index);

    void on_comboBoxPortAtt0_activated(int index);

    void on_comboBoxPortAtt1_activated(int index);

    void on_comboBoxPortAtt2_activated(int index);

    void on_comboBoxPortAtt3_activated(int index);

    void on_comboBoxPortAtt4_activated(int index);

    void on_comboBoxPortAtt5_activated(int index);

    void on_comboBoxPortAtt6_activated(int index);

    void on_comboBoxPortAtt7_activated(int index);

    void on_pushButtonSetRxFreq_clicked();

    void on_pushButtonSetRxLNAAttSw0_clicked();

    void on_pushButtonSetRxLNAAttSw1_clicked();

    void on_pushButtonSetRxAtt0_clicked();

    void on_pushButtonSetRxAtt1_clicked();

    void on_pushButtonSetIFFilterSw_clicked();

    void on_pushButtonSetTxHMC832_clicked();

    void on_pushButtonTxHMC832Adv_clicked();

    void on_pushButtonSetTxHMC833_clicked();

    void on_lineEditTxHMC833Freq_textEdited(const QString &text);

    void on_pushButtonTxHMC833Adv_clicked();

    void on_pushButtonSetTxHMC833Filter_clicked();

    void on_checkBoxTxEn_clicked(bool checked);

    void on_checkBoxRxEn_clicked(bool checked);

    void on_checkBoxTxHMC832_clicked(bool checked);

    void on_checkBoxTxMainPA0_0_3_stateChanged(int arg1);

    void on_checkBoxTxMainPA1_0_3_stateChanged(int arg1);

    void on_checkBoxTxMainPA0_4_7_stateChanged(int arg1);

    void on_checkBoxTxMainPA1_4_7_stateChanged(int arg1);

private:
    Q_HMC832_Dlg *dlg_TX_HMC832;
    Q_HMC832_Dlg *dlg_TX_HMC833;
    Ui::Q_DT3308_RF_F_Widget *ui;
};

#endif // Q_DT3308_RF_F_Widget_H
