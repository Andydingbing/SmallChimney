#ifndef Q_DT3308_BB_F_Widget_H
#define Q_DT3308_BB_F_Widget_H

#include "global.h"

namespace Ui {
class Q_DT3308_BB_F_Widget;
}

class Q_DT3308_BB_F_Widget : public Q_RD_Widget
{
    Q_OBJECT

public:
    explicit Q_DT3308_BB_F_Widget(QWidget *parent = nullptr);
    ~Q_DT3308_BB_F_Widget();

private slots:
    void on_pushButtonSetDAClock_clicked();

    void on_pushButtonResetDA_clicked();

    void on_pushButtonSetDLSrc_clicked();

    void on_pushButtonSetDDS2_clicked();

    void on_pushButtonSetTxPowerComp_clicked();

    void on_pushButtonSetDUC_clicked();

    void on_lineEditDUC_textEdited(const QString &arg1);

    void on_pushButtonSetDDC_clicked();

    void on_pushButtonGetManualPower_clicked();

    void on_pushButtonGetADManual_clicked();

    void on_pushButtonGetADFPGA_clicked();

    void on_pushButtonSetRxPowerComp_clicked();

    void on_lineEditIQPhaseI_textEdited(const QString &arg1);

    void on_lineEditIQPhaseQ_textEdited(const QString &arg1);

    void on_lineEditDCOffsetI_textEdited(const QString &arg1);

    void on_lineEditDCOffsetQ_textEdited(const QString &arg1);

private:
    Ui::Q_DT3308_BB_F_Widget *ui;
};

#endif // Q_DT3308_BB_F_Widget_H
