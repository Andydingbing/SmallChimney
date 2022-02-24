#ifndef Q_LMX2594_DLG_H
#define Q_LMX2594_DLG_H

#include <QDialog>
#include "sp9500x_sp1403_r1a.h"

namespace Ui {
class Q_LMX2594_Dlg;
}

class Q_LMX2594_Dlg : public QDialog
{
    Q_OBJECT

public:
    explicit Q_LMX2594_Dlg(QWidget *parent = nullptr);
    ~Q_LMX2594_Dlg();

    void updateFreq();
    void updateReg0x00();
    void updateReg0x04();
    void updateReg0x0e();
    void updateReg0x1f();
    void updateReg0x25();
    void updateReg0x28();
    void updateReg0x29();
    void updateReg0x2c();
    void updateReg0x2d();
    void updateReg0x2e();
    void updateReg0x3b();
    void updateReg0x3c();
    void updateReg0x4b();

private slots:
    void on_lineEditRFFreq_textEdited(const QString &/*arg1*/) { updateFreq(); }

    void on_lineEditRefFreq_textEdited(const QString &/*arg1*/) { updateFreq(); }

    void on_checkBoxOSCinDoubler_clicked(bool checked);

    void on_lineEditPreRDiv_textEdited(const QString &arg1);

    void on_comboBoxMultipiler_activated(int index);

    void on_lineEditPostRDiv_textEdited(const QString &arg1);

    void on_checkBoxOUT_MUTE_clicked(bool checked);

    void on_checkBoxFCAL_clicked(bool checked);

    void on_comboBoxMUXOUT_LD_activated(int index);

    void on_checkBoxRESET_clicked(bool checked);

    void on_checkBoxPOWER_DOWN_clicked(bool checked);

    void on_comboBoxCPG_activated(int index);

    void on_lineEditACAL_CMP_DELAY_textEdited(const QString &arg1);

    void on_checkBoxMASH_SEED_EN_clicked(bool checked);

    void on_lineEditMASH_SEED_textEdited(const QString &arg1);

    void on_checkBoxMASH_RESET_N_clicked(bool checked);

    void on_comboBoxMASH_ORDER_activated(int index);

    void on_lineEditOUTA_PWR_textEdited(const QString &arg1);

    void on_checkBoxOUTB_PD_clicked(bool checked);

    void on_checkBoxOUTA_PD_clicked(bool checked);

    void on_comboBoxOUTA_MUX_activated(int index);

    void on_lineEditOUT_ISEL_textEdited(const QString &arg1);

    void on_lineEditOUTB_PWR_textEdited(const QString &arg1);

    void on_comboBoxOUTB_MUX_activated(int index);

    void on_comboBoxLD_TYPE_activated(int index);

    void on_lineEditLD_DLY_textEdited(const QString &arg1);

    void on_checkBoxSEG1_clicked(bool checked);

    void on_comboBoxCHDIV_activated(int index);

    void on_pushButtonWrite_clicked();

    void on_pushButtonRead_clicked();

public:
    rd::sp1403_r1a *RF;
    rd::sp1403::lo_t LO;

private:
    Ui::Q_LMX2594_Dlg *ui;
};

#endif // Q_LMX2594_DLG_H
