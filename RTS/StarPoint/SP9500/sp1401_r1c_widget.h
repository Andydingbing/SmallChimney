#ifndef STARPOINT_SP9500_SP1401_R1C_WIDGET_H
#define STARPOINT_SP9500_SP1401_R1C_WIDGET_H

#include "global.h"

class Q_SP1401_R1C_Widget : public Q_Widget
{
    Q_OBJECT

public:
    Q_SP1401_R1C_Widget(QWidget *parent = nullptr);
    ~Q_SP1401_R1C_Widget();

private slots:
    void on_lineEditTXFreq_returnPressed();

    void on_comboBoxTXAtt0_activated(int index) { SP1401_R1C->set_tx_att0(index * 0.5); }

    void on_comboBoxTXAtt1_activated(int index) { SP1401_R1C->set_tx_att1(index * 0.5); }

    void on_comboBoxTXAtt2_activated(int index) { SP1401_R1C->set_tx_att2(index * 0.5); }

    void on_comboBoxTXAtt3_activated(int index) { SP1401_R1C->set_tx_att3(index * 0.5); }

    void on_comboBoxIOMode_activated(int index) { SP1401_R1C->set_io_mode(io_mode_t::_from_index(index)); }

    void on_lineEditDGain_textEdited(const QString &arg1);

    void on_pushButtonSetTXAll_clicked();

    void on_lineEditRXFreq_returnPressed();

    void on_comboBoxRXLNAAttSw_activated(int index) { SP1401_R1C->set_rx_lna_att_sw(index); }

    void on_comboBoxRXAtt019Sw_activated(int index) { SP1401_R1C->set_rx_att_019_sw(index); }

    void on_comboBoxIFFilterSw_activated(int index) { SP1401_R1C->set_rx_if_filter_sw(index); }

    void on_comboBoxRXAtt1_activated(int index) { SP1401_R1C->set_rx_att1(index * 0.5); }

    void on_comboBoxRXAtt2_activated(int index) { SP1401_R1C->set_rx_att2(index * 0.5); }

    void on_comboBoxRXAtt3_activated(int index) { SP1401_R1C->set_rx_att3(index * 0.5); }

    void on_pushButtonSetRXAll_clicked();

    void on_pushButtonGetTempAll_clicked();

    void on_pushButtonGetDetAll_clicked();

    void on_pushButtonSetSN_clicked();

    void on_pushButtonGetSN_clicked();

private:
    Ui::Q_SP1401_R1C_Widget *ui;
};

#endif
