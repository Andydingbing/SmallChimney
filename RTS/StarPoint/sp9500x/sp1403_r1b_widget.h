#ifndef Q_SP1403_R1B_WIDGET_H
#define Q_SP1403_R1B_WIDGET_H

#include "global.h"

class Q_SP1403_R1B_Widget : public Q_RD_Widget
{
    Q_OBJECT

public:
    explicit Q_SP1403_R1B_Widget(QWidget *parent = nullptr);
    ~Q_SP1403_R1B_Widget();

public slots:
    void init();

private slots:
    void on_lineEditTXFreq_textEdited(const QString &arg1);

    void on_comboBoxTX0IOMode_activated(int index)
    { SP1403_R1B->set_io_mode(ns_sp1403::TX0,io_mode_t(index)); updateIOMode(); }

    void on_comboBoxTX1IOMode_activated(int index)
    { SP1403_R1B->set_io_mode(ns_sp1403::TX1,io_mode_t(index)); updateIOMode(); }

    void on_comboBoxTX0LED_activated(int index)
    { SP1403_R1B->set_led(ns_sp1403::TX0,ns_sp1403::led_t::_from_index(size_t(index))); }

    void on_comboBoxTX1LED_activated(int index)
    { SP1403_R1B->set_led(ns_sp1403::TX1,ns_sp1403::led_t::_from_index(size_t(index))); }

    void on_comboBoxTX0Att0_activated(int index)
    { SP1403_R1B->set_att(sp1403_r1b::att_t::TX0_ATT0,index / 2.0); }

    void on_comboBoxTX1Att0_activated(int index)
    { SP1403_R1B->set_att(sp1403_r1b::att_t::TX1_ATT0,index / 2.0); }

    void on_comboBoxTX0Att1_activated(int index)
    { SP1403_R1B->set_att(sp1403_r1b::att_t::TX0_ATT1,index / 2.0); }

    void on_comboBoxTX1Att1_activated(int index)
    { SP1403_R1B->set_att(sp1403_r1b::att_t::TX1_ATT1,index / 2.0); }

    void on_comboBoxTX0Att2_activated(int index)
    { SP1403_R1B->set_att(sp1403_r1b::att_t::TX0_ATT2,index / 2.0); }

    void on_comboBoxTX1Att2_activated(int index)
    { SP1403_R1B->set_att(sp1403_r1b::att_t::TX1_ATT2,index / 2.0); }

    void on_comboBoxTX0Att3_activated(int index)
    { SP1403_R1B->set_att(sp1403_r1b::att_t::TX0_ATT3,index / 2.0); }

    void on_comboBoxTX1Att3_activated(int index)
    { SP1403_R1B->set_att(sp1403_r1b::att_t::TX1_ATT3,index / 2.0); }

    void on_lineEditTXLMX2594_0_Freq_textEdited(const QString &arg1);

    void on_pushButtonTXLMX2594_0_Adv_clicked();

    void on_lineEditTXLMX2594_1_Freq_textEdited(const QString &arg1);

    void on_pushButtonTXLMX2594_1_Adv_clicked();

    void on_comboBoxTX0Sw1_activated(int index)
    { SP1403_R1B->set_tx0_sw1(sp1403_r1b::tx_sw1_t::_from_index(index)); }

    void on_comboBoxTX1Sw1_activated(int index)
    { SP1403_R1B->set_tx1_sw1(sp1403_r1b::tx_sw1_t::_from_index(index)); }

    void on_comboBoxTX0Sw2_activated(int index)
    { SP1403_R1B->set_tx0_sw2(sp1403_r1b::tx_sw2_t::_from_index(index)); }

    void on_comboBoxTX1Sw2_activated(int index)
    { SP1403_R1B->set_tx1_sw2(sp1403_r1b::tx_sw2_t::_from_index(index)); }

    void on_comboBoxTX0Sw3_activated(int index)
    { SP1403_R1B->set_tx0_sw3(sp1403_r1b::tx_sw3_t::_from_index(index)); }

    void on_comboBoxTX1Sw3_activated(int index)
    { SP1403_R1B->set_tx1_sw3(sp1403_r1b::tx_sw3_t::_from_index(index)); }

