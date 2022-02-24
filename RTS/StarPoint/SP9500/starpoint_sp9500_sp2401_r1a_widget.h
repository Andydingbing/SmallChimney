#ifndef STARPOINT_SP9500_SP2401_R1A_WIDGET_H
#define STARPOINT_SP9500_SP2401_R1A_WIDGET_H

#include "starpoint_sp9500_global.h"

class Q_StarPoint_SP9500_SP2401_R1A_Widget : public Q_Widget
{
    Q_OBJECT

public:
    Q_StarPoint_SP9500_SP2401_R1A_Widget(QWidget *parent = nullptr);
    ~Q_StarPoint_SP9500_SP2401_R1A_Widget();

private slots:
    void on_pushButtonSetDAClock_clicked();

    void on_pushButtonSetDASw_clicked();

    void on_pushButtonResetDA_clicked();

    void on_pushButtonSetDDSSrc_clicked();

    void on_pushButtonSetDDS1_clicked();

    void on_pushButtonSetDDS2_clicked();

    void on_pushButtonSetTxPowerComp_clicked();

    void on_pushButtonSetDUC_clicked();

    void on_checkBoxByPassTxFilter_clicked(bool checked);

    void on_pushButtonSetTxFilter_clicked();

    void on_pushButtonSetADClock_clicked();

    void on_pushButtonSetADSw_clicked();

    void on_pushButtonSetDDC_clicked();

    void on_pushButtonGetManualPower_clicked();

    void on_pushButtonGetADManual_clicked();

    void on_pushButtonGetADFPGA_clicked();

    void on_pushButtonSetRxPowerComp_clicked();

    void on_pushButtonSetRxFilter_clicked();

    void on_lineEditIQPhaseI_textEdited(const QString &arg1);

    void on_lineEditIQAmpI_textEdited(const QString &arg1);

    void on_lineEditIQAmpQ_textEdited(const QString &arg1);

    void on_lineEditDCOffsetI_textEdited(const QString &arg1);

    void on_lineEditDCOffsetQ_textEdited(const QString &arg1);

    void on_lineEditDUC_textEdited(const QString &arg1);

    void on_lineEditDDS1_textEdited(const QString &arg1);

private:
    Ui::Q_StarPoint_SP9500_SP2401_R1A_Widget *ui;
};

#endif
