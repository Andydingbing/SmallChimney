#ifndef Q_ERICSSON_AIR_3268_B42_Widget_H
#define Q_ERICSSON_AIR_3268_B42_Widget_H

#include "ericsson_air_3268_b42_global.h"

namespace ns_ericsson {
namespace ns_air_3268 {

class Q_3268_Widget : public Q_Widget
{
    Q_OBJECT

public:
    explicit Q_3268_Widget(QWidget *parent = nullptr);
    ~Q_3268_Widget();

public slots:
    void init();

private slots:
    void on_lineEditTX_Freq_textEdited(const QString &arg1);

    void on_checkBoxTX_clicked(bool checked);

    void on_comboBoxTX_StepAtt_activated(int index);

    void on_checkBoxTX_PA_Bias_clicked(bool checked);

    void on_pushButtonGetTX_PA_Current_clicked();

    void on_lineEditTX_VGA_textEdited(const QString &arg1);

    void on_lineEditDL_CW_textEdited(const QString &arg1);

    void on_checkBoxDL_CW_clicked(bool checked);

    void on_lineEditRX_Freq_textEdited(const QString &arg1);

    void on_checkBoxRX_LNA_clicked(bool checked);

    void on_checkBoxRX_Att_clicked(bool checked);

    void on_checkBoxRX_AGC_clicked(bool checked);

    void on_lineEditRX_VGA_textEdited(const QString &arg1);

    void on_lineEditUL_Gain_textEdited(const QString &arg1);

    void on_comboBoxRX_Type_activated(int index);

private:
    Ui::Q_Ericsson_Air_3268_B42_Widget *ui;
};

} // namespace ns_air_3268
} // namespace ns_ericsson

#endif
