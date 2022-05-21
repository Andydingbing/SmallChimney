#ifndef STARPOINT_SP9500_SP1401_R1A_WIDGET_H
#define STARPOINT_SP9500_SP1401_R1A_WIDGET_H

#include "global.h"

class Q_SP1401_R1A_Widget : public Q_Widget
{
    Q_OBJECT

public:
    Q_SP1401_R1A_Widget(QWidget *parent = nullptr);
    ~Q_SP1401_R1A_Widget();

private slots:
    void on_pushButtonSetTxFreq_clicked();

    void on_pushButtonSetTxAtt1_clicked();

    void on_pushButtonSetTxAtt2_clicked();

    void on_pushButtonSetTxPAAttSw_clicked();

    void on_pushButtonSetTxAtt3_clicked();

    void on_pushButtonGetTxDet_clicked();

    void on_pushButtonGetTxTemp_clicked();

    void on_pushButtonSetIOMode_clicked();

    void on_pushButtonSyncSetTxGain_clicked();

    void on_pushButtonSetRxFreq_clicked();

    void on_pushButtonSetRxLNAAttSw_clicked();

    void on_pushButtonSetRxAtt1_clicked();

    void on_pushButtonSetRxAtt2_clicked();

    void on_pushButtonSetIFFilterSw_clicked();

    void on_pushButtonGetRxDet_clicked();

    void on_pushButtonGetRxTemp_clicked();

    void on_pushButtonSetSN_clicked();

    void on_pushButtonGetSN_clicked();

private:
    Ui::Q_SP1401_R1A_Widget *ui;
};

#endif
