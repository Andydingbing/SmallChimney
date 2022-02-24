#ifndef STARPOINT_SP9500_SP1401_R1F_WIDGET_H
#define STARPOINT_SP9500_SP1401_R1F_WIDGET_H

#include "starpoint_sp9500_global.h"

class Q_StarPoint_SP9500_SP1401_R1F_Widget : public Q_Widget
{
    Q_OBJECT

public:
    Q_StarPoint_SP9500_SP1401_R1F_Widget(QWidget *parent = nullptr);
    ~Q_StarPoint_SP9500_SP1401_R1F_Widget();

private slots:
    void on_lineEditTXFreq_returnPressed();

    void on_comboBoxTXAtt0_activated(int index) { SP1401_R1F->set_tx_att0(index * 0.5); }

    void on_comboBoxTXAtt1_activated(int index) { SP1401_R1F->set_tx_att1(index * 0.5); }

    void on_comboBoxTXAtt2_activated(int index) { SP1401_R1F->set_tx_att2(index * 0.5); }

    void on_comboBoxIOMode_activated(int index);

    void on_lineEditDGain_textChanged(const QString &arg1);

    void on_pushButtonSetTXAll_clicked();

    void on_lineEditRXFreq_returnPressed();

    void on_comboBoxRXLNAAttSw_activated(int index);

    void on_comboBoxRXAtt019Sw_activated(int index);

    void on_comboBoxRXAtt1_activated(int index);

    void on_comboBoxIFFilterSw_activated(int index);

    void on_comboBoxRXAtt2_activated(int index);

    void on_pushButtonSetRXAll_clicked();

    void on_pushButtonGetTempDetAll_clicked();

    void on_pushButtonSetSN_clicked();

    void on_pushButtonGetSN_clicked();

private:
    Ui::Q_StarPoint_SP9500_SP1401_R1F_Widget *ui;
};

#endif