    void on_comboBoxTX0Sw4_activated(int index)
    { SP1403_R1B->set_tx0_sw4(sp1403_r1b::tx_sw4_t::_from_index(index)); }

    void on_comboBoxTX1Sw4_activated(int index)
    { SP1403_R1B->set_tx1_sw4(sp1403_r1b::tx_sw4_t::_from_index(index)); }

    void on_comboBoxTX0Sw5_activated(int index)
    { SP1403_R1B->set_tx0_sw5(sp1403_r1b::tx0_sw5_t::_from_index(index)); }

    void on_comboBoxTX1Sw5_activated(int index)
    { SP1403_R1B->set_tx1_sw5(sp1403_r1b::tx1_sw5_t::_from_index(index)); }

    void on_comboBoxTX0Sw6_activated(int index)
    { SP1403_R1B->set_tx0_sw6(sp1403_r1b::tx0_sw6_t::_from_index(index)); }

    void on_comboBoxTX1Sw6_activated(int index)
    { SP1403_R1B->set_tx1_sw6(sp1403_r1b::tx1_sw6_t::_from_index(index)); }

    void on_comboBoxTX0Sw7_activated(int index)
    { SP1403_R1B->set_tx0_sw7(sp1403_r1b::tx0_sw7_t::_from_index(index)); }

    void on_comboBoxTX0Sw8_activated(int index)
    { SP1403_R1B->set_tx0_sw8(sp1403_r1b::tx0_sw8_t::_from_index(index)); }

    void on_comboBoxTX0Sw9_activated(int index)
    { SP1403_R1B->set_tx0_sw9(sp1403_r1b::tx0_sw9_t::_from_index(index)); }

    void on_comboBoxTX0Sw10_activated(int index)
    { SP1403_R1B->set_tx0_sw10(sp1403_r1b::tx0_sw10_t::_from_index(index)); }

    void on_comboBoxTX0Sw11_activated(int index)
    { SP1403_R1B->set_tx0_sw11(sp1403_r1b::tx0_sw11_t::_from_index(index)); }

    void on_comboBoxTX0Sw12_activated(int index)
    { SP1403_R1B->set_tx0_sw12(sp1403_r1b::tx0_sw12_t::_from_index(index)); }

    void on_lineEditRXFreq_textEdited(const QString &arg1);

    void on_comboBoxRXLNAAttSw_activated(int index);

    void on_comboBoxRXAtt0_activated(int index);

    void on_comboBoxRXAtt1_activated(int index);

    void on_comboBoxBWSw_activated(int index);

    void on_lineEditRXLMX2594_0_Freq_textEdited(const QString &arg1);

    void on_pushButtonRXLMX2594_0_Adv_clicked();

    void on_comboBoxDet_activated(int index);

    void on_pushButtonGetDet_clicked();

    void on_pushButtonGetSN_clicked();

    void on_comboBoxRXSw1_activated(int index)
    { SP1403_R1B->set_rx_sw1(sp1403_r1b::rx_sw1_t::_from_index(index)); }

    void on_comboBoxRXSw2_activated(int index)
    { SP1403_R1B->set_rx_sw2(sp1403_r1b::rx_sw2_t::_from_index(index)); }

    void on_comboBoxRXSw3_activated(int index)
    { SP1403_R1B->set_rx_sw3(sp1403_r1b::rx_sw3_t::_from_index(index)); }

    void on_comboBoxRXSw7_activated(int index)
    { SP1403_R1B->set_rx_sw7(sp1403_r1b::rx_sw7_t::_from_index(index)); }

    void on_pushButtonGetTemp0_clicked();

    void on_pushButtonGetTemp1_clicked();

    void on_pushButtonS6_Write_clicked();

    void on_pushButtonS6_Read_clicked();

private:
    void updateIOMode();
    void updateAtt();
    void updateTXSw();
    void updateTXLO();
    void updateRXSw();
    void updateRXLO();

private:
    Ui::Q_SP1403_R1B_Widget *ui;
    Q_LMX2594_Dlg *dlg_TX_LMX2594_0;
    Q_LMX2594_Dlg *dlg_TX_LMX2594_1;
    Q_LMX2594_Dlg *dlg_RX_LMX2594_0;
};

#endif // Q_SP1403_R1A_WIDGET_H
