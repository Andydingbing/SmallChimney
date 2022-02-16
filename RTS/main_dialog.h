#ifndef MAIN_DIALOG_H
#define MAIN_DIALOG_H

#include <QDialog>
#include "ui_main_dialog.h"

class MainDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MainDialog(QWidget *parent = nullptr);
    ~MainDialog();

private slots:
    void on_checkBoxSequenceMode_clicked(bool checked);

    void on_pushButtonSelectSequence_clicked();

    void on_pushButtonCompile_clicked();

    void on_checkBoxManualMode_clicked(bool checked);

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

public:
    Ui::Q_Main_Dialog *ui;
};

#endif
