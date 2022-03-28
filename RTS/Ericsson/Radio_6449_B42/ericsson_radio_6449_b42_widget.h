#ifndef Q_ERICSSON_RADIO_6449_B42_Widget_H
#define Q_ERICSSON_RADIO_6449_B42_Widget_H

#include "global.h"
#include <QMetaEnum>

class Q_6449_Widget : public Q_Widget
{
    Q_OBJECT

public:
    explicit Q_6449_Widget(QWidget *parent = nullptr);
    ~Q_6449_Widget();

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
    Ui::Q_Ericsson_6449_Widget *ui;
};

#endif
