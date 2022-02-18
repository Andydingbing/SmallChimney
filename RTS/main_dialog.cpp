#include "main_dialog.h"
#include "mainwindow.h"
#include <QFileDialog>

MainDialog::MainDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Q_Main_Dialog)
{
    ui->setupUi(this);

    ui->checkBoxSequenceMode->setChecked(true);
    ui->checkBoxManualMode->setChecked(false);
}

MainDialog::~MainDialog()
{
    delete ui;
}

void MainDialog::on_checkBoxSequenceMode_clicked(bool checked)
{
    ui->pushButtonSelectSequence->setEnabled(checked);
    ui->lineEditSequencePath->setReadOnly(!checked);
    ui->pushButtonCompile->setEnabled(checked);
    ui->textEditCompileError->setReadOnly(!checked);
    ui->checkBoxManualMode->setChecked(!checked);
}

void MainDialog::on_pushButtonSelectSequence_clicked()
{
    QString path = QFileDialog::getOpenFileName(this,tr("Select Main Sequence"),"",tr("Sequence File(*.ts)"));
    ui->lineEditSequencePath->setText(path);

    on_pushButtonCompile_clicked();
}

void MainDialog::on_pushButtonCompile_clicked()
{
    bool ret = g_MainW->verifySequence(ui->lineEditSequencePath->text());

    ui->textEditCompileError->setText(QString::fromStdString(mainSequence.err()));
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(!ret);
}

void MainDialog::on_checkBoxManualMode_clicked(bool checked)
{
    ui->checkBoxSequenceMode->setChecked(!checked);
    ui->pushButtonSelectSequence->setEnabled(!checked);
    ui->lineEditSequencePath->setReadOnly(checked);
    ui->pushButtonCompile->setEnabled(!checked);
    ui->textEditCompileError->setReadOnly(checked);
}

void MainDialog::on_buttonBox_accepted()
{
    mode = ui->checkBoxSequenceMode->isChecked() ? Sequence : Manual;
}

void MainDialog::on_buttonBox_rejected()
{
    QApplication::exit();
}


