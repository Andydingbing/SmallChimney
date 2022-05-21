#ifndef STARPOINT_SP9500_SP1401_R1A_ADV_WIDGET_H
#define STARPOINT_SP9500_SP1401_R1A_ADV_WIDGET_H

#include "global.h"
#include "adf5355_dlg.h"

class Q_SP1401_R1A_Adv_Widget : public Q_Widget
{
    Q_OBJECT

public:
    Q_SP1401_R1A_Adv_Widget(QWidget *parent = nullptr);
    ~Q_SP1401_R1A_Adv_Widget();

private slots:
    void on_comboBoxTxIOSw_activated(int index);

    void on_comboBoxTxAtt3_activated(int index);

    void on_comboBoxTxPAAttSw_activated(int index);

    void on_comboBoxTxAtt2_activated(int index);

    void on_comboBoxTxAtt1_activated(int index);

    void on_comboBoxTxBandSw_activated(int index);

    void on_pushButtonModEn_clicked();

    void on_pushButtonPowerOn_clicked();

    void on_comboBoxIOMode_activated(int index);

    void on_checkBoxTxLED_clicked();

    void on_checkBoxRxLED_clicked();

    void on_pushButtonSetTxFreq_clicked();

    void on_pushButtonSetRxFreq_clicked();

    void on_pushButtonGetTxTemp_clicked();

    void on_pushButtonGetRxTemp_clicked();

    void on_pushButtonGetTxDet_clicked();

    void on_pushButtonGetRxDet_clicked();

    void on_comboBoxRxIOSw1_activated(int index);

    void on_comboBoxRxIOSw2_activated(int index);

    void on_comboBoxRxIOSw_activated(int index);

    void on_comboBoxRxLNAAttSw_activated(int index);

    void on_comboBoxRxAtt1_activated(int index);

    void on_comboBoxRxAtt2_activated(int index);

    void on_comboBoxIFFilterSw_activated(int index);

    void on_pushButtonAdvTxLO1_clicked();

    void on_pushButtonAdvTxLO2_clicked();

    void on_pushButtonAdvRxLO1_clicked();

    void on_pushButtonAdvRxLO2_clicked();

    void on_pushButtonSetTxLO1_clicked();

    void on_pushButtonSetTxLO2_clicked();

    void on_pushButtonSetRxLO1_clicked();

    void on_pushButtonSetRxLO2_clicked();

private:
    Q_ADF5355_Dlg *dlg_ADF5355[4];
    Ui::Q_SP1401_R1A_Adv_Widget *ui;
};

#endif
